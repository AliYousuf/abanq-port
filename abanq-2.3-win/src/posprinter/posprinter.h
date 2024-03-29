/***************************************************************************
                             posprinter.h
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
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/
 
#ifndef POSPRINTER_H
#define POSPRINTER_H

#include <config.h>

#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>
#include <qprocess.h>
#include <qapplication.h>
#include <qmap.h>

#include <private/qtextengine_p.h>
#include <private/qtextlayout_p.h>

class FL_EXPORT FLPosPrinter : public QPaintDevice {

public:

	FLPosPrinter();
	~FLPosPrinter();

	enum PaperWidth { P57_5MM, P69_5MM, P76MM, NPaperWidth = P76MM };

	PaperWidth paperWidth() const {
		return paperWidth_;
	}

	virtual void setPaperWidth( PaperWidth );

	const QString & printerName() const {
		return printerName_;
	}

	void setPrinterName( const QString & pName );

	void cleanup();
	void flush();
	void send( const QString & str, const int col = -1, const int row = -1 );

protected:

	bool cmd( int, QPainter *, QPDevCmdParam * );
	int metric( int ) const;

	int paperWidthToCols();

	void sendStr( const char c, const int col = -1, const int row = -1 );
	void sendEsc( const QString & e, const int col = -1, const int row = -1 );

	void initFile();
	void initStrBuffer();
	void initEscBuffer();

private:

	PaperWidth paperWidth_;
	QFile * file;
	QMap< int, char >   * strBuffer;
	QMap< int, QString > * escBuffer;
	int idxBuffer;
	QString printerName_;
};

#endif
