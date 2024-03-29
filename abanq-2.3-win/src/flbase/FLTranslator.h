/***************************************************************************
                        FLTranslator.h
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

#ifndef FLTRANSLATOR_H_
#define FLTRANSLATOR_H_

#include <qtranslator.h>
#include <qstring.h>
#include <qfile.h>

class FL_EXPORT FLTranslator : public QTranslator {

public:

  /**
  constructor
  */
  FLTranslator( QObject * parent = 0 );

  /**
  destructor
  */
  virtual ~FLTranslator();

  /**
  Carga en el traductor el contenido de un fichero de traducciones existente en la cach� de disco

  El fichero debe introducirse en la cach� de disco antes de llamar a este m�todo, en
  caso contrario no se har� nada.

  @param key Clave sha1 que identifica al fichero en la cach� de disco
  @return  TRUE si la operaci�n tuvo �xito
  */
  bool loadTsContent( const QString & key );
};

#endif /*FLTRANSLATOR_H_*/
