/***************************************************************************
                           FLDiskCache.cpp
                         -------------------
begin                : lun ago 02 2004
copyright            : (C) 2004-2005 by InfoSiAL, S.L.
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

#include "FLDiskCache.h"

bool FLDiskCache::find( const QString & key, QString & str ) {
  QString fileCache( absoluteDirPath() + "/" + key );
  QFile fi( fileCache );
  if ( !fi.open( IO_ReadOnly ) )
    return false;
  QTextStream t( &fi );
  str = t.read();
  fi.close();
  return true;
}

bool FLDiskCache::insert( const QString & key, const QString & str ) {
  QString dirCache( absoluteDirPath() );
  QString fileCache( dirCache + "/" + key );
  QFile fi( fileCache );
  QDir d( dirCache );
  if ( !d.exists() )
    d.mkdir( dirCache );
  else
    if ( fi.exists() )
      return true;
  if ( !str.isEmpty() ) {
    if ( fi.open( IO_WriteOnly ) ) {
      QTextStream t( &fi );
      t << str;
      fi.close();
      return true;
    }
    return false;
  }
  return false;
}

void FLDiskCache::clear() {
  QString dirCache( absoluteDirPath() );
  QDir d( dirCache );
  if ( d.exists() ) {
    QStringList lst = d.entryList( "*; *.*" );
    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
      d.remove( dirCache + "/" + ( *it ) );
  }
}

QString FLDiskCache::absoluteFilePath( const QString & key ) {
  QString fileCache( absoluteDirPath() + "/" + key );
  if ( !QFile::exists( fileCache ) )
    return QString::null;
  return fileCache;
}

QString FLDiskCache::absoluteDirPath() {
  QString dirCache( QString( FL_HOME ) + "/.flxcache" );
  return dirCache;
}
