/***************************************************************************
                          FLFormRecordDB.cpp
                         -------------------
begin                : Sat Jul 28 2001
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
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLFormRecordDB.h"
#include "FLObjectFactory.h"
#include "FLSqlCursor.h"
#include "FLTableMetaData.h"
#include "FLAction.h"
#include "FLManager.h"
#include "FLApplication.h"
#include "FLManagerModules.h"
#include "FLAccessControlLists.h"

FLFormRecordDB::FLFormRecordDB( FLSqlCursor * cursor, const QString & actionName,
                                QWidget * parent, bool showAcceptContinue )
    : FLFormDB( parent, actionName,
                Qt::WStyle_Customize | Qt::WStyle_Maximize | Qt::WStyle_Title |
                Qt::WType_Dialog | Qt::WShowModal | Qt::WStyle_SysMenu ),
    pushButtonAccept( 0 ), pushButtonAcceptContinue( 0 ),
    pushButtonFirst( 0 ), pushButtonPrevious( 0 ), pushButtonNext( 0 ),
    pushButtonLast( 0 ), showAcceptContinue_( showAcceptContinue ),
    iface( 0 ), accepting( false ), initTransLevel( 0 ),
    initialModeAccess( cursor ? cursor->modeAccess() : FLSqlCursor::BROWSE ) {
#ifdef FL_DEBUGGER
  pushButtonIDE = 0;
#endif

  setFocusPolicy( QWidget::NoFocus );

  if ( actionName.isEmpty() )
    action_ = 0;
  else
    if ( cursor )
      action_ = cursor->db()->manager()->action( actionName );
    else
      action_ = FLSqlConnections::database()->manager()->action( actionName );
  cursor_ = cursor;
  name_ = QString::null;

  initForm();
}

FLFormRecordDB::~FLFormRecordDB() {
  if ( iface ) {
    iface->finish();
    iface->setObj( 0 );
  }
}

void FLFormRecordDB::initForm() {
  if ( cursor_ && cursor_->metadata() ) {
    if ( action_ ) {
      cursor_->setAction( action_ );
      idMDI_ = action_->name();
    }

    if ( iface )
      iface->finish();
    setName( "formRecord" + action_->name() );
    QSProject * p = static_cast<FLApplication *>( qApp ) ->project();
    iface = static_cast<FLFormRecordDBInterface *>( p->object( name() ) );
    if ( iface ) {
      iface->setObj( this );
      if ( !oldCursorCtxt )
        oldCursorCtxt = cursor_->context();
      cursor_->setContext( iface );
    }

    QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
    switch ( cursor_->modeAccess() ) {
      case FLSqlCursor::INSERT:
        cursor_->transaction();
        initTransLevel = FLSqlCursor::transactionLevel();
        setCaption( tr( "Insertar " ) + caption );
        break;

      case FLSqlCursor::EDIT:
        cursor_->transaction();
        initTransLevel = FLSqlCursor::transactionLevel();
        setCaption( tr( "Editar " ) + caption );
        break;

      case FLSqlCursor::DEL:
        break;

      case FLSqlCursor::BROWSE:
        cursor_->transaction();
        initTransLevel = FLSqlCursor::transactionLevel();
        setCaption( tr( "Visualizar " ) + caption );
        break;
    }
  } else
    setCaption( tr( "No hay metadatos" ) );
}

void FLFormRecordDB::openIde() {
#ifdef FL_DEBUGGER
  static_cast<FLApplication *>( qApp )->openIDE( iface );
#endif
}

void FLFormRecordDB::setMainWidget( QWidget * w ) {
  if ( !cursor_ || !w )
    return ;

  if ( !cursor_->metadata() )
    return ;

  if ( showed ) {
    if ( mainWidget_ && mainWidget_ != w )
      initMainWidget( w );
  } else
    w->hide();

  if ( pushButtonAcceptContinue )
    pushButtonAcceptContinue->hide();

  if ( pushButtonAccept )
    pushButtonAccept->hide();

  if ( pushButtonCancel )
    pushButtonCancel->hide();

  if ( pushButtonFirst )
    pushButtonFirst->hide();

  if ( pushButtonPrevious )
    pushButtonPrevious->hide();

  if ( pushButtonNext )
    pushButtonNext->hide();

  if ( pushButtonLast )
    pushButtonLast->hide();

  if ( layoutButtons )
    delete layoutButtons;

  if ( layout )
    delete layout;

  w->setFont( qApp->font() );
  layout = new QVBoxLayout( this, 2, 3, "vlay" + name_ );

  layout->add( w );

  layoutButtons = new QHBoxLayout( layout, 3, "hlay" + name_ ) ;
  QSpacerItem *spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding,
      QSizePolicy::Minimum );

  QSize pbSize( 26, 26 );

#ifdef FL_DEBUGGER
  if ( !pushButtonIDE ) {
    pushButtonIDE = new QPushButton( this, "pushButtonIDE" );
    connect( pushButtonIDE, SIGNAL( clicked() ), this, SLOT( openIde() ) );
  }
  pushButtonIDE->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                pushButtonIDE->sizePolicy().hasHeightForWidth() ) );
  pushButtonIDE->setMinimumSize( pbSize );
  pushButtonIDE->setMaximumSize( pbSize );
  QPixmap qsa( QPixmap::fromMimeSource( "bug.png" ) );
  pushButtonIDE->setIconSet( qsa );
  pushButtonIDE->setAccel( QKeySequence( Qt::Key_F3 ) );
  QToolTip::add( pushButtonIDE, tr( "Abrir Depurador (F3)" ) );
  QWhatsThis::add( pushButtonIDE, tr( "Abrir Depurador (F3)" ) );
  pushButtonIDE->setFocusPolicy( QWidget::NoFocus );
  layoutButtons->addWidget( pushButtonIDE );
  connect( pushButtonIDE, SIGNAL( clicked() ), this, SLOT( openIde() ) );
#endif

  layoutButtons->addItem( spacer_2 );

  if ( cursor_->modeAccess() == FLSqlCursor::EDIT ||
       cursor_->modeAccess() == FLSqlCursor::BROWSE ) {
    if ( !pushButtonFirst ) {
      pushButtonFirst = new QPushButton( this, "pushButtonFirst" );
      connect( pushButtonFirst, SIGNAL( clicked() ), this, SLOT( firstRecord() ) );
    }
    pushButtonFirst->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                    pushButtonFirst->sizePolicy().hasHeightForWidth() ) );
    pushButtonFirst->setMinimumSize( pbSize );
    pushButtonFirst->setMaximumSize( pbSize );
    QPixmap rld( QPixmap::fromMimeSource( "first.png" ) );
    pushButtonFirst->setIconSet( rld );
    pushButtonFirst->setAccel( QKeySequence( Qt::Key_F5 ) );
    QToolTip::add( pushButtonFirst, tr( "Aceptar los cambios e ir al primer registro (F5)" ) );
    QWhatsThis::add( pushButtonFirst, tr( "Aceptar los cambios e ir al primer registro (F5)" ) );
    pushButtonFirst->setFocusPolicy( QWidget::NoFocus );
    layoutButtons->addWidget( pushButtonFirst );
    pushButtonFirst->show();

    if ( !pushButtonPrevious ) {
      pushButtonPrevious = new QPushButton( this, "pushButtonPrevious" );
      connect( pushButtonPrevious, SIGNAL( clicked() ), this, SLOT( previousRecord() ) );
    }
    pushButtonPrevious->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                       pushButtonPrevious->sizePolicy().hasHeightForWidth() ) );
    pushButtonPrevious->setMinimumSize( pbSize );
    pushButtonPrevious->setMaximumSize( pbSize );
    QPixmap rld2( QPixmap::fromMimeSource( "previous.png" ) );
    pushButtonPrevious->setIconSet( rld2 );
    pushButtonPrevious->setAccel( QKeySequence( Qt::Key_F6 ) );
    QToolTip::add( pushButtonPrevious, tr( "Aceptar los cambios e ir al registro anterior (F6)" ) );
    QWhatsThis::add( pushButtonPrevious, tr( "Aceptar los cambios e ir al registro anterior (F6)" ) );
    pushButtonPrevious->setFocusPolicy( QWidget::NoFocus );
    layoutButtons->addWidget( pushButtonPrevious );
    pushButtonPrevious->show();

    if ( !pushButtonNext ) {
      pushButtonNext = new QPushButton( this, "pushButtonNext" );
      connect( pushButtonNext, SIGNAL( clicked() ), this, SLOT( nextRecord() ) );
    }
    pushButtonNext->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                   pushButtonNext->sizePolicy().hasHeightForWidth() ) );
    pushButtonNext->setMinimumSize( pbSize );
    pushButtonNext->setMaximumSize( pbSize );
    QPixmap rld3( QPixmap::fromMimeSource( "next.png" ) );
    pushButtonNext->setIconSet( rld3 );
    pushButtonNext->setAccel( QKeySequence( Qt::Key_F7 ) );
    QToolTip::add( pushButtonNext, tr( "Aceptar los cambios e ir al registro siguiente (F7)" ) );
    QWhatsThis::add( pushButtonNext, tr( "Aceptar los cambios e ir al registro siguiente (F7)" ) );
    pushButtonNext->setFocusPolicy( QWidget::NoFocus );
    layoutButtons->addWidget( pushButtonNext );
    pushButtonNext->show();

    if ( !pushButtonLast ) {
      pushButtonLast = new QPushButton( this, "pushButtonLast" );
      connect( pushButtonLast, SIGNAL( clicked() ), this, SLOT( lastRecord() ) );
    }
    pushButtonLast->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                   pushButtonLast->sizePolicy().hasHeightForWidth() ) );
    pushButtonLast->setMinimumSize( pbSize );
    pushButtonLast->setMaximumSize( pbSize );
    QPixmap rld4( QPixmap::fromMimeSource( "last.png" ) );
    pushButtonLast->setIconSet( rld4 );
    pushButtonLast->setAccel( QKeySequence( Qt::Key_F8 ) );
    QToolTip::add( pushButtonLast, tr( "Aceptar los cambios e ir al último registro (F8)" ) );
    QWhatsThis::add( pushButtonLast, tr( "Aceptar los cambios e ir al último registro (F8)" ) );
    pushButtonLast->setFocusPolicy( QWidget::NoFocus );
    layoutButtons->addWidget( pushButtonLast );
    pushButtonLast->show();
  }

  if ( cursor_->modeAccess() != FLSqlCursor::BROWSE ) {
    if ( showAcceptContinue_ ) {
      if ( !pushButtonAcceptContinue ) {
        pushButtonAcceptContinue = new QPushButton( this, "pushButtonAcceptContinue" );
        connect( pushButtonAcceptContinue, SIGNAL( clicked() ), this, SLOT( acceptContinue() ) );
      }
      pushButtonAcceptContinue->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
          pushButtonAcceptContinue->sizePolicy().hasHeightForWidth() ) );
      pushButtonAcceptContinue->setMinimumSize( pbSize );
      pushButtonAcceptContinue->setMaximumSize( pbSize );
      QPixmap rld( QPixmap::fromMimeSource( "reload.png" ) );
      pushButtonAcceptContinue->setIconSet( rld );
      pushButtonAcceptContinue->setFocusPolicy( QWidget::NoFocus );
      pushButtonAcceptContinue->setAccel( QKeySequence( Qt::Key_F9 ) );
      QToolTip::add( pushButtonAcceptContinue, tr( "Aceptar los cambios y continuar con la edición de un nuevo registro (F9)" ) );
      QWhatsThis::add( pushButtonAcceptContinue, tr( "Aceptar los cambios y continuar con la edición de un nuevo registro (F9)" ) );
      layoutButtons->addWidget( pushButtonAcceptContinue );
      pushButtonAcceptContinue->show();
    }

    if ( !pushButtonAccept ) {
      pushButtonAccept = new QPushButton( this, "pushButtonAccept" );
      connect( pushButtonAccept, SIGNAL( clicked() ), this, SLOT( accept() ) );
    }
    pushButtonAccept->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                     pushButtonAccept->sizePolicy().hasHeightForWidth() ) );
    pushButtonAccept->setMinimumSize( pbSize );
    pushButtonAccept->setMaximumSize( pbSize );
    QPixmap ok( QPixmap::fromMimeSource( "button_ok.png" ) );
    pushButtonAccept->setIconSet( ok );
    pushButtonAccept->setFocusPolicy( QWidget::NoFocus );
    pushButtonAccept->setAccel( QKeySequence( Qt::Key_F10 ) );
    QToolTip::add( pushButtonAccept, tr( "Aceptar los cambios y cerrar formulario (F10)" ) );
    QWhatsThis::add( pushButtonAccept, tr( "Aceptar los cambios y cerrar formulario (F10)" ) );
    layoutButtons->addWidget( pushButtonAccept );
    pushButtonAccept->show();
  }

  if ( !pushButtonCancel ) {
    pushButtonCancel = new QPushButton( this, "pushButtonCancel" );
    connect( cursor_, SIGNAL( autoCommit() ), this, SLOT( disablePushButtonCancel() ) );
    connect( pushButtonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
  }
  pushButtonCancel->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                   pushButtonCancel->sizePolicy().hasHeightForWidth() ) );
  pushButtonCancel->setMinimumSize( pbSize );
  pushButtonCancel->setMaximumSize( pbSize );
  QPixmap cancel( QPixmap::fromMimeSource( "button_cancel.png" ) );
  pushButtonCancel->setIconSet( cancel );
  if ( cursor_->modeAccess() != FLSqlCursor::BROWSE ) {
    pushButtonCancel->setFocusPolicy( QWidget::NoFocus );
    pushButtonCancel->setAccel( 4096 );
    QToolTip::add( pushButtonCancel, tr( "Cancelar los cambios y cerrar formulario (Esc)" ) );
    QWhatsThis::add( pushButtonCancel, tr( "Cancelar los cambios y cerrar formulario (Esc)" ) );
  } else {
    QPixmap ok( QPixmap::fromMimeSource( "button_cancel.png" ) );
    pushButtonCancel->setIconSet( ok );
    pushButtonCancel->setFocusPolicy( QWidget::StrongFocus );
    pushButtonCancel->setFocus();
    pushButtonCancel->setAccel( 4096 );
    QToolTip::add( pushButtonCancel, tr( "Aceptar y cerrar formulario (Esc)" ) );
    QWhatsThis::add( pushButtonCancel, tr( "Aceptar y cerrar formulario (Esc)" ) );
  }
  pushButtonCancel->setDefault( true );
  layoutButtons->addWidget( pushButtonCancel );
  pushButtonCancel->show();

  mainWidget_ = w;
  mainWidget_->setFocusPolicy( QWidget::NoFocus );
  int mWidth = mainWidget_->width();
  int mHeight = mainWidget_->height();
  QWidget * actWin = qApp->activeWindow();
  QRect screen = ( actWin ? actWin->geometry() : qApp->mainWidget()->geometry() );
  QRect desk = QApplication::desktop()->geometry();
  QPoint p = screen.center() - QPoint( mWidth / 2, mHeight / 2 );
  if ( p.x() + mWidth > desk.width() )
    p.setX( desk.width() - mWidth );
  if ( p.y() + mHeight > desk.height() )
    p.setY( desk.height() - mHeight );
  if ( p.x() < 0 )
    p.setX( 0 );
  if ( p.y() < 0 )
    p.setY( 0 );
  move( p );
}

void FLFormRecordDB::initScript() {
  if ( iface && cursor_ )
    static_cast<FLApplication *>( qApp ) ->call( "init", QSArgumentList(), iface );
}

void FLFormRecordDB::setMainWidget() {
  if ( !action_ || !cursor_ )
    return ;

  if ( !cursor_->metadata() )
    return ;

  setMainWidget( cursor_->db()->managerModules()->createFormRecord( action_, this, this ) );
}

void FLFormRecordDB::setCursor( FLSqlCursor * c ) {
  if ( !c )
    return ;
  cursor_ = c;
  if ( iface )
    iface->setObjCursor( c );
}

void FLFormRecordDB::afterCommitBuffer() {
  if ( iface )
    static_cast<FLApplication *>( qApp ) ->call( "afterCommitBuffer", QSArgumentList(), iface );
}

void FLFormRecordDB::afterCommitTransaction() {
  if ( iface )
    static_cast<FLApplication *>( qApp ) ->call( "afterCommitTransaction", QSArgumentList(), iface );
}

void FLFormRecordDB::acceptedForm() {
  if ( iface )
    static_cast<FLApplication *>( qApp ) ->call( "acceptedForm", QSArgumentList(), iface );
}

void FLFormRecordDB::canceledForm() {
  if ( iface )
    static_cast<FLApplication *>( qApp ) ->call( "canceledForm", QSArgumentList(), iface );
}

bool FLFormRecordDB::validateForm() {
  if ( iface && ( cursor_->modeAccess() == FLSqlCursor::INSERT || cursor_->modeAccess() == FLSqlCursor::EDIT ) ) {
    QVariant v = static_cast<FLApplication *>( qApp ) ->call( "validateForm", QSArgumentList(), iface ).variant();
    if ( v.isValid() && !v.toBool() )
      return false;
  }
  return true;
}

void FLFormRecordDB::accept() {
  if ( accepting ) {
    return ;
  } else
    accepting = true;

  if ( !cursor_ ) {
    close();
    return ;
  }

  if ( !validateForm() ) {
    accepting = false;
    return ;
  }

  if ( cursor_->checkIntegrity() ) {
    acceptedForm();
    cursor_->setActivatedCheckIntegrity( false );
    if ( cursor_->commitBuffer() ) {
      cursor_->setActivatedCheckIntegrity( true );
    } else {
      accepting = false;
      return ;
    }
  } else {
    accepting = false;
    return ;
  }

  afterCommitBuffer();

  accepted = true;
  close();
}

void FLFormRecordDB::acceptContinue() {
  if ( accepting )
    return ;
  else
    accepting = true;

  if ( !cursor_ ) {
    close();
    return ;
  }

  if ( !validateForm() ) {
    accepting = false;
    return ;
  }

  if ( cursor_->checkIntegrity() ) {
    acceptedForm();
    cursor_->setActivatedCheckIntegrity( false );
    if ( cursor_->commitBuffer() ) {
      cursor_->setActivatedCheckIntegrity( true );
      cursor_->commit();
      cursor_->setModeAccess( FLSqlCursor::INSERT );
      accepted = false;
      QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
      cursor_->transaction();
      setCaption( tr( "Insertar " ) + caption );
      if ( initFocusWidget_ )
        initFocusWidget_->setFocus();
      cursor_->refreshBuffer();
      initScript();
    }
  }
  accepting = false;
#ifdef FL_TEST
  static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FORM_RECORD_READY, this->name() );
#endif
}

void FLFormRecordDB::reject() {
  accepted = false;
  canceledForm();
  close();
}

void FLFormRecordDB::closeEvent( QCloseEvent * e ) {
  if ( focusWidget() ) {
    FLFieldDB * fdb = ::qt_cast<FLFieldDB *>( focusWidget()->parentWidget() );
    if ( fdb && fdb->autoComFrame_ && fdb->autoComFrame_->isVisible() ) {
      fdb->autoComFrame_->hide();
      return;
    }
  }

  if ( cursor_ ) {
    int levels = FLSqlCursor::transactionLevel() - initTransLevel;
    if ( levels > 0 )
      cursor_->rollbackOpened( levels,
                               tr( "Se han detectado transacciones no finalizadas en la última operación.\n" ) +
                               tr( "Se van a cancelar las transacciones pendientes.\n" ) );
    if ( accepted ) {
      if ( !cursor_->commit() )
        return ;
      afterCommitTransaction();
    } else {
      if ( !cursor_->rollback() )
        return ;
      else
        cursor_->QSqlCursor::select();
    }
    emit closed();
    if ( oldCursorCtxt )
      cursor_->setContext( oldCursorCtxt );
  } else
    emit closed();
  QWidget::closeEvent( e );
  deleteLater();
}

void FLFormRecordDB::firstRecord() {
  if ( cursor_ && cursor_->at() != 0 ) {
    if ( !validateForm() )
      return ;
    if ( cursor_->checkIntegrity() ) {
      acceptedForm();
      cursor_->setActivatedCheckIntegrity( false );
      if ( cursor_->commitBuffer() ) {
        cursor_->setActivatedCheckIntegrity( true );
        cursor_->commit();
        cursor_->setModeAccess( initialModeAccess );
        accepted = false;
        QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
        cursor_->transaction();
        cursor_->first();
        initScript();
      }
    }
#ifdef FL_TEST
    static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FORM_RECORD_READY, this->name() );
#endif
  }
}

void FLFormRecordDB::nextRecord() {
  if ( cursor_ && cursor_->isValid() ) {
    if ( cursor_->at() == ( cursor_->size() - 1 ) ) {
      firstRecord();
      return ;
    }
    if ( !validateForm() )
      return ;
    if ( cursor_->checkIntegrity() ) {
      acceptedForm();
      cursor_->setActivatedCheckIntegrity( false );
      if ( cursor_->commitBuffer() ) {
        cursor_->setActivatedCheckIntegrity( true );
        cursor_->commit();
        cursor_->setModeAccess( initialModeAccess );
        accepted = false;
        QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
        cursor_->transaction();
        cursor_->next();
        initScript();
      }
    }
#ifdef FL_TEST
    static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FORM_RECORD_READY, this->name() );
#endif
  }
}

void FLFormRecordDB::previousRecord() {
  if ( cursor_ && cursor_->isValid() ) {
    if ( cursor_->at() == 0 ) {
      lastRecord();
      return ;
    }
    if ( !validateForm() )
      return ;
    if ( cursor_->checkIntegrity() ) {
      acceptedForm();
      cursor_->setActivatedCheckIntegrity( false );
      if ( cursor_->commitBuffer() ) {
        cursor_->setActivatedCheckIntegrity( true );
        cursor_->commit();
        cursor_->setModeAccess( initialModeAccess );
        accepted = false;
        QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
        cursor_->transaction();
        cursor_->prev();
        initScript();
      }
    }
#ifdef FL_TEST
    static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FORM_RECORD_READY, this->name() );
#endif
  }
}

void FLFormRecordDB::lastRecord() {
  if ( cursor_ && cursor_->at() != ( cursor_->size() - 1 ) ) {
    if ( !validateForm() )
      return ;
    if ( cursor_->checkIntegrity() ) {
      acceptedForm();
      cursor_->setActivatedCheckIntegrity( false );
      if ( cursor_->commitBuffer() ) {
        cursor_->setActivatedCheckIntegrity( true );
        cursor_->commit();
        cursor_->setModeAccess( initialModeAccess );
        accepted = false;
        QString caption = cursor_->metadata() ->alias() + " [ " + static_cast<FLApplication *>( qApp ) ->lastTextCaption() + " ]";
        cursor_->transaction();
        cursor_->last();
        initScript();
      }
    }
#ifdef FL_TEST
    static_cast<FLApplication *>( qApp ) ->continueTesting( FLTester::FORM_RECORD_READY, this->name() );
#endif
  }
}

void FLFormRecordDB::disablePushButtonCancel() {
  if ( pushButtonCancel )
    pushButtonCancel->setDisabled( true );
}
