/***************************************************************************
                 FLManagerModules.cpp  -  description
                         -------------------
begin                : mie dic 24 2003
copyright            : (C) 2003-2004 by InfoSiAL, S.L.
email                : mail@infosial.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "FLManagerModules.h"
#include "FLMemCache.h"
#include "FLDiskCache.h"
#include "FLAction.h"
#include "FLSqlCursor.h"
#include "FLManager.h"
#include "FLApplication.h"
#include "FLUtil.h"
#include "FLSqlDatabase.h"

class FLInfoMod {
public:
  FLInfoMod() {}
  QString idModulo;
  QString idArea;
  QString descripcion;
  QString version;
  QCString icono;
  QString areaDescripcion;
};

FLManagerModules::FLManagerModules( FLSqlDatabase * db ) : db_( db ) {
  activeIdArea_ = QString::null;
  activeIdModule_ = QString::null;
  shaLocal_ = QString::null;
  dictKeyFiles = 0;
  listAllIdModules_ = 0;
  listIdAreas_ = 0;
  dictModFiles = 0;
  dictInfoMods = 0;
}

FLManagerModules::~ FLManagerModules() {
  finish();
}

void FLManagerModules::loadAllIdModules() {
  if ( !db_->dbAux() )
    return ;

  if ( !listAllIdModules_ )
    listAllIdModules_ = new QStringList();
  else
    listAllIdModules_->clear();
  listAllIdModules_->append( "sys" );

  if ( !dictInfoMods ) {
    dictInfoMods = new QDict < FLInfoMod >( 37 );
    dictInfoMods->setAutoDelete( true );
  } else
    dictInfoMods->clear();

  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT idmodulo,flmodules.idarea,flmodules.descripcion,version,icono,flareas.descripcion FROM flmodules left join flareas on flmodules.idarea = flareas.idarea" );
  while ( q.next() ) {
    FLInfoMod * infoMod = new FLInfoMod();
    infoMod->idModulo = q.value( 0 ).toString();
    infoMod->idArea = q.value( 1 ).toString();
    infoMod->descripcion = q.value( 2 ).toString();
    infoMod->version = q.value( 3 ).toString();
    infoMod->icono = q.value( 4 ).toCString();
    infoMod->areaDescripcion = q.value( 5 ).toString();
    dictInfoMods->replace( infoMod->idModulo.upper(), infoMod );
    if ( infoMod->idModulo != "sys" )
      listAllIdModules_->append( infoMod->idModulo );
  }
}

void FLManagerModules::loadIdAreas() {
  if ( !db_->dbAux() )
    return ;

  if ( !listIdAreas_ )
    listIdAreas_ = new QStringList();
  else
    listIdAreas_->clear();

  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT idarea FROM flareas WHERE idarea <> 'sys'" );
  while ( q.next() )
    listIdAreas_->append( q.value( 0 ).toString() );
  listIdAreas_->append( "sys" );
}

void FLManagerModules::loadKeyFiles() {
  if ( !dictKeyFiles ) {
    dictKeyFiles = new QDict < QString >( 277 );
    dictKeyFiles->setAutoDelete( true );
  } else
    dictKeyFiles->clear();

  if ( !dictModFiles ) {
    dictModFiles = new QDict < QString >( 277 );
    dictModFiles->setAutoDelete( true );
  } else
    dictModFiles->clear();

  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT nombre,sha,idmodulo FROM flfiles" );
  while ( q.next() ) {
    dictKeyFiles->replace( q.value( 0 ).toString(), new QString( q.value( 1 ).toString() ) );
    dictModFiles->replace( q.value( 0 ).toString().upper(), new QString( q.value( 2 ).toString() ) );
  }
}

void FLManagerModules::init() {
#ifndef FL_QUICK_CLIENT
  if ( !db_->dbAux() )
    return ;

  if ( db_->connectionName() != "default" )
    return ;

  FLTableMetaData *tmpTMD;

  tmpTMD = db_->manager()->createSystemTable( "flfiles" );
  tmpTMD = db_->manager()->createSystemTable( "flsettings" );
  tmpTMD = db_->manager()->createSystemTable( "flserial" );
  tmpTMD = db_->manager()->createSystemTable( "flvar" );

  FLSqlCursor curSet( "flsettings", true, db_->dbAux() );
  curSet.select( "flkey = 'sysmodver'" );
  if ( !curSet.first() || curSet.valueBuffer( "valor" ).toString() != QString( VERSION ) ) {
    FLDiskCache::clear();
    QSqlQuery qry( QString::null, db_->dbAux() );
    qry.exec( "DROP TABLE flserial" );
    qry.exec( "DROP TABLE flvar" );
    if ( curSet.isValid() )
      curSet.setModeAccess( FLSqlCursor::EDIT );
    else
      curSet.setModeAccess( FLSqlCursor::INSERT );
    curSet.refreshBuffer();
    curSet.setValueBuffer( "flkey", "sysmodver" );
    curSet.setValueBuffer( "valor", QString( VERSION ) );
    curSet.commitBuffer();
  }

  tmpTMD = db_->manager()->createSystemTable( "flfiles" );
  tmpTMD = db_->manager()->createSystemTable( "flserial" );
  tmpTMD = db_->manager()->createSystemTable( "flvar" );

  tmpTMD = db_->manager()->createSystemTable( "flareas" );
  if ( tmpTMD ) {
    FLSqlCursor cursor( "flareas", true, db_->dbAux() );
    cursor.setModeAccess( FLSqlCursor::INSERT );
    cursor.refreshBuffer();
    cursor.setValueBuffer( "idarea", "sys" );
    cursor.setValueBuffer( "descripcion", QApplication::tr( "Sistema" ) );
    cursor.setValueBuffer( "bloqueo", QVariant( false, 0 ) );
    cursor.commitBuffer();
  }

  tmpTMD = db_->manager()->createSystemTable( "flmodules" );
  if ( tmpTMD ) {
    FLSqlCursor cursor( "flmodules", true, db_->dbAux() );
    cursor.setModeAccess( FLSqlCursor::INSERT );
    cursor.refreshBuffer();
    cursor.setValueBuffer( "idmodulo", "sys" );
    cursor.setValueBuffer( "idarea", "sys" );
    cursor.setValueBuffer( "descripcion", QApplication::tr( "Administración" ) );
    cursor.setValueBuffer( "icono", contentFS( FLDATA "/sys.xpm" ) );
    cursor.setValueBuffer( "bloqueo", QVariant( false, 0 ) );
    cursor.commitBuffer();
  }
#endif

  readState();
}

void FLManagerModules::finish() {
  writeState();

  if ( dictKeyFiles ) {
    delete dictKeyFiles;
    dictKeyFiles = 0;
  }

  if ( listAllIdModules_ ) {
    delete listAllIdModules_;
    listAllIdModules_ = 0;
  }

  if ( listIdAreas_ ) {
    delete listIdAreas_;
    listIdAreas_ = 0;
  }

  if ( dictModFiles ) {
    delete dictModFiles;
    dictModFiles = 0;
  }

  if ( dictInfoMods ) {
    delete dictInfoMods;
    dictInfoMods = 0;
  }

  FLMemCache::clear();
}

QString FLManagerModules::contentStaticDir( const QString & n ) {
  QString str_ret( contentFS( staticDirFiles_ + "/" + n ) );

  if ( !str_ret.isEmpty() ) {

    QString sha( FLUtil::sha1( str_ret ) );
    QString * s = 0;
    if ( dictKeyFiles && ( s = ( *dictKeyFiles )[ n ] ) && *s == sha )
      return QString::null;
    else if ( dictKeyFiles && n.endsWith( ".qs" ) )
      dictKeyFiles->replace( n, new QString( sha ) );

    if ( n.endsWith( ".mtd" ) ) {
      FLTableMetaData * mtd;
      QDomDocument doc( n );
      QDomElement docElem;

      if ( FLUtil::domDocumentSetContent( doc, str_ret ) ) {
        FLManager * mgr = db_->manager();
        docElem = doc.documentElement();
        mtd = mgr->metadata( &docElem, true );

        if ( !mtd || mtd->isQuery() )
          return str_ret;

        if ( !mgr->existsTable( mtd->name() ) )
          mgr->createTable( mtd );
        else if ( db_->canRegenTables() )
          db_->regenTable( mtd->name(), mtd );
      }
    }
  }

  return str_ret;
}

QString FLManagerModules::content( const QString & n ) {
  if ( n.isEmpty() || n.length() <= 3 )
    return QString::null;

  QString retFS;
  bool notSysTable = db_->dbAux() && n.left( 3 ) != "sys" && !db_->manager()->isSystemTable( n );

  if ( !staticDirFiles_.isEmpty() && notSysTable ) {
    retFS = contentStaticDir( n );
    if ( !retFS.isEmpty() )
      return retFS;
  }

  if ( n.endsWith( ".xml" ) )
    retFS = contentFS( FLDATA "/" + n );
  else if ( n.endsWith( ".qs" ) )
    retFS = contentFS( FLDATA "/scripts/" + n );
  else if ( n.endsWith( ".mtd" ) )
    retFS = contentFS( FLDATA "/tables/" + n );
  else if ( n.endsWith( ".ui" ) )
    retFS = contentFS( FLDATA "/forms/" + n );
  else if ( n.endsWith( ".kut" ) )
    retFS = contentFS( FLDATA "/reports/" + n );
  else if ( n.endsWith( ".qry" ) )
    retFS = contentFS( FLDATA "/queries/" + n );
  else if ( n.endsWith( ".ts" ) )
    retFS = contentFS( FLDATA "/translations/" + n );

  if ( !retFS.isEmpty() )
    return retFS;

  if ( notSysTable ) {
    QSqlQuery q( QString::null, db_->dbAux() );
    q.setForwardOnly( true );
    q.exec( "SELECT contenido,sha FROM flfiles WHERE upper(nombre) = '" + n.upper() + "'" );
    if ( q.next() ) {
      QString ret = q.value( 0 ).toString();
      if ( q.value( 1 ).toString().isEmpty() ) {
        FLSqlCursor cursor( "flfiles", true, db_->dbAux() );
        cursor.select( "upper(nombre) = '" + n.upper() + "'" );

        if ( cursor.lastError().type() != QSqlError::None ) {
          QMessageBox::critical( 0, "Error",
                                 QString( cursor.lastError().driverText().utf8() ) + "\n" +
                                 QString( cursor.lastError().databaseText().utf8() ), QMessageBox::Ok, 0, 0 );
          return QString::null;
        }

        cursor.setModeAccess( FLSqlCursor::EDIT );
        cursor.first();
        QString s = FLUtil::sha1( ret );
        cursor.setValueBuffer( "sha", s );
        cursor.commitBuffer();
      }
      return ret;
    }
  }

  return QString::null;
}

QString FLManagerModules::contentFS( const QString & pN ) {
  QFile fi( pN );
  if ( !fi.open( IO_ReadOnly ) )
    return QString::null;
  QTextStream t( &fi );
  t.setEncoding( QTextStream::Latin1 );
  QString str_ret( t.read() );
  fi.close();
  return str_ret;
}

void FLManagerModules::setContent( const QString & n, const QString & idM, const QString & content ) {
  if ( !db_->dbAux() )
    return ;

  FLSqlCursor cursor( "flfiles", true, db_->dbAux() );

  cursor.select( "upper(nombre) = '" + n.upper() + "' AND upper(idmodulo) = '" + idM.upper() + "'" );

  if ( cursor.first() ) {
    cursor.setModeAccess( FLSqlCursor::EDIT );
    cursor.refreshBuffer();
  } else {
    cursor.setModeAccess( FLSqlCursor::INSERT );
    cursor.refreshBuffer();
    cursor.setValueBuffer( "nombre", n );
    cursor.setValueBuffer( "idmodulo", idM );
  }

  cursor.setValueBuffer( "contenido", content );
  cursor.setValueBuffer( "sha", FLUtil::sha1( content ) );
  cursor.commitBuffer();
}

QString FLManagerModules::shaOfFile( const QString & n ) {
  if ( db_->dbAux() && n.left( 3 ) != "sys" && !db_->manager()->isSystemTable( n ) ) {
    QSqlQuery q( QString::null, db_->dbAux() );
    q.setForwardOnly( true );
    q.exec( "SELECT sha FROM flfiles WHERE upper(nombre) = '" + n.upper() + "'" );
    if ( q.next() )
      return q.value( 0 ).toString();
    return QString::null;
  } else
    return QString::null;
}

QString FLManagerModules::contentCached( const QString & n, QString * shaKey ) {
  if ( n.isEmpty() || n.length() <= 3 )
    return QString::null;

  QString str_ret;
  bool notSysTable = db_->dbAux() && n.left( 3 ) != "sys" && !db_->manager()->isSystemTable( n );

  if ( !staticDirFiles_.isEmpty() && notSysTable ) {
    str_ret = contentStaticDir( n );
    if ( !str_ret.isEmpty() )
      return str_ret;
  }

  QString key;

  if ( notSysTable ) {
    QString * s = 0;
    if ( dictKeyFiles )
      s = ( *dictKeyFiles )[ n ];
    if ( s ) {
      key = *s;
      if ( shaKey )
        * shaKey = key;
    } else {
      key = shaOfFile( n );
      if ( shaKey )
        * shaKey = key;
    }
  } else
    return content( n );

  if ( key.isEmpty() )
    return content( n );

  if ( !FLMemCache::find( key, str_ret ) ) {
    if ( !FLDiskCache::find( key, str_ret ) ) {
      str_ret = content( n );
      if ( !str_ret.isEmpty() ) {
        FLMemCache::insert( key, str_ret );
        FLDiskCache::insert( key, str_ret );
      }
    } else
      FLMemCache::insert( key, str_ret );
  }

  return str_ret;
}

QString FLManagerModules::getScript( const QString & n, QString * shaKey, bool optimize ) {
  if ( n == "sys.qs" || n == "plus_sys.qs" )
    return contentCached( n, shaKey );

  QString s( contentCached( n, shaKey ) + "\n" );

#ifdef FL_TEST
  int index, index2, index3;
  index = s.find( "connect" );
  QString testFunctions = "", fName;
  for ( index = s.find( "connect" ); index > -1; index = s.find( "connect", index + 1 ) ) {
    index2 = s.find( ";", index );
    if ( index2 == -1 )
      continue;
    QString params = s.mid( index, index2 - index );
    QStringList param = QStringList::split( ",", params );
    if ( param.count() < 4 )
      continue;
    index2 = param[ 3 ].find( "iface." );
    if ( index2 == -1 )
      continue;
    index3 = param[ 3 ].find( QRegExp( "[\"(]" ), index2 );
    if ( index3 == -1 )
      continue;
    fName = param[ 3 ].mid( index2 + 6, index3 - index2 - 6 );
    index2 = s.find( "iface." + fName, index );
    s.replace( index2, 6, "tests_" );
    testFunctions += "function tests_" + fName + "(a,b,c,d,e,f) {\n\t" +
                     "this.iface." + fName + "(a,b,c,d,e,f);\n\tsys.continueTesting(\"" +
                     fName + "\");\n}\n";
  }
  s += "\n" + testFunctions ;
#endif

  QRegExp rx;

  if ( optimize && staticDirFiles_.isEmpty() ) {
#if !defined(FL_DEBUGGER) && !defined(FL_DEBUG)
    rx.setPattern( "\\)\\s*:\\s*(FL\\w+|Object\\w*|String\\w*|Date\\w*|Number\\w*|Boolean\\w*|Array\\w*)" );
    s.replace( rx, ")" );

    rx.setPattern( ":\\s*(FL\\w+|Object\\w*|String\\w*|Date\\w*|Number\\w*|Boolean\\w*|Array\\w*)\\s*\\=" );
    s.replace( rx, "=" );

    int pos = 0, counter, slashs;
    uint i = 0;
    bool onString = false;
    while ( pos != -1 && i < s.length() ) {
      if ( onString ) {
        pos = s.find( '"', i );
        if ( pos == -1 ) {
          onString = false;
          ++i;
          continue;
        }
      } else {
        pos = s.find( QRegExp( "//|/\\*|\"" ), i );
        if ( pos == -1 )
          break;
      }

      counter = pos;
      slashs = 0;
      while ( counter > 0 && s.at( counter - 1 ) == '\\' ) {
        --counter;
        ++slashs;
      }
      if ( slashs % 2 ) {
        ++i;
        continue;
      }

      if ( s.at( pos ) == '"' ) {
        i = pos;
        onString = !onString;
        ++i;
        continue;
      } else {
        i = pos;
        if ( s.at( pos + 1 ) == '/' ) {
          pos = s.find( QRegExp( "[\n\r]" ), pos + 2 );
          if ( pos > 0 )
            s.remove( i, pos - i );
          continue;
        } else if ( s.at( pos + 1 ) == '*' ) {
          pos = s.find( "*/", pos + 2 );
          if ( pos > 0 )
            s.remove( i, pos - i + 2 );
          continue;
        } else
          ++i;
      }
    }

    rx.setPattern( "[\n\r]+" );
    s.replace( rx, "\n" );
