/***************************************************************************
                              FLTableDB.h
                          -------------------
 begin                : Sun Jul 1 2001
 copyright            : (C) 2001-2005 by InfoSiAL S.L.
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

#ifndef FLTABLEDB_H
#define FLTABLEDB_H

#include <qlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qvalidator.h>
#include <qstringlist.h>
#include <qwidgetstack.h>
#include <qbuttongroup.h>
#include "FLWidgetTableDB.h"

class FLSqlCursor;
class FLDataTable;
class FLFieldMetaData;
class FLTableDBInterface;

/**
PLUGIN que contiene una tabla de la base de datos.

Este objeto contiene todo lo necesario para manejar
los datos de una tabla. Adem�s de la funcionalidad de
busqueda en la tabla por un campo, mediante filtros.

Este plugin para que sea funcional debe tener como uno
de sus padres o antecesor a un objeto FLFormDB.

@author InfoSiAL S.L.
*/
class FL_EXPORT FLTableDB: public FLWidgetTableDB {

  Q_OBJECT

  Q_PROPERTY( QString tableName READ tableName WRITE setTableName )
  Q_PROPERTY( QString foreignField READ foreignField WRITE setForeignField )
  Q_PROPERTY( QString fieldRelation READ fieldRelation WRITE setFieldRelation )
  Q_PROPERTY( bool checkColumnEnabled READ checkColumnEnabled WRITE setCheckColumnEnabled )
  Q_PROPERTY( QString aliasCheckColumn READ aliasCheckColumn WRITE setAliasCheckColumn )
  Q_PROPERTY( bool findHidden READ findHidden WRITE setFindHidden )
  Q_PROPERTY( bool filterHidden READ filterHidden WRITE setFilterHidden )

  friend class FLTableDBInterface;
  friend class FLFormDB;

public:

  /**
  constructor
  */
  FLTableDB( QWidget * parent = 0, const char *name = 0 );

  /**
  Obtiene el componente tabla de registros
  */
  FLDataTable * tableRecords();

  /**
  Para obtener el cursor utilizado por el componente.

  return Objeto FLSqlCursor con el cursor que contiene los registros para ser utilizados en el formulario
  */
  FLSqlCursor *cursor();

  /**
  Para obtener el nombre de la tabla asociada.

  @return Nombre de la tabla asociado
  */
  QString tableName() const;

  /**
  Para establecer el nombre de la tabla asociada.

  @param fT Nombre de la tabla asociada
  */
  void setTableName( const QString & fT );

  /**
  Para obtener el nombre del campo for�neo.

  @return Nombre del campo
  */
  QString foreignField() const;

  /**
  Para establecer el nombre del campo for�neo.

  @param fN Nombre del campo
  */
  void setForeignField( const QString & fN );

  /**
  Para obtener el nombre del campo relacionado.

  @return Nombre del campo
  */
  QString fieldRelation() const;

  /**
  Para establecer el nombre del campo relacionado.

  @param fN Nombre del campo
  */
  void setFieldRelation( const QString & fN );

  /**
  Establece si el componente esta en modo solo lectura o no.
  */
  void setReadOnly( const bool mode );

  /**
  Establece si el componente esta en modo solo edici�n o no.
  */
  void setEditOnly( const bool mode );

  /**
  Establece el componente a s�lo inserci�n o no.
  */
  void setInsertOnly( const bool mode );

  /**
  Establece el filtro inicial de b�squeda
  */
  void setInitSearch( const QString & iS ) {
    initSearch_ = iS;
  }

  /**
  Establece el orden de las columnas de la tabla.

  @param fields Lista de los nombres de los campos ordenada seg�n se desea que aparezcan en la tabla de izquierda a derecha
  */
  void setOrderCols( QStringList & fields );

  /**
  Devuelve la lista de los campos ordenada por sus columnas en la tabla de izquierda a derecha
  */
  QStringList orderCols();

  /**
  Establece el filtro de la tabla

  @param f Sentencia Where que establece el filtro
  */
  void setFilter( const QString &f );

  /**
  Devuelve el filtro de la tabla

  @return Filtro
  */
  QString filter();

  /**
  Obtiene si la columna de selecci�n est� activada
  */
  bool checkColumnEnabled() const;

  /**
  Establece el estado de activaci�n de la columna de selecci�n

  El cambio de estado no ser� efectivo hasta el siguiente refresh.
  */
  void setCheckColumnEnabled( bool b );

  /**
  Obiente el texto de la etiqueta de encabezado para la columna de selecci�n
  */
  QString aliasCheckColumn() const;

