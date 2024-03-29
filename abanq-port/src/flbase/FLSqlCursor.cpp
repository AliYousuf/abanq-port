/***************************************************************************
                     FLSqlCursor.cpp  -  description
                        -------------------
begin                : Fri Jul 27 2001
copyright            : (C) 2001-2005 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLSqlCursor.h"
#include "FLSqlQuery.h"
#include "FLApplication.h"
#include "FLTableMetaData.h"
#include "FLFieldMetaData.h"
#include "FLRelationMetaData.h"
#include "FLObjectFactory.h"
#include "FLManager.h"
#include "FLFormRecordDB.h"
#include "FLAction.h"
#include "FLUtil.h"
#include "FLSqlSavePoint.h"
#include "FLSqlDatabase.h"
#include "FLSqlConnections.h"
#include "FLAccessControl.h"
#include "FLAccessControlFactory.h"

FL_EXPORT int FLSqlCursor::transaction_ = 0;
FL_EXPORT QPtrStack < FLSqlSavePoint > * FLSqlCursor::stackSavePoints = 0;
FL_EXPORT QPtrQueue < FLSqlSavePoint > * FLSqlCursor::queueSavePoints = 0;
FL_EXPORT FLSqlSavePoint * FLSqlCursor::currentSavePoint = 0;
#ifdef FL_DEBUG
FL_EXPORT long FLSqlCursor::countRefCursor = 0;
#endif

FLSqlCursorPrivate::FLSqlCursorPrivate() :
    buffer_( 0 ), bufferCopy_( 0 ), metadata_( 0 ), edition( true ), browse( true ),
    mainFilter_( QString::null ), action_( 0 ), askForCancelChanges_( false ),
    activatedCheckIntegrity_( true ), ctxt_( 0 ), activatedCommitActions_( true ),
    populated_( false ), query_( QString::null ), inLoopRisksLocks_( false ),
    inRisksLocks_( false ), modalRisksLocks_( 0 ), timerRisksLocks_( 0 ),
    acTable_( 0 ), lastAt_( -1 ), aclDone_( false ),
    idAc_( 0 ), idAcos_( 0 ), idCond_( 0 ), id_( "000" ) {}

FLSqlCursorPrivate::~FLSqlCursorPrivate() {
  undoAcl();

  if ( bufferCopy_ )
    delete bufferCopy_;

  if ( relation )
    delete relation;

  if ( acTable_ )
    delete acTable_;
}

void FLSqlCursorPrivate::doAcl() {
  if ( !acTable_ ) {
    acTable_ = FLAccessControlFactory::create( "table" );
    acTable_->setFromObject( metadata_ );
    acosBackupTable_ = acTable_->getAcos();
    acPermBackupTable_ = acTable_->perm();
    acTable_->clear();
  }

  if ( modeAccess_ == FLSqlCursor::INSERT ||
       ( lastAt_ != -1 && lastAt_ == cursor_->at() ) )
    return;

  lastAt_ = cursor_->at();

  if ( !acosCondName_.isEmpty() ) {

    bool condTrue_ = false;

    switch ( acosCond_ ) {
      case FLSqlCursor::VALUE:
        condTrue_ = ( cursor_->QSqlCursor::value( acosCondName_ ) == acosCondVal_ );
        break;
      case FLSqlCursor::REGEXP:
        condTrue_ = ( QRegExp( acosCondVal_.toString() ).exactMatch( cursor_->QSqlCursor::value( acosCondName_ ).toString() ) );
        break;
      case FLSqlCursor::FUNCTION: {
        buffer_ = cursor_->QSqlCursor::editBuffer( true );
        FLSqlCursorInterface * cI = FLSqlCursorInterface::sqlCursorInterface( cursor_ );
        condTrue_ = ( static_cast<FLApplication *>( qApp )->call( acosCondName_, QSArgumentList( cI ), 0 ).variant() == acosCondVal_ );
      }
      break;
    }

    if ( condTrue_ ) {
      if ( acTable_->name() != id_ ) {
        acTable_->clear();
        acTable_->setName( id_ );
        acTable_->setPerm( acPermTable_ );
        acTable_->setAcos( acosTable_ );
        acTable_->processObject( metadata_ );
        aclDone_ = true;
      }
      return;
    }
  } else
    if ( cursor_->isLocked() || ( cursorRelation_ && cursorRelation_->isLocked() ) ) {
      if ( acTable_->name() != id_ ) {
        acTable_->clear();
        acTable_->setName( id_ );
        acTable_->setPerm( "r-" );
        acTable_->processObject( metadata_ );
        aclDone_ = true;
      }
      return;
    }

  undoAcl();
}

void FLSqlCursorPrivate::undoAcl() {
  if ( acTable_ && aclDone_ ) {
    aclDone_ = false;
    acTable_->clear();
    acTable_->setPerm( acPermBackupTable_ );
    acTable_->setAcos( acosBackupTable_ );
    acTable_->processObject( metadata_ );
  }
}

FLSqlCursor::FLSqlCursor( const QString & name, bool autopopulate,
                          const QString & connectionName, FLSqlCursor * cR,
                          FLRelationMetaData * r, QObject * parent ) :
    QObject( parent, name + QDateTime::currentDateTime().toString( "ddMMyyyyhhmmsszzz" ) + "-K" ),
    QSqlCursor( QString::null, autopopulate, FLSqlConnections::database( static_cast<FLApplication *>( qApp )->connectionNameForAction( name ).isEmpty() ?
                connectionName : static_cast<FLApplication *>( qApp )->connectionNameForAction( name ) )->db() ) {

  d = new FLSqlCursorPrivate();
  d->cursor_ = this;

  QString conN = static_cast<FLApplication *>( qApp )->connectionNameForAction( name );

  d->db_ = FLSqlConnections::database( conN.isEmpty() ? connectionName : conN );

  if ( !name.isEmpty() ) {
    if ( !d->db_->manager()->existsTable( name ) )
      d->metadata_ = d->db_->manager()->createTable( name );
    else
      d->metadata_ = d->db_->manager()->metadata( name );
  }

  d->cursorRelation_ = cR;
  if ( r ) {
    d->relation = new FLRelationMetaData( r->foreignTable(), r->foreignField(), r->cardinality(), r->deleteCascade(), r->updateCascade(), r->checkIn() );
    d->relation->setField( r->field() );
  } else
    d->relation = 0;

  if ( !d->metadata_ )
    return;

  if ( d->metadata_->isQuery() ) {
    FLSqlQuery *qry = d->db_->manager()->query( d->metadata_->query(), this );
    d->query_ = qry->sql();
    if ( qry && !d->query_.isEmpty() )
      exec( d->query_ );
  } else
    QSqlCursor::setName( d->metadata_->name(), autopopulate );

  d->modeAccess_ = BROWSE;
  if ( cR && r ) {
    disconnect( cR, SIGNAL( bufferChanged( const QString & ) ), this, SLOT( refresh( const QString & ) ) );
    connect( cR, SIGNAL( bufferChanged( const QString & ) ), this, SLOT( refresh( const QString & ) ) );
  } else
    seek( 0 );

  d->timer = new QTimer( this, QObject::name() );
  connect( d->timer, SIGNAL( timeout() ), SLOT( refreshDelayed() ) );

  if ( !stackSavePoints && !d->db_->canSavePoint() ) {
    stackSavePoints = new QPtrStack < FLSqlSavePoint >;
    stackSavePoints->setAutoDelete( true );
  }

  if ( !queueSavePoints && !d->db_->canSavePoint() ) {
    queueSavePoints = new QPtrQueue < FLSqlSavePoint >;
    queueSavePoints->setAutoDelete( true );
  }

  d->fieldsNamesUnlock_ = d->metadata_->fieldsNamesUnlock();

#ifdef FL_DEBUG
  ++countRefCursor;
#endif
}

FLSqlCursor::FLSqlCursor( const QString & name, bool autopopulate,
                          QSqlDatabase * db, FLSqlCursor * cR,
                          FLRelationMetaData * r, QObject * parent ) :
    QObject( parent, name + QDateTime::currentDateTime().toString( "ddMMyyyyhhmmsszzz" ) + "-K" ),
    QSqlCursor( QString::null, autopopulate, db ) {

  d = new FLSqlCursorPrivate();
  d->cursor_ = this;

  d->db_ = FLSqlConnections::database();

  if ( !name.isEmpty() ) {
    if ( !d->db_->manager()->existsTable( name ) )
      d->metadata_ = d->db_->manager()->createTable( name );
    else
      d->metadata_ = d->db_->manager()->metadata( name );
  }

  d->cursorRelation_ = cR;
  if ( r ) {
    d->relation = new FLRelationMetaData( r->foreignTable(), r->foreignField(), r->cardinality(), r->deleteCascade(), r->updateCascade(), r->checkIn() );
    d->relation->setField( r->field() );
  } else
    d->relation = 0;

  if ( !d->metadata_ )
    return;

  if ( d->metadata_->isQuery() ) {
    FLSqlQuery *qry = d->db_->manager()->query( d->metadata_->query(), this );
    d->query_ = qry->sql();
    if ( qry && !d->query_.isEmpty() )
      exec( d->query_ );
  } else
    QSqlCursor::setName( d->metadata_->name(), autopopulate );

  d->modeAccess_ = BROWSE;
  if ( cR && r ) {
    disconnect( cR, SIGNAL( bufferChanged( const QString & ) ), this, SLOT( refresh( const QString & ) ) );
    connect( cR, SIGNAL( bufferChanged( const QString & ) ), this, SLOT( refresh( const QString & ) ) );
  } else
    seek( 0 );

  d->timer = new QTimer( this, QObject::name() );
  connect( d->timer, SIGNAL( timeout() ), SLOT( refreshDelayed() ) );

  if ( !stackSavePoints && !d->db_->canSavePoint() ) {
    stackSavePoints = new QPtrStack < FLSqlSavePoint >;
    stackSavePoints->setAutoDelete( true );
  }

  if ( !queueSavePoints && !d->db_->canSavePoint() ) {
    queueSavePoints = new QPtrQueue < FLSqlSavePoint >;
    queueSavePoints->setAutoDelete( true );
  }

  d->fieldsNamesUnlock_ = d->metadata_->fieldsNamesUnlock();

#ifdef FL_DEBUG
  ++countRefCursor;
#endif
}

FLSqlCursor::~FLSqlCursor() {
  if ( !d->transactionsOpened.isEmpty() )
    rollbackOpened( -1, tr( "Se han detectado transacciones no finalizadas en la �ltima operaci�n.\n" ) +
                    tr( "Se van a cancelar las transacciones pendientes.\n" ) );
  delete d;
#ifdef FL_DEBUG
  --countRefCursor;
#endif
}

void FLSqlCursor::refreshDelayed( int msec ) {
  if ( !d->timer )
    return;
  const QObject * obj = sender();
  if ( obj ) {
    if ( !obj->inherits( "QTimer" ) ) {
      d->timer->start( msec, true );
      return ;
    }
  } else {
    d->timer->start( msec, true );
    return ;
  }
  QString cFilter( QSqlCursor::filter() );
  setFilter( "" );
  if ( cFilter == QSqlCursor::filter() && isValid() )
    return;
  QSqlCursor::select();
  int pos = atFrom();
  if ( pos >= size() )
    pos = size() - 1;
  if ( !seek( pos, false, true ) ) {
    d->buffer_ = 0;
    emit newBuffer();
  } else {
    if ( d->cursorRelation_ && d->relation && d->cursorRelation_->metadata() ) {
      QVariant v( valueBuffer( d->relation->field() ) );
      if ( d->cursorRelation_->valueBuffer( d->relation->foreignField() ) != v )
        d->cursorRelation_->setValueBuffer( d->relation->foreignField(), v );
    }
  }
}

void FLSqlCursor::refresh( const QString & fN ) {
  if ( !d->metadata_ )
    return ;

  if ( d->cursorRelation_ && d->relation ) {
    d->persistentFilter_ = QString::null;
    if ( !d->cursorRelation_->metadata() )
      return ;
    if ( d->cursorRelation_->metadata() ->primaryKey() == fN && d->cursorRelation_->modeAccess() == INSERT )
      return ;
    if ( fN.isEmpty() || d->relation->foreignField() == fN ) {
      d->buffer_ = 0;
      refreshDelayed();
    }
  } else {
    QSqlCursor::select();
    int pos = atFrom();
    if ( pos >= size() )
      pos = size() - 1;
    if ( !seek( pos, false, true ) ) {
      d->buffer_ = 0;
      emit newBuffer();
    }
  }
}

bool FLSqlCursor::setEditMode() {
  if ( d->modeAccess_ == INSERT ) {
    if ( !commitBuffer() ) {
      return false;
    }
    refresh();
    setModeAccess( EDIT );
    return refreshBuffer();
  } else
    if ( d->modeAccess_ == EDIT )
      return true;

  return false;
}

bool FLSqlCursor::commitBufferCursorRelation() {
  switch ( d->modeAccess_ ) {
    case INSERT:
      if ( d->cursorRelation_ && d->relation ) {
        if ( d->cursorRelation_->metadata() && d->cursorRelation_->modeAccess() == INSERT ) {
          if ( !d->cursorRelation_->commitBuffer() ) {
            d->modeAccess_ = BROWSE;
            return false;
          }
          setFilter( "" );
          d->cursorRelation_->refresh();
          d->cursorRelation_->setModeAccess( EDIT );
          d->cursorRelation_->refreshBuffer();
        }
      }
      break;

    case BROWSE:
    case EDIT:
      if ( d->cursorRelation_ && d->relation ) {
        if ( d->cursorRelation_->metadata() && d->cursorRelation_->modeAccess() == INSERT ) {
          if ( !d->cursorRelation_->commitBuffer() ) {
            d->modeAccess_ = BROWSE;
            return false;
          }
          d->cursorRelation_->refresh();
          d->cursorRelation_->setModeAccess( EDIT );
          d->cursorRelation_->refreshBuffer();
        }
      }
      break;
  }

  return true;
}

bool FLSqlCursor::refreshBuffer() {
  if ( !d->metadata_ )
    return false;

  if ( !isValid() && d->modeAccess_ != INSERT )
    return false;

  switch ( d->modeAccess_ ) {
    case INSERT: {
      if ( !commitBufferCursorRelation() )
        return false;

      d->buffer_ = primeInsert();
      setNotGenerateds();

      FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();

      if ( fieldList ) {
        FLFieldMetaData * field;
        QDictIterator<FLFieldMetaData> it( *fieldList );
        QString fiName;
        int type;
        QVariant::Type fltype;
        QVariant defVal;

        while ( ( field = it.current() ) != 0 ) {
          ++it;

          fiName = field->name();
          d->buffer_->setNull( fiName );
          if ( !d->buffer_->isGenerated( fiName ) )
            continue;

          type = field->type();
          fltype = FLFieldMetaData::flDecodeType( type );
          defVal = field->defaultValue();
          if ( defVal.isValid() ) {
            defVal.cast( fltype );
            d->buffer_->setValue( fiName, defVal );
          }

          if ( type == FLFieldMetaData::Serial )
            d->buffer_->setValue( fiName, d->db_->nextSerialVal( d->metadata_->name(), fiName ).toUInt() );

          if ( field->isCounter() ) {
            if ( d->ctxt_ ) {
              QVariant siguiente = static_cast<FLApplication *>( qApp ) ->call( "calculateCounter", QSArgumentList( fiName ), d->ctxt_ ).variant();
              if ( siguiente.isValid() ) {
                d->buffer_->setValue( fiName, siguiente );
              } else {
                siguiente = FLUtil::nextCounter( fiName, this );
                if ( siguiente.isValid() )
                  d->buffer_->setValue( fiName, siguiente );
              }
            }
          }
        }
      }

      if ( d->cursorRelation_ && d->relation && d->cursorRelation_->metadata() )
        setValueBuffer( d->relation->field(), d->cursorRelation_->valueBuffer( d->relation->foreignField() ) );

      d->undoAcl();

      updateBufferCopy();
      emit newBuffer();
    }
    break;

    case EDIT: {
      if ( !commitBufferCursorRelation() )
        return false;

      d->buffer_ = primeUpdate();
      setNotGenerateds();

      updateBufferCopy();
      emit newBuffer();
    }
    break;

    case DEL:
      if ( isLocked() ) {
        QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ),
                              tr( "Registro bloqueado, no se puede eliminar" ), QMessageBox::Ok, 0, 0 );
        d->modeAccess_ = BROWSE;
        return false;
      }

      d->buffer_ = primeDelete();
      setNotGenerateds();

      updateBufferCopy();
      break;

    case BROWSE:
      d->buffer_ = editBuffer( true );
      setNotGenerateds();
      emit newBuffer();
      break;
  }

  return true;
}

bool FLSqlCursor::checkIntegrity( bool showError ) {
  if ( !d->buffer_ || !d->metadata_ )
    return false;

  if ( !d->activatedCheckIntegrity_ )
    return true;

  if ( d->modeAccess_ == INSERT || d->modeAccess_ == EDIT ) {
    if ( !isModifiedBuffer() && d->modeAccess_ == EDIT )
      return true;
    FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();
    bool checkedCK = false;

    if ( !fieldList )
      return true;

    FLFieldMetaData * field;
    QString msg = "\n", fiName, fmdName;
    QVariant s, ss;

    QDictIterator<FLFieldMetaData> it( *fieldList );
    while ( ( field = it.current() ) != 0 ) {
      ++it;

      fiName = field->name();
      if ( !d->buffer_->isGenerated( fiName ) )
        continue;

      s.clear();
      if ( !d->buffer_->isNull( fiName ) ) {
        s = d->buffer_->value( fiName );
        if ( s.toString().isEmpty() )
          s.clear();
      }

      FLFieldMetaData * fMD = field->associatedField();
      if ( fMD && s.isValid() && !s.isNull() ) {
        if ( !field->relationM1() ) {
          msg += "\n" + tr( "FLSqlCursor : Error en metadatos, el campo %1 tiene un campo asociado pero no existe relaci�n muchos a uno" ).arg( d->metadata_->name() + ":" + fiName );
          continue;
        }
        FLTableMetaData *tMD = d->db_->manager()->metadata( field->relationM1()->foreignTable() );
        if ( !tMD )
          continue;
        fmdName = fMD->name();
        ss.clear();
        if ( !d->buffer_->isNull( fmdName ) ) {
          ss = d->buffer_->value( fmdName );
          if ( ss.toString().isEmpty() )
            ss.clear();
        }
        if ( ss.isValid() && !ss.isNull() ) {
          QString filter = d->db_->manager()->formatAssignValue( field->associatedFieldFilterTo(), fMD, ss, true );
          filter += " AND ";
          filter += d->db_->manager()->formatAssignValue( field->relationM1() ->foreignField(), field, s, true );
          FLSqlQuery q( 0, d->db_->connectionName() );
          q.setTablesList( tMD->name() );
          q.setSelect( field->associatedFieldFilterTo() );
          q.setFrom( tMD->name() );
          q.setWhere( filter );
          q.setForwardOnly( true );
          q.exec();
          if ( !q.next() )
            msg += "\n" + d->metadata_->name() + ":" + field->alias() + tr( " : %1 no pertenece a %2" ).arg( s.toString(), ss.toString() );
          else
            d->buffer_->setValue( fmdName, q.value( 0 ) );
        } else
          msg += "\n" + d->metadata_->name() + ":" + field->alias() + tr( " : %1 no se puede asociar a un valor NULO" ).arg( s.toString() );
      }

      if ( d->modeAccess_ == EDIT && d->buffer_->value( fiName ) == d->bufferCopy_->value( fiName ) )
        continue;

      if ( d->buffer_->isNull( fiName ) && !field->allowNull() && field->type() != FLFieldMetaData::Serial )
        msg += "\n" + d->metadata_->name() + ":" + field->alias() + tr( " : No puede ser nulo" );

      if ( field->isUnique() ) {
        QString pK = d->metadata_->primaryKey();
        if ( !d->buffer_->isNull( pK ) && s.isValid() && !s.isNull() ) {
          QVariant pKV( d->buffer_->value( pK ) );
          FLSqlQuery q( 0, d->db_->connectionName() );
          q.setTablesList( d->metadata_->name() );
          q.setSelect( fiName );
          q.setFrom( d->metadata_->name() );
          q.setWhere( d->db_->manager()->formatAssignValue( field, s, true ) +
                      " AND " + d->metadata_->primaryKey( d->metadata_->isQuery() ) + "<>" +
                      d->db_->manager()->formatValue( d->metadata_->fieldType( pK ), pKV ) );
          q.setForwardOnly( true );
          q.exec();
          if ( q.next() )
            msg += "\n" + d->metadata_->name() + ":" + field->alias() +
                   tr( " : Requiere valores �nicos, y ya hay otro registro con el valor %1 en este campo" ).arg( s.toString() );
        }
      }

      if ( field->isPrimaryKey() && d->modeAccess_ == INSERT && s.isValid() && !s.isNull() ) {
        FLSqlQuery q( 0, d->db_->connectionName() );
        q.setTablesList( d->metadata_->name() );
        q.setSelect( fiName );
        q.setFrom( d->metadata_->name() );
        q.setWhere( d->db_->manager()->formatAssignValue( field, s, true ) );
        q.setForwardOnly( true );
        q.exec();
        if ( q.next() )
          msg += "\n" + d->metadata_->name() + ":" + field->alias() + tr( " : Es clave primaria y requiere valores �nicos, y ya hay otro registro con el valor %1 en este campo" ).arg( s.toString() );
      }

      if ( field->relationM1() && s.isValid() && !s.isNull() ) {
        if ( field->relationM1()->checkIn() && field->relationM1() ->foreignTable() != d->metadata_->name() ) {
          const FLRelationMetaData * r = field->relationM1();
          FLTableMetaData *tMD = d->db_->manager()->metadata( r->foreignTable() );
          if ( !tMD )
            continue;
          FLSqlQuery q( 0, d->db_->connectionName() );
          q.setTablesList( tMD->name() );
          q.setSelect( r->foreignField() );
          q.setFrom( tMD->name() );
          q.setWhere( d->db_->manager()->formatAssignValue( r->foreignField(), field, s, true ) );
          q.setForwardOnly( true );
          q.exec();
          if ( !q.next() )
            msg += "\n" + d->metadata_->name() + ":" + field->alias() + tr( " : El valor %1 no existe en la tabla %2" ).arg( s.toString(), r->foreignTable() );
          else
            d->buffer_->setValue( fiName, q.value( 0 ) );
        }
      }

      FLTableMetaData::FLFieldMetaDataList * fieldListCK = d->metadata_->fieldListOfCompoundKey( fiName );
      if ( fieldListCK && !checkedCK && d->modeAccess_ == INSERT ) {
        if ( !fieldListCK->isEmpty() ) {
          FLFieldMetaData * fieldCK;
          QString filter;
          QString fields;
          QString valuesFields;
          QDictIterator<FLFieldMetaData> it( *fieldListCK );
          while ( ( fieldCK = it.current() ) != 0 ) {
            ++it;
            QVariant sCK = d->buffer_->value( fieldCK->name() );
            if ( filter.isEmpty() )
              filter += d->db_->manager()->formatAssignValue( fieldCK, sCK, true );
            else
              filter += " AND " + d->db_->manager()->formatAssignValue( fieldCK, sCK, true );
            if ( fields.isEmpty() )
              fields += fieldCK->alias();
            else
              fields += "+" + fieldCK->alias();
            if ( valuesFields.isEmpty() )
              valuesFields += sCK.toString();
            else
              valuesFields += "+" + sCK.toString();
          }
          FLSqlQuery q( 0, d->db_->connectionName() );
          q.setTablesList( d->metadata_->name() );
          q.setSelect( fiName );
          q.setFrom( d->metadata_->name() );
          q.setWhere( filter );
          q.setForwardOnly( true );
          q.exec();
          if ( q.next() )
            msg += "\n" + fields + tr( " : Requiere valor �nico, y ya hay otro registro con el valor %1" ).arg( valuesFields );
          checkedCK = true;
        }
      }
    }

    if ( msg != "\n" ) {
      if ( showError ) {
        QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ), tr( "No se puede validar el registro actual, porque :" ) + msg, QMessageBox::Ok, 0, 0 );
      }

      return false;
    }
  }

  if ( d->modeAccess_ == DEL ) {
    FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();
    FLFieldMetaData *field;
    QString msg, fiName;
    QVariant s;
    QDictIterator<FLFieldMetaData> it( *fieldList );

    while ( ( field = it.current() ) != 0 ) {
      ++it;

      fiName = field->name();
      if ( !d->buffer_->isGenerated( fiName ) )
        continue;

      s.clear();
      if ( !d->buffer_->isNull( fiName ) ) {
        s = d->buffer_->value( fiName );
        if ( s.toString().isEmpty() )
          s.clear();
      }

      if ( !s.isValid() || s.isNull() )
        continue;

      FLFieldMetaData::FLRelationMetaDataList * relationList = field->relationList();

      if ( !relationList )
        continue;
      if ( !relationList->isEmpty() ) {
        FLRelationMetaData * r;
        QPtrListIterator<FLRelationMetaData> itR( *relationList );

        while ( ( r = itR.current() ) != 0 ) {
          ++itR;
          if ( !r->checkIn() )
            continue;
          FLTableMetaData *mtd = d->db_->manager()->metadata( r->foreignTable() );
          if ( !mtd )
            continue;
          FLFieldMetaData *f = mtd->field( r->foreignField() );
          if ( f ) {
            if ( f->relationM1() ) {
              if ( f->relationM1()->deleteCascade() )
                continue;
              if ( !f->relationM1()->checkIn() )
                continue;
            } else
              continue;
          } else {
            msg += "\n" + tr( "FLSqlCursor : Error en metadatos, %1 no es v�lido" ).arg( r->foreignField() );
            continue;
          }

          FLSqlQuery q( 0, d->db_->connectionName() );
          q.setTablesList( mtd->name() );
          q.setSelect( r->foreignField() );
          q.setFrom( mtd->name() );
          q.setWhere( d->db_->manager()->formatAssignValue( r->foreignField(), field, s, true ) );
          q.setForwardOnly( true );
          q.exec();
          if ( q.next() )
            msg += "\n" + d->metadata_->name() + ":" + field->alias() +
                   tr( " : Con el valor %1 hay registros en la tabla %2:%3" ).arg( s.toString(), mtd->name(), mtd->alias() );
        }
      }
    }

    if ( !msg.isEmpty() ) {
      QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ), tr( "No se puede borrar registro, porque : " ) + msg, QMessageBox::Ok, 0, 0 );
      return false;
    }
  }

  return true;
}

void FLSqlCursor::checkRisksLocks( bool terminate ) {
  if ( d->timerRisksLocks_ )
    d->timerRisksLocks_->stop();
  if ( d->inLoopRisksLocks_ && terminate ) {
    d->inLoopRisksLocks_ = false;
    d->inRisksLocks_ = !( d->db_->detectRisksLocks( d->metadata_->name(),
                          valueBuffer( d->metadata_->primaryKey() ).toString() ).isEmpty() );
    QApplication::eventLoop()->exitLoop();
  } else
    if ( !terminate ) {
      QStringList locks( d->db_->detectRisksLocks( d->metadata_->name(),
                         valueBuffer( d->metadata_->primaryKey() ).toString() ) );

      if ( !locks.isEmpty() ) {
        if ( !d->inLoopRisksLocks_ ) {
          d->inRisksLocks_ = true;
          d->inLoopRisksLocks_ = true;
          if ( !d->timerRisksLocks_ )
            d->timerRisksLocks_ = new QTimer( this );
          if ( !d->modalRisksLocks_ )
            d->modalRisksLocks_ = new QLabel( tr( "SISTEMA  ANTIBLOQUEOS  ACTIVADO" ), 0, 0, Qt::WStyle_Customize | Qt::WStyle_Splash | Qt::WShowModal );
          QFont lblFont( qApp->font() );
          lblFont.setPointSize( 20 );
          lblFont.setBold( true );
          d->modalRisksLocks_->setFont( lblFont );
          d->modalRisksLocks_->setFrameStyle( QFrame::Box | QFrame::Plain );
          d->modalRisksLocks_->show();

          connect( d->timerRisksLocks_, SIGNAL( timeout() ), this, SLOT( checkRisksLocks() ) );
          connect( QApplication::eventLoop(), SIGNAL( aboutToBlock() ), this, SLOT( checkRisksLocks() ) );
          QApplication::eventLoop()->enterLoop();

          d->timerRisksLocks_->deleteLater();
          d->timerRisksLocks_ = 0;
          d->modalRisksLocks_->deleteLater();
          d->modalRisksLocks_ = 0;

          if ( !d->inRisksLocks_ ) {
            QString msgLock( tr( "<p><img source=\"locked.png\" align=\"right\"><b><u>Sin Riesgo de Bloqueo</u></b><br><br>"
                                 "Ya no existe riesgo de bloqueo puede continuar con la transacci�n actual." ) );
            QMap<QString, QSArgumentList> scrCall;
            ::qt_cast<FLApplication *>( qApp )->popupWarnDbLocks( msgLock, scrCall );
          }
        } else {
          disconnect( QApplication::eventLoop(), SIGNAL( aboutToBlock() ), this, SLOT( checkRisksLocks() ) );

          QString msgLock( tr( "<p><img source=\"locked.png\" align=\"right\"><b><u>Riesgo de Bloqueo Detectado</u></b><br><br>"
                               "Va a acceder a datos que est�n siendo modificados por otro usuario en este momento, "
                               "es muy probable que su conexi�n y la aplicaci�n queden congeladas hasta que el "
                               "otro usuario termine su transacci�n.<br><br>Si le es posible postponer la operaci�n "
                               "que est� realizando se recomienda cancelar los cambios actuales y esperar "
                               "unos segundos antes de continuar."
                               "<br><br><em><a href=\"sys.terminateChecksLocks\">Cerrar sistema antibloqueos</a></em></p>"
                               "<<br><em><a href=\"sys.statusDbLocksDialog\">M�s informaci�n</a></em></p>" ) );

          QMap<QString, QSArgumentList> scrCall;
          scrCall["sys.terminateChecksLocks"] = QSArgumentList( FLSqlCursorInterface::sqlCursorInterface( this ) );
          scrCall["sys.statusDbLocksDialog"] = QSArgumentList( locks );
          ::qt_cast<FLApplication *>( qApp )->popupWarnDbLocks( msgLock, scrCall );

          if ( d->timerRisksLocks_ )
            d->timerRisksLocks_->start( 10000, true );
        }
      } else
        if ( d->inLoopRisksLocks_ )
          checkRisksLocks( true );
    }
}

bool FLSqlCursor::commitBuffer( bool emite, bool checkLocks ) {
  if ( !d->buffer_ || !d->metadata_ )
    return false;

  if ( d->db_->canDetectLocks() && checkLocks ) {
    checkRisksLocks();
    if ( d->inRisksLocks_ && QMessageBox::No == QMessageBox::warning( 0, tr( "Bloqueo inminente" ),
         tr( "Los registros que va a modificar est�n bloqueados actualmente,\n"
             "si continua hay riesgo de que su conexi�n quede congelada hasta finalizar el bloqueo.\n\n"
             "� Desa continuar aunque exista riesgo de bloqueo ?" ),
         QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape ) )
      return false;
  }

  if ( !checkIntegrity() )
    return false;

  FLApplication * flapp = ::qt_cast<FLApplication *>( qApp );

  if ( d->modeAccess_ == EDIT || d->modeAccess_ == INSERT ) {
    FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();
    FLFieldMetaData *field;

    QDictIterator<FLFieldMetaData> it( *fieldList );
    while ( ( field = it.current() ) != 0 ) {
      ++it;

      if ( !d->buffer_->isGenerated( field->name() ) )
        continue;

      if ( d->ctxt_ && field->calculated() ) {
        QVariant v = flapp->call( "calculateField", QSArgumentList( field->name() ), d->ctxt_ ).variant();
        if ( v.isValid() )
          setValueBuffer( field->name(), v );
      }
    }
  }

  QString functionBefore;
  QString functionAfter;
  if ( d->modeAccess_ != BROWSE && d->activatedCommitActions_ ) {
    QString idMod = d->db_->managerModules()->idModuleOfFile( d->metadata_->name() + ".mtd" );
    if ( !idMod.isEmpty() ) {
      functionBefore = idMod + ".beforeCommit_" + d->metadata_->name();
      functionAfter = idMod + ".afterCommit_" + d->metadata_->name();
    } else {
      functionBefore = "sys.beforeCommit_" + d->metadata_->name();
      functionAfter = "sys.afterCommit_" + d->metadata_->name();
    }
    if ( !functionBefore.isEmpty() ) {
      FLSqlCursorInterface * cI = FLSqlCursorInterface::sqlCursorInterface( this );
      QVariant v = flapp->call( functionBefore, QSArgumentList( cI ), 0 ).variant();
      if ( v.isValid() && !v.toBool() )
        return false;
    }
  }

  if ( !checkIntegrity() )
    return false;

  QString pKN = d->metadata_->primaryKey();
  bool updated = false;
  FLSqlSavePoint * savePoint = 0;

  switch ( d->modeAccess_ ) {
    case INSERT: {
      if ( d->cursorRelation_ && d->relation ) {
        if ( d->cursorRelation_->metadata() ) {
          setValueBuffer( d->relation->field(), d->cursorRelation_->valueBuffer( d->relation->foreignField() ) );
          d->cursorRelation_->setAskForCancelChanges( true );
        }
      }
      QString pKWhere( d->db_->manager()->formatAssignValue( d->metadata_->field( pKN ), valueBuffer( pKN ) ) );
      insert( false );
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint )
          currentSavePoint->saveInsert( pKN, d->buffer_, this );
      }
      if ( !functionAfter.isEmpty() && d->activatedCommitActions_ ) {
        if ( !savePoint )
          savePoint = new FLSqlSavePoint( 0 );
        savePoint->saveInsert( pKN, d->buffer_, this );
      }
      if ( !d->persistentFilter_.contains( pKWhere ) )
        d->persistentFilter_ = ( d->persistentFilter_.isEmpty() ?  pKWhere : d->persistentFilter_ + " OR " + pKWhere );
      updated = true;
    }
    break;

    case EDIT: {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint )
          currentSavePoint->saveEdit( pKN, d->bufferCopy_, this );
      }
      if ( !functionAfter.isEmpty() && d->activatedCommitActions_ ) {
        if ( !savePoint )
          savePoint = new FLSqlSavePoint( 0 );
        savePoint->saveEdit( pKN, d->bufferCopy_, this );
      }

      if ( d->cursorRelation_ && d->relation ) {
        if ( d->cursorRelation_->metadata() ) {
          if ( isModifiedBuffer() )
            d->cursorRelation_->setAskForCancelChanges( true );
        }
      }
      if ( isModifiedBuffer() ) {
        for ( uint i = 0; i < d->buffer_->count(); ++i )
          if ( ( d->buffer_->value( i ) == d->bufferCopy_->value( i ) ) && ( d->buffer_->isNull( i ) == d->bufferCopy_->isNull( i ) ) )
            d->buffer_->setGenerated( i, false );
        update( false );
        for ( uint i = 0; i < d->buffer_->count(); ++i )
          d->buffer_->setGenerated( i, true );
        updated = true;
        setNotGenerateds();
      }
    }
    break;

    case DEL: {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint )
          currentSavePoint->saveDel( pKN, d->bufferCopy_, this );
      }
      if ( !functionAfter.isEmpty() && d->activatedCommitActions_ ) {
        if ( !savePoint )
          savePoint = new FLSqlSavePoint( 0 );
        savePoint->saveDel( pKN, d->bufferCopy_, this );
      }

      if ( d->cursorRelation_ && d->relation )
        if ( d->cursorRelation_->metadata() )
          d->cursorRelation_->setAskForCancelChanges( true );
      del( false );
      updated = true;
    }
    break;

    case BROWSE:
      break;
  }

  if ( d->modeAccess_ != BROWSE && !functionAfter.isEmpty() && d->activatedCommitActions_ ) {
    FLSqlCursorInterface * cI = FLSqlCursorInterface::sqlCursorInterface( this );
    QVariant v = flapp->call( functionAfter, QSArgumentList( cI ), 0 ).variant();
    if ( v.isValid() && !v.toBool() ) {
      if ( savePoint ) {
        savePoint->undo();
        delete savePoint;
      }
      return false;
    }
  }

  if ( savePoint )
    delete savePoint;

  d->modeAccess_ = BROWSE;

  if ( updated ) {
    setFilter( "" );
    clearMapPosByPK();
  }

  if ( updated && emite )
    emit cursorUpdated();

  emit bufferCommited();

  return true;
}

bool FLSqlCursor::seek( int i, bool relative, bool emite ) {
  if ( d->modeAccess_ == DEL )
    return false;

  bool b = QSqlCursor::seek( i, relative );

  if ( b && emite )
    emit currentChanged( at() );

  if ( b )
    return refreshBuffer();

  return b;
}

void FLSqlCursor::afterSeek() {
  QSqlCursor::afterSeek();
  d->doAcl();
}

bool FLSqlCursor::next( bool emite ) {
  if ( d->modeAccess_ == DEL )
    return false;

  bool b = QSqlCursor::next();

  if ( b && emite )
    emit currentChanged( at() );

  if ( b )
    return refreshBuffer();

  return b;
}

bool FLSqlCursor::prev( bool emite ) {
  if ( d->modeAccess_ == DEL )
    return false;

  bool b = QSqlCursor::prev();

  if ( b && emite )
    emit currentChanged( at() );

  if ( b )
    return refreshBuffer();

  return b;
}

bool FLSqlCursor::first( bool emite ) {
  if ( d->modeAccess_ == DEL )
    return false;

  bool b = QSqlCursor::first();

  if ( b && emite )
    emit currentChanged( at() );

  if ( b )
    return refreshBuffer();

  return b;
}

bool FLSqlCursor::last( bool emite ) {
  if ( d->modeAccess_ == DEL )
    return false;

  bool b = QSqlCursor::last();

  if ( b && emite )
    emit currentChanged( at() );

  if ( b )
    return refreshBuffer();

  return b;
}

int FLSqlCursor::del( bool invalidate ) {
  if ( !d->metadata_ )
    return -1;

  if ( d->ctxt_ ) {
    QString f = "recordDelBefore" + d->metadata_->name();
    static_cast<FLApplication *>( qApp ) ->call( f, QSArgumentList(), d->ctxt_ );
  }

  FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();
  FLFieldMetaData *field;
  QString fiName;
  QVariant s;

  QDictIterator<FLFieldMetaData> it( *fieldList );
  while ( ( field = it.current() ) != 0 ) {
    ++it;

    fiName = field->name();
    if ( !d->buffer_->isGenerated( fiName ) )
      continue;

    s.clear();
    if ( !d->buffer_->isNull( fiName ) ) {
      s = d->buffer_->value( fiName );
      if ( s.toString().isEmpty() )
        s.clear();
    }

    if ( !s.isValid() || s.isNull() )
      continue;

    FLFieldMetaData::FLRelationMetaDataList * relationList = field->relationList();

    if ( !relationList )
      continue;

    if ( !relationList->isEmpty() ) {
      FLRelationMetaData * r;
      QPtrListIterator<FLRelationMetaData> itR( *relationList );

      while ( ( r = itR.current() ) != 0 ) {
        ++itR;

        FLSqlCursor c( r->foreignTable() );
        if ( !c.metadata() )
          continue;
        FLFieldMetaData * f = c.metadata() ->field( r->foreignField() );
        if ( f && f->relationM1() && f->relationM1() ->deleteCascade() ) {
          c.setForwardOnly( true );
          c.select( d->db_->manager()->formatAssignValue( r->foreignField(), f, s, true ) );
          while ( c.next() ) {
            c.setModeAccess( DEL );
            c.refreshBuffer();
            if ( !c.commitBuffer( false ) )
              return false;
          }
        }
      }
    }
  }

  int r = QSqlCursor::del( invalidate );

  if ( d->ctxt_ ) {
    QString f = "recordDelAfter" + d->metadata_->name();
    static_cast<FLApplication *>( qApp ) ->call( f, QSArgumentList(), d->ctxt_ );
  }

  return r;
}

void FLSqlCursor::setAtomicValueBuffer( const QString & fN, const QString & functionName ) {
  if ( !d->buffer_ || fN.isEmpty() || !d->metadata_ )
    return ;

  FLFieldMetaData * field = d->metadata_->field( fN );
  if ( !field ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::setAtomicValueBuffer() : No existe el campo " ) + d->metadata_->name() + ":" + fN );
#endif
    return ;
  }

  if ( !d->db_->dbAux() )
    return ;

  int type = field->type();
  QVariant::Type fltype = FLFieldMetaData::flDecodeType( type );
  QString pK = d->metadata_->primaryKey();
  QVariant v;

  if ( d->cursorRelation_ && d->modeAccess_ != BROWSE )
    d->cursorRelation_->commit( false );

  if ( !pK.isEmpty() && d->db_->db() != d->db_->dbAux() ) {
    QVariant pKV = d->buffer_->value( pK );
    d->db_->dbAux() ->transaction();

    QSArgumentList arglist;
    arglist.append( QSArgument( fN ) );
    arglist.append( QSArgument( d->buffer_->value( fN ) ) );
    v = static_cast<FLApplication *>( qApp ) ->call( functionName, arglist, d->ctxt_ ).variant();

    QSqlQuery q( QString::null, d->db_->dbAux() );
    if ( q.exec( "UPDATE " + d->metadata_->name() + " SET " + fN + "=" + d->db_->manager()->formatValue( type, v ) + " WHERE " + d->db_->manager()->formatAssignValue( d->metadata_->field( pK ), pKV ) ) )
      d->db_->dbAux() ->commit();
    else
      d->db_->dbAux() ->rollback();
  } else {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor : No se puede actualizar el campo de forma at�mica, porque no existe clave primaria" ) );
#endif
  }

  if ( v.isValid() )
    v.cast( fltype );
  d->buffer_->setValue( fN, v );

  emit bufferChanged( fN );
}

void FLSqlCursor::setValueBuffer( const QString & fN, const QVariant & v ) {
  if ( !d->buffer_ || fN.isEmpty() || !d->metadata_ )
    return ;

  FLFieldMetaData * field = d->metadata_->field( fN );
  if ( !field ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::setValueBuffer() : No existe el campo " ) + d->metadata_->name() + ":" + fN );
#endif
    return ;
  }

  int type = field->type();
  QVariant::Type fltype = FLFieldMetaData::flDecodeType( type );
  QVariant vv( v );

  if ( vv.type() == QVariant::Bool && fltype != QVariant::Bool )
    vv.clear();
  if ( vv.isValid() ) {
    vv.cast( fltype );
    if ( !vv.isNull() && type == QVariant::Pixmap ) {
      QString largeValue( vv.toString() );
      largeValue = d->db_->manager()->storeLargeValue( d->metadata_, largeValue );
      if ( !largeValue.isEmpty() )
        vv = largeValue;
    }
  }

  if ( field->outTransaction() && d->db_->dbAux() && d->db_->db() != d->db_->dbAux() && d->modeAccess_ != INSERT ) {
    QString pK( d->metadata_->primaryKey() );

    if ( d->cursorRelation_ && d->modeAccess_ != BROWSE )
      d->cursorRelation_->commit( false );

    if ( !pK.isEmpty() ) {
      QVariant pKV( d->buffer_->value( pK ) );
      QSqlQuery q( QString::null, d->db_->dbAux() );
      q.exec( "UPDATE " + d->metadata_->name() + " SET " + fN + "=" +
              d->db_->manager()->formatValue( type, vv ) + " WHERE " +
              d->db_->manager()->formatAssignValue( d->metadata_->field( pK ), pKV ) );
    } else {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : No se puede actualizar el campo fuera de transaccion, porque no existe clave primaria" ) );
#endif
    }
  } else
    d->buffer_->setValue( fN, vv );

  emit bufferChanged( fN );
#ifdef FL_TEST
  static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FIELD_READY, fN );
#endif
}

QVariant FLSqlCursor::valueBuffer( const QString & fN ) {
  if ( !d->buffer_ || fN.isEmpty() || !d->metadata_ )
    return QVariant();

  FLFieldMetaData * field = d->metadata_->field( fN );
  if ( !field ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::valueBuffer() : No existe el campo " ) + d->metadata_->name() + ":" + fN );
#endif
    return QVariant();
  }

  int type = field->type();
  QVariant::Type fltype = FLFieldMetaData::flDecodeType( type );
  if ( d->buffer_->isNull( fN ) )
    if ( type == QVariant::Double || type == QVariant::Int || type == QVariant::UInt )
      return 0;

  QVariant v;
  if ( field->outTransaction() && d->db_->dbAux() && d->db_->db() != d->db_->dbAux() && d->modeAccess_ != INSERT ) {
    QString pK( d->metadata_->primaryKey() );
    if ( !pK.isEmpty() ) {
      QVariant pKV( d->buffer_->value( pK ) );
      QSqlQuery q( QString::null, d->db_->dbAux() );
      if ( q.exec( "SELECT " + fN + " FROM " + d->metadata_->name() + " WHERE " +
                   d->db_->manager()->formatAssignValue( d->metadata_->field( pK ), pKV ) ) ) {
        if ( q.isActive() && q.next() )
          v = q.value( 0 );
      }
    } else {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : No se puede obtener el campo fuera de transaccion, porque no existe clave primaria" ) );
#endif
    }
  } else
    v = d->buffer_->value( fN );

  if ( v.isValid() )
    v.cast( fltype );
  if ( !v.isNull() && type == QVariant::Pixmap ) {
    QVariant vLarge( d->db_->manager()->fetchLargeValue( v.toString() ) );
    if ( vLarge.isValid() )
      return vLarge;
  }
  return v;
}

QVariant FLSqlCursor::valueBufferCopy( const QString & fN ) const {
  if ( !d->bufferCopy_ || fN.isEmpty() || !d->metadata_ )
    return QVariant();

  FLFieldMetaData * field = d->metadata_->field( fN );
  if ( !field ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::valueBufferCopy() : No existe el campo " ) + d->metadata_->name() + ":" + fN );
#endif
    return QVariant();
  }

  int type = field->type();
  QVariant::Type fltype = FLFieldMetaData::flDecodeType( type );
  if ( d->bufferCopy_->isNull( fN ) )
    if ( type == QVariant::Double || type == QVariant::Int || type == QVariant::UInt )
      return 0;

  QVariant v( d->bufferCopy_->value( fN ) );
  if ( v.isValid() )
    v.cast( fltype );
  if ( !v.isNull() && type == QVariant::Pixmap ) {
    QVariant vLarge( d->db_->manager()->fetchLargeValue( v.toString() ) );
    if ( vLarge.isValid() )
      return vLarge;
  }
  return v;
}

void FLSqlCursor::deleteRecord() {
  openFormInMode( DEL );
}

void FLSqlCursor::browseRecord() {
  openFormInMode( BROWSE );
}

void FLSqlCursor::editRecord() {
  openFormInMode( EDIT );
}

void FLSqlCursor::insertRecord() {
  openFormInMode( INSERT );
}

void FLSqlCursor::copyRecord() {
  if ( !d->metadata_ || !d->buffer_ )
    return;

  if ( !isValid() || size() <= 0 ) {
    QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ), tr( "No hay ning�n registro seleccionado" ), QMessageBox::Ok, 0, 0 );
    return;
  }

  FLTableMetaData::FLFieldMetaDataList * fieldList = d->metadata_->fieldList();
  if ( !fieldList )
    return;

  QSqlRecord *bufferAux = new QSqlRecord( *d->buffer_ );

  insertRecord();

  QDictIterator<FLFieldMetaData> it( *fieldList );
  for ( ; it.current(); ++it ) {
    if ( !( *it ) )
      continue;
    if ( d->buffer_->isNull( ( *it )->name() ) && !( *it )->isPrimaryKey() &&
         !d->metadata_->fieldListOfCompoundKey( ( *it )->name() ) && !( *it )->calculated() )
      d->buffer_->setValue( ( *it )->name(), bufferAux->value( ( *it )->name() ) );
  }

  delete bufferAux;
  emit newBuffer();
}

void FLSqlCursor::openFormInMode( int m, bool cont ) {
  if ( !d->metadata_ )
    return ;

  if ( ( !isValid() || size() <= 0 ) && m != INSERT ) {
    QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ), tr( "No hay ning�n registro seleccionado" ), QMessageBox::Ok, 0, 0 );
    return ;
  }

  d->askForCancelChanges_ = false;

  if ( m == DEL ) {
    int res = QMessageBox::information( qApp->focusWidget(),
                                        tr( "Borrar registro" ), tr( "El registro activo ser� borrado. � Est� seguro ?" ),
                                        QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape );

    if ( res == QMessageBox::No ) {
      return ;
    }

    transaction();
    d->modeAccess_ = DEL;
    if ( !refreshBuffer() ) {
      commit();
    } else {
      if ( !commitBuffer() )
        rollback();
      else
        commit();
    }
    return ;
  }

  d->modeAccess_ = m;
  if ( d->buffer_ )
    d->buffer_->clearValues( true );

  if ( !d->action_ )
    d->action_ = d->db_->manager()->action( d->metadata_->name() );

  if ( !d->action_ ) {
#ifdef FL_DEBUG
    qWarning( "FLSqlCursor : Para poder abrir un registro de edici�n se necesita una acci�n asociada al cursor, o una acci�n definida con el mismo nombre que la tabla de la que procede el cursor." );
#endif
    return ;
  }

  if ( d->action_->formRecord().isEmpty() ) {
    QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ),
                          tr( "No hay definido ning�n formulario para manejar registros de esta tabla" ), QMessageBox::Ok, 0, 0 );
    return ;
  } else {
    QSProject *p = static_cast<FLApplication *>( qApp ) ->project();
    FLFormRecordDBInterface * iface = static_cast<FLFormRecordDBInterface *>( p->object( "formRecord" + d->action_->name() ) );
    if ( iface )
      if ( iface->obj() ) {
        QMessageBox::warning( qApp->focusWidget(), tr( "Aviso" ),
                              tr( "Ya hay abierto un formulario de edici�n de registros para esta tabla.\nNo se abrir�n m�s para evitar ciclos repetitivos de edici�n de registros." ),
                              QMessageBox::Ok, 0, 0 );
        return ;
      }
  }

  if ( !refreshBuffer() )
    return ;
  FLFormRecordDB *f = new FLFormRecordDB( this, d->action_->name(), static_cast<FLApplication *>( qApp )->mainWidget(), cont );
  if ( refreshBuffer() ) {
    f->setMainWidget();
    f->setFocus();
    f->show();
    updateBufferCopy();
  } else
    f->close();
}

void FLSqlCursor::chooseRecord() {
  if ( d->edition )
    editRecord();
  else
    if ( d->browse )
      browseRecord();

  emit recordChoosed();
}

bool FLSqlCursor::fieldDisabled( const QString & fN ) {
  if ( d->modeAccess_ == INSERT || d->modeAccess_ == EDIT ) {
    if ( d->cursorRelation_ && d->relation ) {
      if ( !d->cursorRelation_->metadata() )
        return false;
      return ( d->relation->field().lower() == fN.lower() );
    } else
      return false;
  } else
    return false;
}

bool FLSqlCursor::transaction( bool lock ) {
  if ( !d->db_ && !d->db_->db() ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::transaction() : No hay conexi�n con la base de datos" ) );
#endif
    return false;
  }

  if ( transaction_ == 0 && d->db_->canTransaction() ) {
    static_cast<FLApplication *>( qApp ) ->statusHelpMsg( tr( "Iniciando transacci�n..." ) );
    if ( d->db_->db()->transaction() ) {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      }
      ++transaction_;
      d->transactionsOpened.push( transaction_ );
      if ( lock ) {
#ifdef FL_DEBUG
        qWarning( tr( "FLSqlCursor : No se puede bloquear la tabla" ) );
#endif
      }
      return true;
    } else {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : Fallo al intentar iniciar transacci�n" ) );
#endif
      return false;
    }
  } else {
    static_cast<FLApplication *>( qApp )->statusHelpMsg( tr( "Creando punto de salvaguarda %1..." ).arg( transaction_ ) );
    if ( !d->db_->canSavePoint() ) {
      if ( transaction_ == 0 ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      }
      if ( currentSavePoint )
        stackSavePoints->push( currentSavePoint );
      currentSavePoint = new FLSqlSavePoint( transaction_ );
    } else
      d->db_->savePoint( QString::number( transaction_ ) );
    ++transaction_;
    d->transactionsOpened.push( transaction_ );
    return true;
  }
}

bool FLSqlCursor::rollback() {
  if ( !d->db_ && !d->db_->db() ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::rollback() : No hay conexi�n con la base de datos" ) );
#endif
    return false;
  }

  bool cancel = false;
  if ( ( d->modeAccess_ == INSERT || d->modeAccess_ == EDIT )
       && isModifiedBuffer() && d->askForCancelChanges_ ) {
    int res = QMessageBox::information( qApp->focusWidget(),
                                        tr( "Cancelar cambios" ),
                                        tr( "Todos los cambios efectuados en el formulario actual se cancelar�n. � Est� seguro ?" ),
                                        QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape );

    if ( res == QMessageBox::No )
      return false;
    cancel = true;
  }

  if ( transaction_ > 0 ) {
    if ( !d->transactionsOpened.isEmpty() ) {
      int trans = d->transactionsOpened.pop();
      if ( trans != transaction_ )
        qWarning( tr( "FLSqlCursor : El cursor va a deshacer la transacci�n %1 pero la �ltima que inici� es la %2" )
                  .arg( transaction_ )
                  .arg( trans ) );
    } else
      qWarning( tr( "FLSqlCursor : El cursor va a deshacer la transacci�n %1 pero no ha iniciado ninguna" )
                .arg( transaction_ ) );
    transaction_--;
  } else
    return true;

  if ( transaction_ == 0 && d->db_->canTransaction() ) {
    static_cast<FLApplication *>( qApp ) ->statusHelpMsg( tr( "Deshaciendo transacci�n..." ) );
    if ( d->db_->db()->rollback() ) {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      }
      d->modeAccess_ = BROWSE;
      if ( cancel )
        QSqlCursor::select();
      return true;
    } else {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : Fallo al intentar deshacer transacci�n" ) );
#endif
      return false;
    }
  } else {
    static_cast<FLApplication *>( qApp ) ->statusHelpMsg( tr( "Restaurando punto de salvaguarda %1..." ).arg( transaction_ ) );
    if ( !d->db_->canSavePoint() ) {
      int tamQueue = queueSavePoints->count();
      int tempId;
      FLSqlSavePoint * tempSavePoint;
      for ( int i = 0; i < tamQueue; ++i ) {
        tempSavePoint = queueSavePoints->dequeue();
        tempId = tempSavePoint->id();
        if ( tempId > transaction_ || transaction_ == 0 ) {
          tempSavePoint->undo();
          delete tempSavePoint;
        } else
          queueSavePoints->enqueue( tempSavePoint );
      }
      if ( currentSavePoint ) {
        currentSavePoint->undo();
        delete currentSavePoint;
        currentSavePoint = 0;
        if ( !stackSavePoints->isEmpty() )
          currentSavePoint = stackSavePoints->pop();
      }
      if ( transaction_ == 0 ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      }
    } else
      d->db_->rollbackSavePoint( QString::number( transaction_ ) );
    d->modeAccess_ = BROWSE;
    return true;
  }
}

bool FLSqlCursor::commit( bool notify ) {
  if ( !d->db_ && !d->db_->db() ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::commit() : No hay conexi�n con la base de datos" ) );
#endif
    return false;
  }

  if ( !notify )
    emit autoCommit();

  if ( transaction_ > 0 ) {
    if ( !d->transactionsOpened.isEmpty() ) {
      int trans = d->transactionsOpened.pop();
      if ( trans != transaction_ )
        qWarning( tr( "FLSqlCursor : El cursor va a terminar la transacci�n %1 pero la �ltima que inici� es la %2" )
                  .arg( transaction_ )
                  .arg( trans ) );
    } else
      qWarning( tr( "FLSqlCursor : El cursor va a terminar la transacci�n %1 pero no ha iniciado ninguna" )
                .arg( transaction_ ) );
    transaction_--;
  } else
    return true;

  if ( transaction_ == 0 && d->db_->canTransaction() ) {
    static_cast<FLApplication *>( qApp ) ->statusHelpMsg( tr( "Terminando transacci�n..." ) );
    if ( d->db_->db()->commit() ) {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      }
      if ( notify )
        d->modeAccess_ = BROWSE;
      return true;
    } else {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : Fallo al intentar terminar transacci�n" ) );
#endif
      return false;
    }
  } else {
    static_cast<FLApplication *>( qApp ) ->statusHelpMsg( tr( "Liberando punto de salvaguarda %1..." ).arg( transaction_ ) );
    if ( ( transaction_ == 1 && d->db_->canTransaction() ) || ( transaction_ == 0 && !d->db_->canTransaction() ) ) {
      if ( !d->db_->canSavePoint() ) {
        if ( currentSavePoint ) {
          delete currentSavePoint;
          currentSavePoint = 0;
        }
        stackSavePoints->clear();
        queueSavePoints->clear();
      } else
        d->db_->releaseSavePoint( QString::number( transaction_ ) );
      if ( notify )
        d->modeAccess_ = BROWSE;
      return true;
    }
    if ( !d->db_->canSavePoint() ) {
      int tamQueue = queueSavePoints->count();
      FLSqlSavePoint * tempSavePoint;
      for ( int i = 0; i < tamQueue; ++i ) {
        tempSavePoint = queueSavePoints->dequeue();
        tempSavePoint->setId( transaction_ - 1 );
        queueSavePoints->enqueue( tempSavePoint );
      }
      if ( currentSavePoint ) {
        queueSavePoints->enqueue( currentSavePoint );
        currentSavePoint = 0;
        if ( !stackSavePoints->isEmpty() )
          currentSavePoint = stackSavePoints->pop();
      }
    } else
      d->db_->releaseSavePoint( QString::number( transaction_ ) );
    if ( notify )
      d->modeAccess_ = BROWSE;
    return true;
  }
}

bool FLSqlCursor::select( const QString & filter, const QSqlIndex & sort ) {
  if ( !d->metadata_ )
    return false;

  QString f = filter, bFilter = baseFilter();

  QString finalFilter;
  if ( bFilter.isEmpty() )
    finalFilter = f;
  else {
    if ( filter.isEmpty() || bFilter.contains( f ) )
      finalFilter = bFilter;
    else
      finalFilter = bFilter + " AND " + f;
  }

  QStringList fields;
  if ( d->metadata_->isQuery() )
    fields = sort.toStringList( d->metadata_->name() );
  else
    fields = sort.toStringList();
  finalFilter = finalFilter.simplifyWhiteSpace();
  finalFilter.replace( "=;", "= NULL;" );
  while ( finalFilter.endsWith( ";" ) )
    finalFilter.truncate( finalFilter.length() - 1 );
  int posOrderBy = finalFilter.findRev( "ORDER BY", -1, false );
  QString fieldsOrderBy;
  if ( posOrderBy != -1 ) {
    fieldsOrderBy = finalFilter.mid( posOrderBy + 8, finalFilter.length() - 8 );
    finalFilter.remove( posOrderBy, finalFilter.length() - 8 );
    if ( !fieldsOrderBy.isEmpty() )
      fields += QStringList::split( ",", fieldsOrderBy );
  }
  if ( d->metadata_ && !sort.field( d->metadata_->primaryKey() ) && !fieldsOrderBy.upper().contains( d->metadata_->primaryKey().upper() ) )
    fields << d->metadata_->primaryKey();

  if ( d->metadata_->isQuery() ) {
    FLSqlQuery *qry = d->db_->manager()->query( d->metadata_->query(), this );
    FLTableMetaData * mtdAux = d->db_->manager()->metadata( d->metadata_->name(), true );
    QStringList fL = QStringList::split( ',', mtdAux->fieldList( false ) );

    for ( QStringList::Iterator it = fL.begin(); it != fL.end(); ++it ) {
      if ( !( *it ).contains( "." ) ) {
        finalFilter.replace( d->metadata_->name() + "." + *it, *it );
        finalFilter.replace( QRegExp( "([\\W])" + *it + "([\\W])" ), "\\1" + d->metadata_->name() + "." + *it + "\\2" );
      }
    }

    if ( qry ) {
      QString where = qry->where();
      if ( where.isEmpty() )
        qry->setWhere( finalFilter );
      else
        if ( finalFilter.isEmpty() )
          qry->setWhere( where );
        else
          qry->setWhere( where + " AND " + finalFilter );

      fields = sort.toStringList();
      fL = qry->fieldList();
      for ( QStringList::Iterator it = fields.begin(); it != fields.end(); ++it ) {
        for ( QStringList::Iterator it2 = fL.begin(); it2 != fL.end(); ++it2 )
          if ( ( *it2 ).section( '.', 1, 1 ) == ( *it ).section( ' ', 0, 0 ) ) {
            if ( !fieldsOrderBy.isEmpty() )
              fieldsOrderBy += "," + ( *it2 ) + " " + ( *it ).section( ' ', 1, 1 );
            else
              fieldsOrderBy += ( *it2 ) + " " + ( *it ).section( ' ', 1, 1 );
            break;
          }
      }

      if ( !fieldsOrderBy.isEmpty() )
        qry->setOrderBy( fieldsOrderBy );
    }

    d->query_ = qry->sql();
    return exec( d->query_ );
  }

  return QSqlCursor::select( finalFilter, QSqlIndex::fromStringList( fields, this ) );
}

void FLSqlCursor::setSort( const QSqlIndex& sort ) {
  if ( d->metadata_ ) {
    QStringList fields;
    if ( d->metadata_->isQuery() )
      fields = sort.toStringList( d->metadata_->name() );
    else
      fields = sort.toStringList();
    fields << d->metadata_->primaryKey();
    QSqlIndex idx;
    idx.fromStringList( fields, this );
    QSqlCursor::setSort( idx );
  } else
    QSqlCursor::setSort( sort );
}

QString FLSqlCursor::baseFilter() {
  QString relationFilter, finalFilter;

  if ( d->cursorRelation_ && d->relation && d->metadata_ && d->cursorRelation_->metadata() ) {
    QVariant fgValue = d->cursorRelation_->valueBuffer( d->relation->foreignField() );
    FLFieldMetaData * field = d->metadata_->field( d->relation->field() );
    relationFilter = d->db_->manager()->formatAssignValue( field, fgValue, true );
  }

  if ( !d->mainFilter_.isEmpty() )
    finalFilter = d->mainFilter_;

  if ( !relationFilter.isEmpty() && !finalFilter.contains( relationFilter ) )
    if ( finalFilter.isEmpty() )
      finalFilter = relationFilter;
    else
      finalFilter += " AND " + relationFilter;

  return finalFilter;
}

QString FLSqlCursor::curFilter() {
  QString f = QSqlCursor::filter(), bFilter = baseFilter();

  while ( f.endsWith( ";" ) )
    f.truncate( f.length() - 1 );
  if ( bFilter.isEmpty() )
    return f;
  else {
    if ( f.isEmpty() || bFilter.contains( f ) )
      return bFilter;
    else
      return bFilter + " AND " + f;
  }
}

void FLSqlCursor::setMainFilter( const QString & f ) {
  d->mainFilter_ = f;
  refresh();
}

void FLSqlCursor::setFilter( const QString & filter ) {
  QString finalFilter = filter, bFilter = baseFilter();

  if ( !bFilter.isEmpty() ) {
    if ( finalFilter.isEmpty() || bFilter.contains( finalFilter ) )
      finalFilter = bFilter;
    else
      finalFilter = bFilter + " AND " + finalFilter;
  }

  if ( !finalFilter.isEmpty() && !d->persistentFilter_.isEmpty() && !finalFilter.contains( d->persistentFilter_ ) )
    finalFilter += " OR " + d->persistentFilter_;

  QSqlCursor::setFilter( finalFilter );
}

void FLSqlCursor::updateBufferCopy() {
  if ( d->bufferCopy_ )
    delete d->bufferCopy_;
  d->bufferCopy_ = new QSqlRecord( *d->buffer_ );
}

bool FLSqlCursor::isModifiedBuffer() {
  if ( !d->buffer_ || !d->bufferCopy_ )
    return false;

  for ( uint i = 0; i < d->buffer_->count(); ++i ) {
    if ( ( d->buffer_->value( i ) != d->bufferCopy_->value( i ) ) ||
         ( d->buffer_->isNull( i ) != d->bufferCopy_->isNull( i ) ) )
      return true;
  }

  return false;
}

void FLSqlCursor::setUnLock( const QString & fN, bool v ) {
  if ( !d->metadata_ || d->modeAccess_ != BROWSE )
    return ;

  if ( d->metadata_->fieldType( fN ) != FLFieldMetaData::Unlock ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLSqlCursor::setUnLock s�lo permite modificar campos del tipo Unlock" ) );
#endif
    return ;
  }

  d->buffer_ = primeUpdate();
  d->buffer_->setValue( fN, QVariant( v, 0 ) );
  update();
  refreshBuffer();
}

bool FLSqlCursor::isLocked() {
  if ( d->modeAccess_ != INSERT && !d->fieldsNamesUnlock_.isEmpty() ) {
    for ( QStringList::const_iterator it = d->fieldsNamesUnlock_.begin();
          it != d->fieldsNamesUnlock_.end(); ++it ) {
      if ( !QSqlCursor::value( *it ).toBool() )
        return true;
    }
  }
  return false;
}

void FLSqlCursor::setContext( QObject *c ) {
  d->ctxt_ = c;
}

bool FLSqlCursor::bufferIsNull( int i ) const {
  if ( d->buffer_ )
    return d->buffer_->isNull( i );
  else
    return true;
}

bool FLSqlCursor::bufferIsNull( const QString & name ) const {
  if ( d->buffer_ )
    return d->buffer_->isNull( name );
  else
    return true;
}

void FLSqlCursor::bufferSetNull( int i ) const {
  if ( d->buffer_ )
    d->buffer_->setNull( i );
}

void FLSqlCursor::bufferSetNull( const QString & name ) const {
  if ( d->buffer_ )
    d->buffer_->setNull( name );
}

bool FLSqlCursor::bufferCopyIsNull( int i ) const {
  if ( d->bufferCopy_ )
    return d->bufferCopy_->isNull( i );
  else
    return true;
}

bool FLSqlCursor::bufferCopyIsNull( const QString & name ) const {
  if ( d->bufferCopy_ )
    return d->bufferCopy_->isNull( name );
  else
    return true;
}

void FLSqlCursor::bufferCopySetNull( int i ) const {
  if ( d->bufferCopy_ )
    d->bufferCopy_->setNull( i );
}

void FLSqlCursor::bufferCopySetNull( const QString & name ) const {
  if ( d->bufferCopy_ )
    d->bufferCopy_->setNull( name );
}

bool FLSqlCursor::inTransaction() {
  return ( FLSqlCursor::transaction_ > 0 );
}

int FLSqlCursor::atFrom() {
  if ( !d->buffer_ || !d->metadata_ )
    return 0;

  QString pKN = d->metadata_->primaryKey();
  QVariant pKValue = valueBuffer( pKN );

  if ( d->mapPosByPK.contains( pKValue.toString() ) )
    return d->mapPosByPK[pKValue.toString()];

#ifndef FL_QUICK_CLIENT
  int pos = d->db_->atFrom( this );
#else
  int pos = -99;
#endif

  if ( pos == -99 ) {
    QSqlQuery q( QString::null, d->db_->db() );
    QString sql, sqlIn, cFilter = curFilter();
    FLFieldMetaData * field = d->metadata_->field( pKN );

    if ( !d->metadata_->isQuery() || pKN.contains( "." ) ) {
      sql = "SELECT " + pKN + " FROM " + d->metadata_->name();
    } else {
      FLSqlQuery *qry = d->db_->manager()->query( d->metadata_->query(), this );
      sql = "SELECT " + d->metadata_->name() + "." + pKN + " FROM " + qry->from();
    }
    if ( !cFilter.isEmpty() )
      sql += " WHERE " + cFilter;

    if ( field ) {
      if ( !cFilter.isEmpty() )
        sqlIn = sql + " AND " +
                d->db_->manager()->formatAssignValue( field, pKValue );
      else
        sqlIn = sql + " WHERE " +
                d->db_->manager()->formatAssignValue( field, pKValue );
      q.exec( sqlIn );
      if ( !q.next() ) {
        QSqlCursor::seek( at() );
        pos = isValid() ? at() : 0;
        return pos;
      }
    }

    if ( sort().count() > 0 )
      sql += " ORDER BY " + sort().toString();

    bool found = false;
    q.exec( sql );

    pos = 0;
    if ( q.first() ) {
      if ( q.value( 0 ) != pKValue ) {
        pos = q.size();
        if ( q.last() && pos > 1 ) {
          --pos;
          if ( q.value( 0 ) != pKValue ) {
            while ( q.prev() && pos > 1 ) {
              --pos;
              if ( q.value( 0 ) == pKValue ) {
                found = true;
                break;
              }
            }
          } else
            found = true;
        }
      } else
        found = true;
    }

    if ( !found ) {
      QSqlCursor::seek( at() );
      pos = isValid() ? at() : 0;
    } else
      d->mapPosByPK.replace( pKValue.toString(), pos );

    return pos;
  }

  d->mapPosByPK.replace( pKValue.toString(), pos );
  return pos;
}

int FLSqlCursor::atFromBinarySearch( const QString & fN, const QString & v, bool orderAsc ) {
  if ( !QSqlCursor::contains( fN ) || !QSqlCursor::isActive() )
    return -1;

  bool isForward = QSqlCursor::isForwardOnly();
  int ret = -1, atBackup = QSqlCursor::at();
  int ini = 0, fin = QSqlCursor::size() - 1, mid, cmp;

  QSqlCursor::setForwardOnly( false );
  while ( ini <= fin ) {
    mid = ( ini + fin ) / 2;
    QSqlCursor::seek( mid );
    cmp = ( orderAsc ?
            QString::localeAwareCompare( v, QSqlCursor::value( fN ).toString() ) :
            QString::localeAwareCompare( QSqlCursor::value( fN ).toString(), v ) );
    if ( !cmp ) {
      ret = mid;
      break;
    } else {
      if ( cmp > 0 )
        ini = mid + 1;
      else
        fin = mid - 1;
    }
    ret = ini;
  }
  QSqlCursor::seek( atBackup );
  QSqlCursor::setForwardOnly( isForward );

  return ret;
}

bool FLSqlCursor::exec( const QString& query ) {
  bool ret = QSqlCursor::exec( query );
  if ( ret && d->metadata_->isQuery() && ( d->query_ != query || !d->populated_ ) ) {
    d->query_ = query;
    QSqlCursor::clear();
    d->populated_ = false;
    populateCursor();
  }
  return ret;
}

void FLSqlCursor::populateCursor() {
  if ( !d->metadata_ )
    return;

  if ( d->metadata_->isQuery() ) {
    QSqlCursor::setName( d->metadata_->name(), true );
    FLTableMetaData * mtd = d->metadata_;
    QStringList fieldsNames = QStringList::split( ",", mtd->fieldsNames() );
    int i = 0;
    for ( QStringList::Iterator it = fieldsNames.begin(); it != fieldsNames.end(); ++it ) {
      if ( !QSqlCursor::field( ( *it ) ) ) {
        QVariant::Type type = ( QVariant::Type )mtd->fieldType( ( *it ) );
        if ( type == FLFieldMetaData::Serial )
          type = QVariant::UInt;
        QSqlCursor::insert( i, QSqlFieldInfo( ( *it ), type, -1, mtd->fieldLength( ( *it ) ), -1, QVariant(), 0, true ) );
      }
      ++i;
    }
    d->populated_ = true;
  }
}

void FLSqlCursor::setNotGenerateds() {
  if ( d->metadata_ && d->metadata_->isQuery() && d->buffer_ ) {
    QSqlRecordInfo inf = driver()->recordInfo( *( QSqlQuery* )this );
    for ( QSqlRecordInfo::const_iterator it = inf.begin(); it != inf.end(); ++it ) {
      if ( !d->metadata_->field( ( *it ).name() ) ) {
        d->buffer_->setGenerated( ( *it ).name(), false );
        continue;
      }
      if ( !d->metadata_->field( ( *it ).name() )->generated() )
        d->buffer_->setGenerated( ( *it ).name(), false );
    }
  }
}

void FLSqlCursor::clearMapPosByPK() {
  d->mapPosByPK.clear();
}

QVariant FLSqlCursor::calculateField( const QString & name ) {
  if ( !d->metadata_ )
    return QVariant();
  return d->db_->manager()->formatValue( d->metadata_->fieldType( name ), QVariant() );
}

int FLSqlCursor::transactionLevel() {
  return transaction_;
}

QStringList FLSqlCursor::transactionsOpened() {
  QValueStack<int>::const_iterator it;
  QStringList list;
  for ( it = d->transactionsOpened.begin(); it != d->transactionsOpened.end(); ++it )
    list << QString::number( *it );
  return list;
}

void FLSqlCursor::rollbackOpened( int count, const QString & msg ) {
  int ct = ( count < 0 ? d->transactionsOpened.count() : count );
  if ( ct ) {
    if ( !msg.isEmpty() )
      static_cast<FLApplication *>( qApp )->project()->interpreter()->throwError( msg );
    for ( int i = 0; i < ct; ++i ) {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : Deshaciendo transacci�n abierta %1" ).arg( transaction_ ) );
#endif
      rollback();
    }
  }
}

void FLSqlCursor::commitOpened( int count, const QString & msg ) {
  int ct = ( count < 0 ? d->transactionsOpened.count() : count );
  if ( ct ) {
    if ( !msg.isEmpty() )
      static_cast<FLApplication *>( qApp )->project()->interpreter()->throwError( msg );
    for ( int i = 0; i < ct; ++i ) {
#ifdef FL_DEBUG
      qWarning( tr( "FLSqlCursor : Terminando transacci�n abierta %1" ).arg( transaction_ ) );
#endif
      commit();
    }
  }
}

void FLSqlCursor::setAcTable( const QString & ac ) {
  d->idAc_++;
  d->id_ = QString::number( d->idAc_ ) + QString::number( d->idAcos_ ) + QString::number( d->idCond_ );
  d->acPermTable_ = ac;
}

void FLSqlCursor::setAcosTable( const QStringList & acos ) {
  d->idAcos_++;
  d->id_ = QString::number( d->idAc_ ) + QString::number( d->idAcos_ ) + QString::number( d->idCond_ );
  d->acosTable_ = acos;
}

void FLSqlCursor::setAcosCondition( const QString & condName, AcosConditionEval cond, const QVariant & condVal ) {
  d->idCond_++;
  d->id_ = QString::number( d->idAc_ ) + QString::number( d->idAcos_ ) + QString::number( d->idCond_ );
  d->acosCondName_ = condName;
  d->acosCond_ = cond;
  d->acosCondVal_ = condVal;
}