#endif
  }

  rx.setMinimal( true );
  rx.setPattern( "class\\s+(\\w+)\\s+extends\\s+\\1([\\s\n]*\\{.*\\}[\\s\n]*\\})" );
  s.replace( rx, "/* ¡¡ ERROR !! : LA CLASE HEREDA DE ELLA MISMA."
             "\nCODIGO INHABILITADO AUTOMÁTICAMENTE POR AbanQ :\n\n"
             "class \\1 extends \\1 \\2\n\n ¡¡ FIN ERROR !! */" );
  return s;
}

QWidget * FLManagerModules::createUI( const QString & n, QObject * connector,
                                      QWidget * parent, const char *name ) {
  QBuffer b;
  b.open( IO_ReadWrite );
  QTextStream t( &b );
  t.setEncoding( QTextStream::Latin1 );
  t << contentCached( n );
  b.reset();
  QWidget * w = QWidgetFactory::create( &b, connector, parent, name );
  return w;
}

QWidget * FLManagerModules::createForm( FLAction * a, QObject * connector,
                                        QWidget * parent, const char *name ) {
  return createUI( a->form(), connector, parent, name );
}

QWidget * FLManagerModules::createFormRecord( FLAction * a, QObject * connector,
    QWidget * parent, const char *name ) {
  return createUI( a->formRecord(), connector, parent, name );
}

