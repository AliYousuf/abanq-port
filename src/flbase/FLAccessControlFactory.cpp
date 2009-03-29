/***************************************************************************
                       FLAccessControlFactory.cpp
                          -------------------
 begin                : Fri Oct 10 2005
 copyright            : (C) 2004-2005 by InfoSiAL S.L.
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

#include "FLAccessControlFactory.h"
#include "FLAccessControl.h"
#include "FLTableMetaData.h"
#include "FLFieldMetaData.h"

/**
Control de Acceso para objetos de alto nivel del tipo "mainwindow"

@author InfoSiAL S.L.
*/
class FLAccessControlMainWindow : public FLAccessControl {

public:

  FLAccessControlMainWindow() : FLAccessControl() {}
  ~FLAccessControlMainWindow() {}

  /**
  @return El tipo del que se encarga; "mainwindow".
  */
  QString type();

  /**
  Procesa objetos que heredan de la clase QMainWindow.

  S�lo controla los hijos del objeto que son de la clase QAction, y s�lo
  permite hacerlos no visibles. Los permisos que acepta son :

  - "-w" o "--" (no_lectura/escritura o no_lectura/no_escritura) -> no visible

  Esto permite eliminar opciones de las barras de men�s y barras
  de herramientas de las ventanas principales, as� como opciones de
  activaci�n de m�dulos de la caja de herramientas principal, haci�ndolas
  no visibles para el usuario.
  */
  void processObject( void * obj );

  /** No implementado */
  void setFromObject( void * obj );
};

QString FLAccessControlMainWindow::type() {
  return "mainwindow";
}

void FLAccessControlMainWindow::processObject( void * obj ) {
  if ( !obj || !acosPerms_ )
    return ;

  if ( !perm_.isEmpty() ) {
    QObjectList * l = static_cast<QObject *>( obj )->queryList( "QAction" );
    QObjectListIt ito( *l );
    QAction * a;
    while ( ( a = static_cast<QAction *>( ito.current() ) ) != 0 ) {
      ++ito;
      if ( ( *acosPerms_ )[ a->name()] )
        continue;
      if ( perm_ == "-w" || perm_ == "--" )
        a->setVisible( false );
    }
    delete l;
  }

  QDictIterator < QString > it( *acosPerms_ );
  for ( ; it.current(); ++it ) {
    QAction * a = static_cast<QAction *>( static_cast<QObject *>( obj )->child( it.currentKey(), "QAction" ) );
    if ( a ) {
      QString perm = *( *it );
      if ( perm == "-w" || perm == "--" )
        a->setVisible( false );
    }
  }
}

void FLAccessControlMainWindow::setFromObject( void * obj ) {
#ifdef FL_DEBUG
  qWarning( "FLAccessControlMainWindow::setFromObject " + QApplication::tr( "No implementado todav�a." ) );
#endif
}

/**
Control de Acceso para objetos de alto nivel del tipo "form"

@author InfoSiAL S.L.
*/
class FLAccessControlForm : public FLAccessControl {

public:

  FLAccessControlForm();
  ~FLAccessControlForm() {}

  /**
  @return El tipo del que se encarga; "form".
  */
  QString type();

  /**
  Procesa objetos que son de la clase FLFormDB.

  S�lo controla los hijos del objeto que son de la clase QWidget,y s�lo
  permite hacerlos no visibles o no editables. En realidad hacerlos
  no visibles significa que sean no editables y modficando la paleta para
  que toda la regi�n del componente sea mostrada en color negro. Los permisos
  que acepta son :

  - "-w" o "--" (no_lectura/escritura o no_lectura/no_escritura) -> no visible
  - "r-" (lectura/no_escritura) -> no editable

  Esto permite que cualquier componente de un formulario de FacturaLUX ( FLFormDB,
  FLFormRecordDB y FLFormSearchDB) se pueda hacer no visible o no editable a conveniencia.
  */
  void processObject( void * obj );

