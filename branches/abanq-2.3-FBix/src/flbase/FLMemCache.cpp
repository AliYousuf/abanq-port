/***************************************************************************
                           FLMemCache.cpp
                         -------------------
begin                : vie oct 10 2003
copyright            : (C) 2003-2005 by InfoSiAL, S.L.
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

#include "FLMemCache.h"

static QCache<QString> * str_cache = 0;

bool FLMemCache::find( const QString & key, QString & str ) {
  if ( !str_cache ) {
    str_cache = new QCache<QString>( 50, 101 );
    str_cache->setAutoDelete( true );
  }
  QString * s = str_cache->find( key );
  if ( s )
    str = *s;
  return !!s;
}

bool FLMemCache::insert( const QString & key, const QString & str ) {
  if ( !str_cache ) {
    str_cache = new QCache<QString>( 50, 101 );
    str_cache->setAutoDelete( true );
  }
  QString *s = new QString( str );
  bool rt = str_cache->insert( key, s );
  if ( !rt )
    delete s;

  return rt;
}

void FLMemCache::clear() {
  if ( !str_cache || str_cache->isEmpty() )
    return;
#ifdef FL_DEBUG
  qWarning( "*************************************************" );
  qWarning( "FLMemCache" );
  qWarning( "*************************************************" );
  str_cache->statistics();
  qWarning( "*************************************************" );
  qWarning( "FLMemCache" );
  qWarning( "*************************************************" );
#endif
  str_cache->clear();
}
