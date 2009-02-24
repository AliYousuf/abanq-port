/***************************************************************************
                             FLDiskCache.h
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
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef FLDISKCACHE_H
#define FLDISKCACHE_H

#include <config.h>

#include <qstring.h>
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>

class FLDiskCache {

public:

  /**
  Busca en cach� seg�n la clave indicada.

  Si hay acierto en el segundo par�metro se devuelve por referencia el contenido

  @param key Clave de b�squeda
  @param str De salida. El contenido encontrado.
  @return TRUE si hubo acierto
  */
  static bool find( const QString & key, QString & str );

  /**
  Inserta en cach� un contenido asign�ndole una clave

  @param key Clave asignada
  @param str Contenido
  @return TRUE si la inserci�n tubo �xito, FALSE en caso contrario
  */
  static bool insert( const QString & key, const QString & str );

  /**
  Limpia completamente todo el contenido de la cach�
  */
  static void clear();

  /**
  Obtiene la ruta absoluta del fichero que almacena el contenido referenciado por la clave indicada

  @param key Clave que referencia el contenido
  @return Ruta absoluta del fichero. Si no existe un fichero asociado a la clave indicada devuelve una cadena vac�a
  */
  static QString absoluteFilePath( const QString & key );

  /**
  @return El directorio principal utilizado por la cach� para almacenar los archivos
  */
  static QString absoluteDirPath();
};

#endif
