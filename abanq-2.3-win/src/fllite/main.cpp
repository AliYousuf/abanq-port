/***************************************************************************
                                 main.cpp
                           -------------------
  begin                : lun mar 19 13:44:14 CET 2001
  copyright            : (C) 2001-2005 by InfoSiAL, S.L.
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
extern "C" {
#include <stdlib.h>
}

#include <config.h>

#include <qfont.h>
#include <qtextcodec.h>
#include <qsettings.h>
#include <qsplashscreen.h>
#include <qpaintdevicemetrics.h>
#include <qfile.h>
#include <qtextstream.h>

#include "FLApplication.h"
#include "FLConnectDBDialog.h"
#include "FLTranslator.h"
#include "FLDiskCache.h"
#ifdef FL_TEST
#include "FLTester.h"
#endif


int main( int argc, char **argv ) {
#ifdef FL_TEST
  FLEventLoop e;
#endif

  QPalette p( QColor( 222, 222, 205 ), QColor( 222, 222, 205 ) );
  p.setColor( QPalette::Active, QColorGroup::Dark, QColor( 246, 242, 246 ) );
  p.setColor( QPalette::Inactive, QColorGroup::Dark, QColor( 246, 242, 246 ) );
  p.setColor( QPalette::Disabled, QColorGroup::Dark, QColor( 246, 242, 246 ) );
  p.setColor( QPalette::Active, QColorGroup::Base, QColor( 255, 255, 238 ) );
  p.setColor( QPalette::Inactive, QColorGroup::Base, QColor( 255, 255, 238 ) );
  p.setColor( QPalette::Disabled, QColorGroup::Base, QColor( 255, 255, 238 ) );
  p.setColor( QPalette::Active, QColorGroup::Highlight, QColor( 24, 60, 139 ) );
  p.setColor( QPalette::Active, QColorGroup::HighlightedText, Qt::white );
  p.setColor( QPalette::Inactive, QColorGroup::Highlight, QColor( 24, 60, 139 ) );
  p.setColor( QPalette::Inactive, QColorGroup::HighlightedText, Qt::white );
  p.setColor( QPalette::Disabled, QColorGroup::Highlight, QColor( 24, 60, 139 ) );
  p.setColor( QPalette::Disabled, QColorGroup::HighlightedText, Qt::white );
  p.setColor( QPalette::Inactive, QColorGroup::Text, Qt::black );
  p.setColor( QPalette::Disabled, QColorGroup::Text, QColor( 100, 100, 100 ) );
  p.setColor( QPalette::Active, QColorGroup::Foreground, QColor( 30, 30, 30 ) );
  p.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor( 30, 30, 30 ) );
  p.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 100, 100, 100 ) );
  p.setColor( QPalette::Active, QColorGroup::ButtonText, QColor( 30, 30, 30 ) );
  p.setColor( QPalette::Inactive, QColorGroup::ButtonText, QColor( 30, 30, 30 ) );
  p.setColor( QPalette::Disabled, QColorGroup::ButtonText, QColor( 100, 100, 100 ) );

  QString formAlone, callFunction, arguments, strConn;
  bool quitAfterCall = false, autoLogin_ = false, noMax = false;

  for ( int i = 1; i < argc; i++ ) {
    if ( qstrcmp( argv[ i ], "-f" ) == 0 ) {
      if ( i != argc - 1 ) {
        i++;
        formAlone = QString::fromLatin1( argv[ i ] );
      }
    } else if ( qstrcmp( argv[ i ], "-c" ) == 0 ) {
      if ( i != argc - 1 ) {
        i++;
        callFunction = QString::fromLatin1( argv[ i ] );
      }
    } else if ( qstrcmp( argv[ i ], "-a" ) == 0 ) {
      if ( i != argc - 1 ) {
        i++;
        arguments = QString::fromLatin1( argv[ i ] );
      }
    } else if ( qstrcmp( argv[ i ], "-sc" ) == 0 ) {
      if ( i != argc - 1 ) {
        i++;
        strConn = QString::fromLatin1( argv[ i ] );
      }
    } else if ( qstrcmp( argv[ i ], "-q" ) == 0 ) {
      quitAfterCall = true;
    } else if ( qstrcmp( argv[ i ], "-auto" ) == 0 ) {
      autoLogin_ = true;
    } else if ( qstrcmp( argv[ i ], "-nomax" ) == 0 ) {
      noMax = true;
    }
  }

  FLApplication FacturaLUX( argc, argv );
  QSettings settings;
  settings.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QFont appFont;
  QString keybase( "/facturalux/lite/" );

  QStringList paths( FacturaLUX.libraryPaths() );
  QString pathApp( QString( FLPREFIX ) + "/plugins" );
  if ( !paths.contains( pathApp ) )
    paths << pathApp;
  FacturaLUX.setLibraryPaths( paths );

#if defined (Q_OS_LINUX)
  QPaintDeviceMetrics pdm( QApplication::desktop() );
  float relDpi;
  if ( pdm.logicalDpiX() < pdm.logicalDpiY() )
    relDpi = 78. / pdm.logicalDpiY();
  else
    relDpi = 78. / pdm.logicalDpiX();
  int pointSize = 10 * relDpi;
#else
  int pointSize = 10;
#endif

#if defined(Q_OS_WIN32)
  appFont.setFamily( settings.readEntry( keybase + "fuente/familia", "Tahoma" ) );
  pointSize = 8;
#else

#if defined(Q_OS_MACX)
  appFont.setFamily( settings.readEntry( keybase + "fuente/familia", "Lucida Grande" ) );
  pointSize = 10;
#else
  appFont.setFamily( settings.readEntry( keybase + "fuente/familia", "Sans" ) );
#endif

#endif

  appFont.setPointSize( settings.readNumEntry( keybase + "fuente/puntos", pointSize ) );
  appFont.setBold( settings.readBoolEntry( keybase + "fuente/negrita", false ) );
  appFont.setItalic( settings.readBoolEntry( keybase + "fuente/cursiva", false ) );
  appFont.setUnderline( settings.readBoolEntry( keybase + "fuente/subrayado", false ) );
  appFont.setStrikeOut( settings.readBoolEntry( keybase + "fuente/tachado", false ) );

  FacturaLUX.setFont( appFont );

#if defined(Q_OS_WIN32)
  FacturaLUX.setStyle( settings.readEntry( keybase + "estilo", "Windows" ) );
#else
  FacturaLUX.setStyle( settings.readEntry( keybase + "estilo", "Plastik" ) );
#endif

  QString Language( QTextCodec::locale() );
  QString key = "sys" + Language.left( 2 ) + "ts";
  QString fileTs = "sys." + Language.left( 2 ) + ".ts";
  QString contentTs;

  if ( FLDiskCache::absoluteFilePath( key ).isEmpty() ) {
    QString pN = QString( FLDATA ) + "/translations/" + fileTs;
    QFile fi( pN );
    if ( fi.open( IO_ReadOnly ) ) {
      QTextStream t( &fi );
      contentTs = t.read();
      fi.close();
      FLDiskCache::insert( key, contentTs );
    }
  }

  FLTranslator * tor = new FLTranslator( 0 );
  tor->loadTsContent( key );
  FacturaLUX.installTranslator( tor );
  FacturaLUX.setPalette( p, true );

  bool autoLogin = settings.readBoolEntry( keybase + "application/autoLogin", autoLogin_ );
  FLConnectDBDialog *s = new FLConnectDBDialog( autoLogin, 0, "FLConnectDBDialog", 0, strConn );
  static_cast<QWidget *>( s->child( "frMore" ) )->hide();
  s->adjustSize();
  s->setPalette( p );
  int ret = -1;
  bool connectAttempts = 0;

  do {
    if ( !s->isShown() ) {
      s->show();
      s->raise();
    }
    if ( autoLogin && !connectAttempts ) {
      QTimer::singleShot( 1000, s, SLOT( tryConnect() ) );
      connectAttempts++;
    } else
      s->exec();
  } while ( s->error() );

  ret = s->result();
  if ( ret != QDialog::Accepted ) {
    FacturaLUX.quit();
    if ( s )
      delete s;
    return 0;
  } else {

#ifdef FL_QUICK_CLIENT
    QSplashScreen *splash = new QSplashScreen( QPixmap::fromMimeSource( "splashabanq.png" ) );
#else
    QSplashScreen *splash = new QSplashScreen( QPixmap::fromMimeSource( "splashabanq.png" ) );
#endif
    FacturaLUX.setNotExit( settings.readBoolEntry( keybase + "application/notExit", false ) );
    FacturaLUX.setPrintProgram( settings.readEntry( keybase + "printing/printProgram", QString::null ) );
    splash->show();
    splash->message( FacturaLUX.tr( "Inicializando..." ), Qt::AlignRight, QColor( 0, 0, 0 ) );
    FacturaLUX.flushX();
    FacturaLUX.syncX();
    FacturaLUX.processEvents();
    FacturaLUX.init( settings.readEntry( keybase + "application/formAlone", formAlone ),
                     settings.readEntry( keybase + "application/callFunction", callFunction ),
                     settings.readEntry( keybase + "application/arguments", arguments ),
                     settings.readBoolEntry( keybase + "application/quitAfterCall", quitAfterCall ),
                     settings.readBoolEntry( keybase + "application/noMax", noMax ) );
    if ( FacturaLUX.mainWidget() ) {
      FacturaLUX.mainWidget()->show();
      FacturaLUX.mainWidget()->raise();
    }
    if ( s )
      delete s;
    delete splash;
    return FacturaLUX.exec();
  }
  exit( 0 );
}