void FLManagerModules::setActiveIdModule( const QString & id ) {
#if defined (FL_QUICK_CLIENT)
  if ( id == "sys" ) {
    activeIdModule_ = QString::null;
    activeIdArea_ = QString::null;
    return;
  }
#endif

  if ( id.isEmpty() || !dictInfoMods ) {
    activeIdArea_ = QString::null;
    activeIdModule_ = QString::null;
    return ;
  }

  FLInfoMod *iM = ( *dictInfoMods )[ id.upper()];
  if ( iM ) {
    activeIdArea_ = iM->idArea;
    activeIdModule_ = id;
  } else {
#ifdef FL_DEBUG
    qWarning( QApplication::tr( "FLManagerModules : Se ha intentando activar un módulo inexistente" ) );
#endif
    activeIdArea_ = QString::null;
    activeIdModule_ = QString::null;
  }
}

void FLManagerModules::readState() {
  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );
  QString idDB = "noDB";
  if ( db_->dbAux() )
    idDB = db_->dbAux() ->databaseName() + db_->dbAux() ->hostName() + db_->dbAux() ->userName() +
           db_->dbAux() ->driverName() + QString::number( db_->dbAux() ->port() );

  activeIdModule_ = config.readEntry( keybase + "Modules/activeIdModule/" + idDB, QString::null );
  activeIdArea_ = config.readEntry( keybase + "Modules/activeIdArea/" + idDB, QString::null );
  shaLocal_ = config.readEntry( keybase + "Modules/shaLocal/" + idDB, QString::null );
  staticDirFiles_ = config.readEntry( keybase + "Modules/staticDirFiles", QString::null );