  /**
  Establece el texto de la etiqueta de encabezado para la columna de selecci�n

  El cambio del texto de la etiqueta no ser� efectivo hasta el pr�ximo refresh
  */
  void setAliasCheckColumn( const QString & t );

  /**
  Obtiene si el marco de b�squeda est� oculto
  */
  bool findHidden() const;

  /**
  Oculta o muestra el marco de b�squeda

  @param  h TRUE lo oculta, FALSE lo muestra
  */
  void setFindHidden( bool h );

  /**
  Obtiene si el marco para conmutar entre datos y filtro est� oculto
  */
  bool filterHidden() const;

  /**
  Oculta o muestra el marco para conmutar entre datos y filtro

  @param  h TRUE lo oculta, FALSE lo muestra
  */
  void setFilterHidden( bool h );

protected:

  /**
  Filtro de eventos
  */
  bool eventFilter( QObject * obj, QEvent * ev );

  /**
  Captura evento mostrar
  */
  void showEvent( QShowEvent * e );

private:

  /**
  Tipos de condiciones para el filtro
  */
  enum CondType {
    All,
    Contains,
    Starts,
    End,
    Equal,
    Dist,
    Greater,
    Less,
    FromTo,
    Null,
    NotNull
  };

  /**
  Redefinida por conveniencia
  */
  void showWidget();

  /**
  Asigna el cursor actual del componente a la tabla de registros
  */
  void setTableRecordsCursor();

  /**
  Refresca la pesta�a datos aplicando el filtro
  */
  void refreshTabData();

  /**
  Refresca la pesta�a del filtro
  */
  void refreshTabFilter();

  /**
  Para obtener la enumeraci�n correspondiente a una condici�n para el filtro a partir de
  su literal
  */
  FLTableDB::CondType decodeCondType( const QString & strCondType ) const;

  /**
  Construye la cla�sula de filtro en SQL a partir del contenido de los valores
  definidos en la pesta�a de filtro
  */
  QString tdbFilterBuildWhere();

  /**
  Inicializa un editor falso y no funcional.

  Esto se utiliza cuando se est� editando el formulario con el dise�ador y no
  se puede mostrar el editor real por no tener conexi�n a la base de datos.
  Crea una previsualizaci�n muy esquem�tica del editor, pero suficiente para
  ver la posisic�n y el tama�o aproximado que tendr� el editor real.
  */
  void initFakeEditor();

  /**
  Componente para visualizar los registros
  */
  FLDataTable * tableRecords_;

  /**
  Nombre de la tabla a la que esta asociado este componente.
  */
  QString tableName_;

  /**
  Nombre del campo for�neo
  */
  QString foreignField_;

  /**
  Nombre del campo de la relaci�n
  */
  QString fieldRelation_;

  /**
  Cursor con los datos de origen para el componente
  */
  FLSqlCursor *cursor_;

  /**
  Cursor auxiliar de uso interno para almacenar los registros de la tabla
  relacionada con la de origen
  */
  FLSqlCursor *cursorAux;

  /**
  Matiene la ventana padre
  */
  QWidget *topWidget;

  /**
  Indica que la ventana ya ha sido mostrada una vez
  */
  bool showed;

  /**
  Mantiene el filtro de la tabla
  */
  QString filter_;

  /**
  Almacena si el componente est� en modo s�lo lectura
  */
  bool readonly_;
  bool reqReadOnly_;

  /**
  Almacena si el componente est� en modo s�lo edici�n
  */
  bool editonly_;
  bool reqEditOnly_;

  /**
  Indica si el componente est� en modo s�lo permitir a�adir registros
  */
  bool insertonly_;
  bool reqInsertOnly_;

  /**
  Almacena los metadatos del campo por el que est� actualmente ordenada la tabla
  */
  FLFieldMetaData *sortField_;

  /**
  Cr�nometro interno
  */
  QTimer *timer;

  /**
  Filtro inicial de b�squeda
  */
  QString initSearch_;

  /**
  Indica que la columna de seleci�n est� activada
  */
  bool checkColumnEnabled_;

  /**
  Indica el texto de la etiqueta de encabezado para la columna de selecci�n
  */
  QString aliasCheckColumn_;

  /**
  Indica el nombre para crear un pseudocampo en el cursor para la columna de selecci�n
  */
  QString fieldNameCheckColumn_;

  /**
  Indica que la columna de selecci�n est� visible
  */
  bool checkColumnVisible_;

  /**
  Indica el n�mero de columna por la que ordenar los registros
  */
  int sortColumn_;

  /**
  Indica el sentido ascendente o descendente del la ordenacion actual de los registros
  */
  bool orderAsc_;

