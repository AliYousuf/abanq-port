/***************************************************************************
                           FLReportEngine.h
                          -------------------
 begin                : jue jun 27 2002
 copyright            : (C) 2002-2005 by InfoSiAL S.L.
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

#ifndef FLREPORTENGINE_H
#define FLREPORTENGINE_H

#define FL_MAXGROUPLEVEL 10

#include <config.h>

#include <qdom.h>
#include <qstringlist.h>
#include <qfile.h>

#include "mreportengine.h"

class FLSqlCursor;
class FLSqlQuery;

/**
Motor de generaci�n de informes. Hereda de MReportEngine.

Extiende la funcionalidad de MReportEngine, para crear el origen
de datos (una estructura XML) de Kugar a partir de una tabla o una consulta.
El origen de una tabla o una consulta son objetos del tipo FLSqlCursor para tablas
y FLSqlQuery para consultas. El formato de los informes se sigue tomando de un
fichero XML seg�n el formato prefijado por Kugar.

@author InfoSiAL S.L.
*/
class FL_EXPORT FLReportEngine: public MReportEngine {

  Q_OBJECT

public:

  /**
  constructor
  */
  FLReportEngine( QObject * parent = 0 );

  /**
  desctructor
  */
  ~FLReportEngine();

  /**
  Establece los datos del informe a partir de una consulta

  @param q Objeto FLSqlQuery con la consulta de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setReportData( FLSqlQuery * q );


  /**
  Establece los datos del informe a partir de un document XML

  @param d Objeto QDomNode con la consulta de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setFLReportData( QDomNode d );

  /**
  Establece los datos del informe a partir de una tabla

  @param t Objeto FLSqlCursor con  la tabla de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setReportData( FLSqlCursor * t );

  /**
  Establece la plantilla para el informe.

  El nombre de la plantilla corresponde con el nombre del fichero con extesi�n ".kut".

  @param t Nombre dela plantilla
  @return TRUE si todo ha ido bien
  */
  bool setFLReportTemplate( const QString & t );

private:

  /**
  A�ade los datos del registro activo de la consulta dada al origen de datos del informe.

  Todos los campos del registro son agregados a la estructura XML con los datos de origen
  del informe, en un elemento tipo "Row", indicando a que nivel pertenece el registro seg�n
  el par�metro l. La consulta debe pasarse en un estado v�lido, es decir, con un registro activo.

  @param q Consulta de la que tomar su registro activo para agregar datos al informe
  @param l Nivel al  que pertenece el registro
  */
  void addRowToReportData( FLSqlQuery * q, const int l );

  /**
  Agrupa los registros de la consulta recursivamente seg�n el nivel de agrupamiento, y los
  va sacando al objeo raiz XML, en el orden adecuado.

  La consulta debe pasarse en un estado v�lido, es decir, con un registro activo
  */
  void groupBy( int levelMax, FLSqlQuery * q, QString vA[ FL_MAXGROUPLEVEL ] );

  /**
  Registros de la consulta en formato de nodos XML para Kugar
  */
  QDomDocumentFragment * rows;

  /**
  Guarda la consulta origen
  */
  FLSqlQuery * qry_;
};

#endif