#if defined (FL_QUICK_CLIENT)
  if ( activeIdModule_ == "sys" ) {
    activeIdModule_ = QString::null;
    activeIdArea_ = QString::null;
  }
#endif

  if ( activeIdModule_.isEmpty() || !listAllIdModules().contains( activeIdModule_ ) )
    setActiveIdModule( QString::null );
}

void FLManagerModules::writeState() {
  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );
  QString idDB = "noDB";
  if ( db_->dbAux() )
    idDB = db_->database() + db_->host() + db_->user() + db_->driverName() + QString::number( db_->port() );

  config.writeEntry( keybase + "Modules/activeIdModule/" + idDB, activeIdModule_ );
  config.writeEntry( keybase + "Modules/activeIdArea/" + idDB, activeIdArea_ );
  config.writeEntry( keybase + "Modules/shaLocal/" + idDB, shaLocal_ );
}

QStringList FLManagerModules::listIdAreas() {
  if ( listIdAreas_ )
    return * listIdAreas_;

  QStringList ret;
  if ( !db_->dbAux() )
    return ret;

  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT idarea FROM flareas WHERE idarea <> 'sys'" );
  while ( q.next() )
    ret << q.value( 0 ).toString();
  ret << "sys";

  return ret;
}

QStringList FLManagerModules::listIdModules( const QString & idA ) {
  QStringList ret;
  if ( dictInfoMods ) {
    QDictIterator < FLInfoMod > it( *dictInfoMods );
    while ( it.current() ) {
      if ( it.current() ->idArea.upper() == idA.upper() )
        ret << it.current() ->idModulo;
      ++it;
    }
  }
  return ret;
}

