/***************************************************************************
                            FLDataTable.h
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

#ifndef FLDATATABLE_H
#define FLDATATABLE_H

#include <qdatatable.h>
#include <qpainter.h>
#include <qpixmapcache.h>
#include <qdatetime.h>
#include <qcolor.h>
#include <qcheckbox.h>

class FLSqlCursor;

/**
Clase que es una redefinicion de la clase QDataTable,
especifica para las necesidades de FacturaLUX.

@author InfoSiAL S.L.
*/
class FL_EXPORT FLDataTable: public QDataTable {

  Q_OBJECT

public:

  /**
  constructor
  */
  FLDataTable( QWidget * parent = 0, const char *name = 0, bool popup = false );

  /**
  desctructor
  */
  ~FLDataTable();

  /**
  Establece el cursor
  */
  void setFLSqlCursor( FLSqlCursor * c );

  /**
  Establece un filtro persistente que siempre se aplica al cursor antes
  de hacer un refresh
  */
  void setPersistentFilter( const QString & pFilter );

  /**
  Devuelve el cursor
  */
  FLSqlCursor *cursor() const {
    return cursor_;
  }

  /**
  Establece la tabla a s�lo lectura o no
  */
  void setFLReadOnly( const bool mode );

  /**
  Establece la tabla a s�lo edici�n o no
  */
  void setEditOnly( const bool mode );

  /**
  Establece la tabla a s�lo insercion o no
  */
  void setInsertOnly( const bool mode );

  /**
  Obtiene la lista con las claves primarias de los registros seleccionados por chequeo
  */
  const QValueList<QVariant> primarysKeysChecked() const;

  /**
  Limpia la lista con las claves primarias de los registros seleccionados por chequeo
  */
  void clearChecked();

  /**
  Establece el estado seleccionado por chequeo para un regsitro, indicando el valor de su clave primaria
  */
  void setPrimaryKeyChecked( const QVariant & primaryKeyValue, bool on );

protected:

  /**
  Filtrado de eventos
  */
  bool eventFilter( QObject * o, QEvent * e );

  /**
  Redefinido por conveniencia para pintar la celda
  */
  void paintCell( QPainter * p, int row, int col, const QRect & cr,
                  bool selected, const QColorGroup & cg );

  /**
  Redefinido por conveniencia para pintar el campo
  */
  void paintField( QPainter * p, const QSqlField * field, const QRect & cr, bool );

  /**
  Redefinido por conveniencia, para evitar que aparezca el menu contextual
  con las opciones para editar registros
  */
  void contentsContextMenuEvent( QContextMenuEvent * e );

  /**
  Redefine por conveniencia, el comportamiento al hacer doble clic sobre una
  celda
  */
  void contentsMouseDoubleClickEvent( QMouseEvent * e );

  /**
  Redefinida por conveniencia
  */
  void focusOutEvent( QFocusEvent * );

private:

  /**
  Numero de la fila (registro) seleccionada actualmente
  */
  int rowSelected;

  /**
  Numero de la columna (campo) seleccionada actualmente
  */
  int colSelected;

  /**
  Cursor, con los registros
  */
  FLSqlCursor *cursor_;

  /**
  Almacena la tabla est� en modo s�lo lectura
  */
  bool readonly_;

  /**
  Almacena la tabla est� en modo s�lo edici�n
  */
  bool editonly_;

  /**
  Indica si la tabla est� en modo s�lo inserci�n
  */
  bool insertonly_;

  /**
  Texto del �ltimo campo dibujado en la tabla
  */
  QString lastTextPainted;

  /**
  Brochas para el color resaltado de filas
  */
  QBrush bu_;

  /**
  Pixmap precargados
  */
  QPixmap ok;
  QPixmap no;

  /**
  Lista con las claves primarias de los registros seleccionados por chequeo
  */
  QValueList<QVariant> primarysKeysChecked_;

  /**
  Filtro persistente para el cursor
  */
  QString persistentFilter_;

  /**
  Indicador para evitar refrescos anidados
  */
  bool refreshing_;

  /**
  Indica si el componente es emergente ( su padre es un widget del tipo Popup )
  */
  bool popup_;

public slots:

  /**
  Selecciona la fila completa en la que est� la celda activa.

  @param r Fila de la celda activa
  @param c Columna de la celda activa
  */
  void selectRow( int r = -1, int c = -1 );

  /**
  Redefinida por conveniencia
  */
  void setFocus();

  /**
  Redefinida por conveniencia
  */
  void refresh();

  /**
  Hace que la fila seleccionada est� visible
  */
  void ensureRowSelectedVisible();

  /**
  Foco r�pido sin refrescos para optimizar
  */
  void setQuickFocus();

protected slots:

  /**
  Activado cuando se hace click en el chequeo de la columna de selecci�n
  */
  void setChecked( bool on );

signals:

  /**
  Indica que se ha elegido un registro
  */
  void recordChoosed();

  /**
  Indica que ha cambiado el estado del campo de selecci�n de un registro. Es decir
  se ha incluido o eliminado su clave primaria de la lista de claves primarias seleccionadas.
  Esta se�al se emite cuando el usuario hace click en el control de chequeo y cuando se cambia
  program�ticamente el chequeo mediante el m�todo FLDataTable::setPrimaryKeyChecked.

  @param  primaryKeyValue El valor de la clave primaria del registro correspondiente
  @param  on  El nuevo estado; TRUE chequeo activado, FALSE chequeo desactivado
  */
  void primaryKeyToggled( const QVariant & primaryKeyValue, bool on );
};

// Uso interno
class FL_EXPORT FLCheckBox: public QCheckBox {

public:

  FLCheckBox( QWidget * parent, const char * name = 0 ) : QCheckBox( parent, name ) {}

protected:

  void drawButton( QPainter * p );
  bool hitButton( const QPoint & pos ) const;
};

#endif
