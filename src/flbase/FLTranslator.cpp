/***************************************************************************
                         FLTranslator.cpp
                     -------------------
begin                : 07/08/2007
copyright            : (C) 2003-2007 by InfoSiAL S.L.
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

#include "FLTranslator.h"
#include "FLDiskCache.h"
#include "FLTranslations.h"

FLTranslator::FLTranslator( QObject * parent ) : QTranslator( parent ) {}

FLTranslator::~FLTranslator() {}

bool FLTranslator::loadTsContent( const QString & key ) {
  QString tsFile = FLDiskCache::absoluteFilePath( key );
  QString qmFile = FLDiskCache::absoluteDirPath() + "/" + key + ".qm";
  FLTranslations trans;

  if ( !QFile::exists( qmFile ) ) {
    if ( tsFile.isEmpty() )
      return false;
    trans.lrelease( tsFile, qmFile );
  }

  return QTranslator::load( qmFile );
}