QStringList FLManagerModules::listAllIdModules() {
  if ( listAllIdModules_ )
    return * listAllIdModules_;

  QStringList ret;
  if ( !db_->dbAux() )
    return ret;

  ret << "sys";
  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT idmodulo FROM flmodules WHERE idmodulo <> 'sys'" );
  while ( q.next() )
    ret << q.value( 0 ).toString();

  return ret;
}

QString FLManagerModules::idAreaToDescription( const QString & idA ) {
  if ( dictInfoMods ) {
    QDictIterator < FLInfoMod > it( *dictInfoMods );
    while ( it.current() ) {
      if ( it.current() ->idArea.upper() == idA.upper() )
        return it.current() ->areaDescripcion;
      ++it;
    }
  }
  return idA;
}

QString FLManagerModules::idModuleToDescription( const QString & idM ) {
  if ( !dictInfoMods )
    return idM;
  FLInfoMod * iM = ( *dictInfoMods )[ idM.upper()];
  if ( iM )
    return iM->descripcion;
  else
    return idM;
}

QPixmap FLManagerModules::iconModule( const QString & idM ) {
  QPixmap pix;
  if ( dictInfoMods ) {
    FLInfoMod *iM = ( *dictInfoMods )[ idM.upper()];
    if ( iM ) {
      QCString cs = iM->icono;
      if ( !cs.isEmpty() ) {
        if ( !QPixmapCache::find( cs.left( 100 ), pix ) ) {
          pix.loadFromData( cs );
          QPixmapCache::insert( cs.left( 100 ), pix );
        }
      }
    }
  }
  return pix;
}

