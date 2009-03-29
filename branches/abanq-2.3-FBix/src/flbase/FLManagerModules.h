/***************************************************************************
                    FLManagerModules.h  -  description
                          -------------------
 begin                : mie dic 24 2003
 copyright            : (C) 2003-2004 by InfoSiAL, S.L.
 email                : mail@infosial.com
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef FLMANAGERMODULES_H
#define FLMANAGERMODULES_H

#include <config.h>

#include <qstring.h>
#include <qfile.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qbuffer.h>
#include <qwidgetfactory.h>
#include <qsqldatabase.h>
#include <qsettings.h>
#include <qpixmap.h>
#include <qpixmapcache.h>
#include <qdict.h>

class FLAction;
class FLInfoMod;
class FLSqlDatabase;

/**
Gestor de módulos.

Esta clase permite realizar las funciones básicas de manejo de ficheros
de texto que forman parte de los módulos de aplicación, utilizando como
soporte de almacenamiento la base de datos y el sistema de cachés de texto
para optimizar las lecturas.

Gestiona la carga y descarga de módulos. Mantiene cual es el módulo activo.
El módulo activo se puede establecer en cualquier momento con
FLManagerModules::setActiveIdModule().

Los módulos se engloban en áreas (FACTURACION, FINANCIERA, PRODUCCION, etc..) y
cada módulo tiene varios ficheros de texto XML y scripts. Toda la estructura de
módulos se almacena en las tablas flareas, flmodulos, flserial y flfiles, sirviendo esta
clase como interfaz para el manejo de dicha estructura en el entorno de trabajo
de FacturaLUX.

@author InfoSiAL S.L.
*/
class FL_EXPORT FLManagerModules {

  friend class FLSqlDatabase;

protected:

  /**
  constructor
  */
  FLManagerModules( FLSqlDatabase * db );

public:

  /**
  destructor
  */
  ~FLManagerModules();

public:

  /**
  Acciones de inicialización del sistema de módulos.
  */
  void init();

  /**
  Acciones de finalización del sistema de módulos.
  */
  void finish();

  /**
  Obtiene el contenido de un fichero almacenado la base de datos.

  Este método busca el contenido del fichero solicitado en la
  base de datos, exactamente en la tabla flfiles, si no lo encuentra
  intenta obtenerlo del sistema de ficheros.

  @param n Nombre del fichero.
  @return QString con el contenido del fichero o vacía en caso de error.
  */
  QString content( const QString & n );

  /**
  Obtiene el contenido de un fichero almacenado en el sistema de ficheros.

  @param pN Ruta y nombre del fichero en el sistema de ficheros
  @return QString con el contenido del fichero o vacía en caso de error.
  */
  QString contentFS( const QString & pN );

  /**
  Almacena el contenido de un fichero en un módulo dado.

  @param n Nombre del fichero.
  @param idM Identificador del módulo al que se asociará el fichero
  @param content Contenido del fichero.
  */
  void setContent( const QString & n, const QString & idM, const QString & content );

  /**
  Obtiene el contenido de un fichero, utilizando la caché de memoria y disco.

  Este método primero busca el contenido del fichero solicitado en la
  caché interna, si no está lo obtiene con el método FLManagerModules::content().

  @param n Nombre del fichero.
  @return QString con el contenido del fichero o vacía en caso de error.
  */
  QString contentCached( const QString & n, QString * shaKey = 0 );

  /**
  Obtiene el contenido de un fichero de script, procesándolo para cambiar las conexiones que contenga,
  de forma que al acabar la ejecución de la función conectada se reanude el guión de pruebas.

  Tambien realiza procesos de formateo del código para optimizarlo.

  @param n Nombre del fichero.
  @return QString con el contenido del fichero o vacía en caso de error.
  */
  QString getScript( const QString & n, QString * shaKey = 0, bool optimize = true );

  /**
  Crea un formulario a partir de su fichero de descripción.

  Utiliza el método FLManagerModules::contentCached() para obtener el texto XML que describe
  el formulario.

  @param n Nombre del fichero que contiene la descricpción del formulario.
  @return QWidget correspondiente al formulario construido.
  */
  QWidget * createUI( const QString & n, QObject * connector = 0, QWidget * parent = 0, const char * name = 0 );

  /**
  Crea el formulario maestro de una acción a partir de su fichero de descripción.

  Utiliza el método FLManagerModules::createUI() para obtener el formulario construido.

  @param a Objeto FLAction.
  @return QWidget correspondiente al formulario construido.
  */
  QWidget * createForm( FLAction * a, QObject * connector = 0, QWidget * parent = 0, const char * name = 0 );

  /**
  Esta función es igual a la anterior, sólo se diferencia en que carga
  la descripción de interfaz del formulario de edición de registros.
  */
  QWidget * createFormRecord( FLAction * a, QObject * connector = 0, QWidget * parent = 0, const char * name = 0 );

