/**********************************************************************
** Copyright (C) 2000-2005 Trolltech AS.  All rights reserved.
**
** This file is part of Qt Linguist.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "trwindow.h"

#include <qapplication.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qsettings.h>
#include <qsplashscreen.h>

int main( int argc, char **argv )
{
	QApplication::setColorSpec( QApplication::ManyColor );
	QApplication::setDesktopSettingsAware( false );
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "ISO8859-15" ) );
    QApplication app( argc, argv );
    QApplication::setOverrideCursor( Qt::waitCursor );

	QSettings settings;
	settings.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
	QFont appFont;
	QString keybase( "/facturalux/lite/" );

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
	pointSize = 11;
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

	app.setFont( appFont );

#if defined(Q_OS_WIN32)
	app.setStyle( settings.readEntry( keybase + "estilo", "Bluecurve" ) );
#else
	app.setStyle( settings.readEntry( keybase + "estilo", "Plastik" ) );
#endif

    QTranslator translator( 0 );
    translator.load( QString( "linguist_" ) + QTextCodec::locale(), "." );
    app.installTranslator( &translator );

    bool showSplash = TRUE;

/* obsolate code 
    QString keybase("/Qt Linguist/3.1/");
    QSettings config;
    config.insertSearchPath( QSettings::Windows, "/Trolltech" );

    QRect r( QApplication::desktop()->screenGeometry() );
    r.setX( config.readNumEntry( keybase + "Geometry/MainwindowX", r.x() ) );
    r.setY( config.readNumEntry( keybase + "Geometry/MainwindowY", r.y() ) );
    r.setWidth( config.readNumEntry( keybase + "Geometry/MainwindowWidth", r.width() ) );
    r.setHeight( config.readNumEntry( keybase + "Geometry/MainwindowHeight", r.height() ) );
*/
    QSplashScreen *splash = 0;
    if ( showSplash ) {
	splash = new QSplashScreen( QPixmap::fromMimeSource("splash.png"),
				    Qt::WDestructiveClose );
	splash->show();
    }

    TrWindow *tw = new TrWindow;
    app.setMainWidget( tw );

    if ( app.argc() > 1 )
	tw->openFile( QString(app.argv()[app.argc() - 1]) );

		//<-- this is duplicated from trwindow.cpp and should be moved to there 
    keybase = "/Qt Linguist/" +
		     QString::number( (QT_VERSION >> 16) & 0xff ) +
		     "." + QString::number( (QT_VERSION >> 8) & 0xff ) + "/";

	QSettings config;
    config.insertSearchPath( QSettings::Windows, "/Trolltech" );
		//-->
    if ( config.readBoolEntry( keybase + "Geometry/MainwindowMaximized", FALSE ) )
	tw->showMaximized();
    else
	tw->show();
    if ( splash )
	splash->finish( tw );
    QApplication::restoreOverrideCursor();

    return app.exec();
}