QString FLManagerModules::versionModule( const QString & idM ) {
  if ( !dictInfoMods )
    return idM;
  FLInfoMod * iM = ( *dictInfoMods )[ idM.upper()];
  if ( iM )
    return iM->version;
  else
    return idM;
}

QString FLManagerModules::shaGlobal() {
#ifdef FL_QUICK_CLIENT
  return "quick_client";
#else

  if ( !db_->dbAux() )
    return QString::null;

  QSqlQuery q( QString::null, db_->dbAux() );
  q.setForwardOnly( true );
  q.exec( "SELECT sha FROM flserial" );
  if ( q.lastError().type() != QSqlError::None ) {
    QMessageBox::critical( 0, "Error",
                           QString( q.lastError().driverText().utf8() ) + "\n" +
                           QString( q.lastError().databaseText().utf8() ), QMessageBox::Ok, 0, 0 );
    return QString::null;
  }
  if ( q.next() )
    return q.value( 0 ).toString();
  else
    return QString::null;
#endif
}

QString & FLManagerModules::activeIdArea() {
  return FLManagerModules::activeIdArea_;
}

QString & FLManagerModules::activeIdModule() {
  return FLManagerModules::activeIdModule_;
}

QString FLManagerModules::shaLocal() {
#ifdef FL_QUICK_CLIENT
  return "quick_client";
#else

  return FLManagerModules::shaLocal_;
#endif
}

void FLManagerModules::setShaLocalFromGlobal() {
  shaLocal_ = shaGlobal();
}

QString FLManagerModules::idModuleOfFile( const QString & n ) {
  if ( !dictModFiles )
    return QString::null;
  QString * ret = ( *dictModFiles )[ n.upper()];
  if ( !ret )
    return QString::null;
  return *ret;
}