  /**
  Para establecer el módulo activo.

  Automáticamente también establece cual es el área correspondiente al módulo,
  ya que un módulo sólo puede pertenecer a una sola área.

  @param id Identificador del módulo
  */
  void setActiveIdModule( const QString & id );

  /**
  Para obtener el area del módulo activo.

  @return Identificador del area
  */
  QString & activeIdArea();

  /**
  Para obtener el módulo activo.

  @return Identificador del módulo
  */
  QString & activeIdModule();

  /**
  Obtiene la lista de identificadores de area cargadas en el sistema.

  @return Lista de identificadores de areas
  */
  QStringList listIdAreas();

  /**
  Obtiene la lista de identificadores de módulos cargados en el sistema de una area dada.

  @param idA Identificador del área de la que se quiere obtener la lista módulos
  @return Lista de identificadores de módulos
  */
  QStringList listIdModules( const QString & idA );

  /**
  Obtiene la lista de identificadores de todos los módulos cargados en el sistema.

  @return Lista de identificadores de módulos
  */
  QStringList listAllIdModules();

  /**
  Obtiene la descripción de un área a partir de su identificador.

  @param idA Identificador del área.
  @return Texto de descripción del área, si lo encuentra o idA si no lo encuentra.
  */
  QString idAreaToDescription( const QString & idA );

  /**
  Obtiene la descripción de un módulo a partir de su identificador.

  @param idM Identificador del módulo.
  @return Texto de descripción del módulo, si lo encuentra o idM si no lo encuentra.
  */
  QString idModuleToDescription( const QString & idM );

  /**
  Para obtener el icono asociado a un módulo.

  @param idM Identificador del módulo del que obtener el icono
  @return QPixmap con el icono
  */
  QPixmap iconModule( const QString & idM );

  /**
  Para obtener la versión de un módulo.

  @param idM Identificador del módulo del que se quiere saber su versión
  @return Cadena con la versión
  */
  QString versionModule( const QString & idM );

  /**
  Para obtener la clave sha local.

  @return Clave sha de la versión de los módulos cargados localmente
  */
  QString shaLocal();

  /**
  Para obtener la clave sha global.

  @return Clave sha de la versión de los módulos cargados globalmente
  */
  QString shaGlobal();

  /**
  Establece el valor de la clave sha local con el del global.
  */
  void setShaLocalFromGlobal();

  /**
  Obtiene la clave sha asociada a un fichero almacenado.

  @param n Nombre del fichero
  @return Clave sh asociada al ficheros
  */
  QString shaOfFile( const QString & n );

  /**
  Carga en el diccionario de claves las claves sha1 de los ficheros
  */
  void loadKeyFiles();

  /**
  Carga la lista de todos los identificadores de módulos
  */
  void loadAllIdModules();

  /**
  Carga la lista de todos los identificadores de areas
  */
  void loadIdAreas();

  /**
  Para obtener el identificador del módulo al que pertenece un fichero dado.

  @param n Nombre del fichero incluida la extensión
  @return Identificador del módulo al que pertenece el fichero
  */
  QString idModuleOfFile( const QString & n );

  /**
  Obtiene el contenido de un fichero desde FLManagerModules::staticDirFiles_.

  @param n Nombre del fichero.
  @return QString con el contenido del fichero o vacía en caso de error.
  */
  QString contentStaticDir( const QString & n );

protected:

  /**
  Guarda el estado del sistema de módulos
  */
  void writeState();

  /**
  Lee el estado del sistema de módulos
  */
  void readState();

private:

  /**
  Mantiene el identificador del area a la que pertenece el módulo activo.
  */
  QString activeIdArea_;

  /**
  Mantiene el identificador del módulo activo.
  */
  QString activeIdModule_;

  /**
  Mantiene la clave sha correspondiente a la version de los módulos cargados localmente
  */
  QString shaLocal_;

  /**
  Diccionario de claves de ficheros, para optimizar lecturas
  */
  QDict < QString > * dictKeyFiles;

  /**
  Lista de todos los identificadores de módulos cargados, para optimizar lecturas
  */
  QStringList * listAllIdModules_;

  /**
  Lista de todas los identificadores de areas cargadas, para optimizar lecturas
  */
  QStringList * listIdAreas_;

  /**
  Diccionario con información de los módulos
  */
  QDict < FLInfoMod > * dictInfoMods;

  /**
  Diccionario de identificadores de modulo de ficheros, para optimizar lecturas
  */
  QDict < QString > * dictModFiles;

  /**
  Fuerza a leer los ficheros desde un directorio local, en vez desde la base de datos,
  si contiene una ruta valida a un directorio
  */
  QString staticDirFiles_;

  /**
  Base de datos a utilizar por el manejador
  */
  FLSqlDatabase * db_;
};

#endif
