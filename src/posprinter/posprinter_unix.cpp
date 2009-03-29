/***************************************************************************
                         posprinter_unix.cpp
                         -------------------
begin                : Vie Mar 10 2006
copyright            : (C) 2006 by InfoSiAL S.L.
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

#include "posprinter.h"

void FLPosPrinter::flush() {
	if ( !file )
		initFile();

	QString fileName = QString( FL_HOME ) + "/.flxcache/outposprinter.tmp";

	for( int i = 0; i < idxBuffer; i++ ) {
		if ( escBuffer && escBuffer->contains( i ) ) {
			const QString & esc = (*escBuffer)[i];
			for( int j = 0; j < esc.length(); j++ )
				file->putch( esc[j] );
		}

		if ( strBuffer && strBuffer->contains( i ) )
			file->putch( (*strBuffer)[i] );
	}

	file->flush();
	file->close();

	QProcess * proc = new QProcess();

	if ( printerName_.isNull() )
		printerName_ = "fluxtpv";
	proc->addArgument( "/usr/bin/lpr" );
	proc->addArgument( "-P" );
	proc->addArgument( printerName_ );
	proc->addArgument( fileName );

	if ( !proc->start() )
		qWarning( "FLPosPrinter::flush() : Error escribiendo en impresora /usr/bin/lpr -P " + printerName_ );

	while ( proc->isRunning() )
		qApp->processEvents();

	qApp->processEvents();

	if ( proc )
		delete proc;
}
