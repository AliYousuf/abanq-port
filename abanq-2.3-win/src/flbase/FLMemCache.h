/***************************************************************************
                            FLMemCache.h
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
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef FLMEMCACHE_H
#define FLMEMCACHE_H

#include <qstring.h>
#include <qcache.h>
#include <qobject.h>

class FLMemCache {
public:

  static bool find( const QString &key, QString& );
  static bool insert( const QString &key, const QString& );
  static void clear();
};

#endif