  /**
  Almacena la �ltima cla�sula de filtro aplicada en el refresco
  */
  QString tdbFilterLastWhere_;

  /**
  Diccionario que relaciona literales descriptivos de una condici�n de filtro
  con su enumeraci�n
  */
  QMap<QString, FLTableDB::CondType> mapCondType;

  /**
  Indica si el marco de b�squeda est� oculto
  */
  bool findHidden_;

  /**
  Indica si el marco para conmutar entre datos y filtro est� oculto
  */
  bool filterHidden_;

  /**
  Editor falso
  */
  QWidget * fakeEditor_;

public slots:

  /**
  Actualiza el conjunto de registros.
  */
  void refresh( const bool refreshHead = false, const bool refreshData = false );

  /**
  Actualiza el conjunto de registros con un retraso.

  Acepta un lapsus de tiempo en milisegundos, activando el cron�metro interno para
  que realize el refresh definitivo al cumplirse dicho lapsus.

  @param msec Cantidad de tiempo del lapsus, en milisegundos.
  */
  void refreshDelayed( int msec = 50, const bool refreshData = true );

  /**
  Invoca al m�todo FLSqlCursor::insertRecord()
  */
  void insertRecord();

  /**
  Invoca al m�todo FLSqlCursor::editRecord()
  */
  void editRecord();

  /**
  Invoca al m�todo FLSqlCursor::browseRecord()
  */
  void browseRecord();

  /**
  Invoca al m�todo FLSqlCursor::deleteRecord()
  */
  void deleteRecord();

  /**
  Invoca al m�todo FLSqlCursor::copyRecord()
  */
  void copyRecord();

  /**
  Coloca la columna como primera pasando el nombre del campo.

  Este slot est� conectado al cuadro combinado de busqueda
  del componente. Cuando seleccionamos un campo este se coloca
  como primera columna y se reordena la tabla con esta columna.
  De esta manera siempre tendremos la tabla ordenada mediante
  el campo en el que queremos buscar.

  @param c Nombre del campo, esta columna intercambia su posion con la primera columna
  @return Falso si no existe el campo
  @author viernes@xmarts.com.mx
  @author InfoSiAL, S.L.
  */
  void putFirstCol( const QString & c );

  /**
  Mueve una columna de un campo origen a la columna de otro campo destino

  @param  from  Nombre del campo de la columna de origen
  @param  to    Nombre del campo de la columna de destino
  */
  void moveCol( const QString & from, const QString & to );

  /**
  Inicia el cursor segun este campo sea de la tabla origen o de
  una tabla relacionada
  */
  void initCursor();

  /**
  Posiciona el cursor en un registro valido
  */
  void seekCursor();

protected slots:

  /**
  Coloca la columna indicada como primera.

  Este slot est� conectado al cuadro combinado de busqueda
  del componente. Cuando seleccionamos un campo este se coloca
  como primera columna y se reordena la tabla con esta columna.
  De esta manera siempre tendremos la tabla ordenada mediante
  el campo en el que queremos buscar.

  @param c Numero de la columna en la tabla, esta columna intercambia
       su posion con la primera columna
  */
  void putFirstCol( int c );

  /**
  Mueve una columna desde una posicion origen a otra posicion destino.

  @param  from  Posicion de la columna de origen
  @param  to    Posicion de la columna de destino
  */
  void moveCol( int from, int to );

  /**
  Filtra los registros de la tabla utilizando el primer campo, seg�n el patr�n dado.

  Este slot est� conectado al cuadro de texto de busqueda del componente,
  tomando el contenido de este como patr�n para el filtrado.

  @param p Cadena de caracteres con el patr�n de filtrado
  */
  void filterRecords( const QString & p );

  /**
  Conmuta el sentido de la ordenaci�n de los registros de la tabla, de ascendente a descendente y
  viceversa. Los registros siempre se ordenan por la primera columna.
  */
  void switchSortOrder( int );

  /**
  Activa la tabla de datos
  */
  void activeTabData( bool );

  /**
  Activa la tabla de filtro
  */
  void activeTabFilter( bool );

  /**
  Limpia e inicializa el filtro
  */
  void tdbFilterClear();

signals:

  /**
  Se�al emitida cuando se establece si el componente es o no de solo lectura.
  */
  void readOnlyChanged( bool );

  /**
  Se�al emitida cuando se establece si el componente es o no de solo edici�n.
  */
  void editOnlyChanged( bool );

  /**
  Se�al emitida cuando se establece si el componente es o no de solo inserci�n.
  */
  void insertOnlyChanged( bool );

  /**
  Se�al emitida cuando se establece cambia el registro seleccionado.
  */
  void currentChanged();
};

#endif