  /** No implementado */
  void setFromObject( void * obj );

private:

  QPalette pal;
};

FLAccessControlForm::FLAccessControlForm() : FLAccessControl() {
  QColorGroup cg;
  QColor bg = qApp->palette().color( QPalette::Active, QColorGroup::Background );
  cg.setColor( QColorGroup::Foreground, bg );
  cg.setColor( QColorGroup::Text, bg );
  cg.setColor( QColorGroup::ButtonText, bg );
  cg.setColor( QColorGroup::Base, bg );
  cg.setColor( QColorGroup::Background, bg );
  pal.setDisabled( cg );
}

QString FLAccessControlForm::type() {
  return "form";
}

void FLAccessControlForm::processObject( void * obj ) {
  if ( !obj || !acosPerms_ )
    return ;

  if ( !perm_.isEmpty() ) {
    QObjectList * l = static_cast<QObject *>( obj ) ->queryList( "QWidget" );
    QObjectListIt ito( *l );
    QWidget * w;
    while ( ( w = static_cast<QWidget *>( ito.current() ) ) != 0 ) {
      ++ito;
      if ( ( *acosPerms_ )[ w->name()] )
        continue;
      if ( perm_ == "-w" || perm_ == "--" ) {
        w->setPalette( pal );
        w->setDisabled( true );
        w->hide();
        continue;
      }
      if ( perm_ == "r-" )
        w->setDisabled( true );
    }
    delete l;
  }

  QDictIterator < QString > it( *acosPerms_ );
  for ( ; it.current(); ++it ) {
    QWidget * w = static_cast<QWidget * >( static_cast<QObject *>( obj )->child( it.currentKey(), "QWidget" ) );
    if ( w ) {
      QString perm = *( *it );
      if ( perm == "-w" || perm == "--" ) {
        w->setPalette( pal );
        w->setDisabled( true );
        w->hide();
        continue;
      }
      if ( perm == "r-" )
        w->setDisabled( true );
    }
  }
}

void FLAccessControlForm::setFromObject( void * obj ) {
#ifdef FL_DEBUG
  qWarning( "FLAccessControlForm::setFromObject " + QApplication::tr( "No implementado todav�a." ) );
#endif
}

/**
Control de Acceso para objetos de alto nivel del tipo "table"

@author InfoSiAL S.L.
*/
class FLAccessControlTable : public FLAccessControl {

public:

  FLAccessControlTable() : FLAccessControl() {}
  ~FLAccessControlTable() {}

  /**
  @return El tipo del que se encarga; "table".
  */
  QString type();

  /**
  Procesa objetos que son de la clase FLTableMetaData.

  S�lo controla los objetos que pertenecen a �l de la clase FLFieldMetaData
  y s�lo permite hacerlos no visibles o no editables. Esto en la pr�ctica
  significa que los componentes de los formularios de la clase FLFieldDB y
  FLTableDB utilizar�n los metadatos para autoconfigurarse y ser�n no visibles
  o no editables si as� lo establecen los objetos FLTableMetaData y FLFieldMetaData
  que definen esos metadatos.

  Los permisos que acepta son :

  - "-w" o "--" (no_lectura/escritura o no_lectura/no_escritura) -> no visible
  - "r-" (lectura/no_escritura) -> no editable

  Esto permite que cualquier componente de un formulario de FacturaLUX ( FLFormDB,
  FLFormRecordDB y FLFormSearchDB) del tipo FLFieldDB o FLTableDB sea no visible
  o no editable a partir de los metadatos que utiliza para autoconfigurarse,
  independientemente del formulario en que se encuentre. Por ejemplo, si definimos
  que el campo "nombre" de la tabla "clientes" es no visible, implicar� que cada vez
  que un componente tipo FLFieldDB forme parte de un formulario y est� enlazando con el campo
  "nombre" de "clientes" su contenido sea no visible para el usuario.
  */
  void processObject( void * obj );

  /**
  Crea la regla la regla de control de acceso a partir de objetos de la clase FLTableMetaData.

  Al igual que en FLAccessControlTable::processObject() s�lo tiene en cuenta
  las propiedades 'visible' y 'editable' de los campos, a partir de las cuales
  crea la regla.
  */
  void setFromObject( void * obj );
};

QString FLAccessControlTable::type() {
  return "table";
}

void FLAccessControlTable::processObject( void * obj ) {
  if ( !obj )
    return ;

  int maskPerm = 0;
  bool hasAcos = ( acosPerms_ && !acosPerms_->isEmpty() );

  if ( !perm_.isEmpty() ) {
    if ( perm_.left( 1 ) == "r" )
      maskPerm |= 2;
    if ( perm_.right( 1 ) == "w" )
      maskPerm |= 1;
  } else if ( hasAcos )
      maskPerm = 8;
    else
      return;

  QString fieldPerm;
  QString * fieldPermPtr;
  int maskFieldPerm = 0;

  FLTableMetaData::FLFieldMetaDataList *fL = static_cast<FLTableMetaData *>( obj ) ->fieldList();
  if ( !fL )
    return ;

  FLFieldMetaData * field;
  QDictIterator<FLFieldMetaData> it( *fL );

  while ( ( field = it.current() ) != 0 ) {
    maskFieldPerm = maskPerm;
    ++it;

    if ( hasAcos && ( fieldPermPtr = ( *acosPerms_ )[ field->name()] ) ) {
      fieldPerm = *fieldPermPtr;
      maskFieldPerm = 0;

      if ( fieldPerm.left( 1 ) == "r" )
        maskFieldPerm |= 2;
      if ( fieldPerm.right( 1 ) == "w" )
        maskFieldPerm |= 1;
    }

    switch ( maskFieldPerm ) {
      case 0:
        field->setVisible( false );
        field->setEditable( false );
        break;
      case 1:
        field->setVisible( false );
        field->setEditable( true );
        break;
      case 2:
        field->setVisible( true );
        field->setEditable( false );
        break;
      case 3:
        field->setVisible( true );
        field->setEditable( true );
        break;
    }
  }
}

void FLAccessControlTable::setFromObject( void * obj ) {
  if ( !obj )
    return;

  if ( acosPerms_ ) {
    acosPerms_->clear();
    delete acosPerms_;
  }

  acosPerms_ = new QDict < QString >( 31 );
  acosPerms_->setAutoDelete( true );

  FLTableMetaData::FLFieldMetaDataList *fL = static_cast<FLTableMetaData *>( obj ) ->fieldList();
  if ( !fL )
    return;

  FLFieldMetaData * field;
  QChar permR, permW;
  QDictIterator<FLFieldMetaData> it( *fL );

  while ( ( field = it.current() ) != 0 ) {
    ++it;
    permR = '-';
    permW = '-';
    if ( field->visible() )
      permR = 'r';
    if ( field->editable() )
      permW = 'w';
    acosPerms_->replace( field->name(), new QString( QString( permR ) + permW ) );
  }
}

FLAccessControl * FLAccessControlFactory::create( const QString & type ) {
  if ( type.isEmpty() )
    return 0;

  if ( type == "mainwindow" )
    return new FLAccessControlMainWindow();
  else if ( type == "form" )
    return new FLAccessControlForm();
  else if ( type == "table" )
    return new FLAccessControlTable();

  return 0;
}

QString FLAccessControlFactory::type( QObject * obj ) {
  if ( !obj )
    return QString::null;

  if ( obj->isA( "QMainWindow" ) )
    return "mainwindow";
  else if ( obj->isA( "FLTableMetaData" ) )
    return "table";
  else if ( obj->inherits( "FLFormDB" ) )
    return "form";

  return QString::null;
}
