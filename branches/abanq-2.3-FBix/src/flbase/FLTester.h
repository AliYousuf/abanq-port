/***************************************************************************
                            FLTester.h
                        -------------------
begin                : lun 29 Mar 2004
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
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef FLTESTER_H
#define FLTESTER_H

#include <qapplication.h>
#include <qdom.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qobjectlist.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <qbuffer.h>
#include <qwidgetfactory.h>
#include <qsqlquery.h>
#include <qsqlcursor.h>
#include <qtimer.h>
#include <qradiobutton.h>
#include <qeventloop.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qptrqueue.h>
#include <qtabwidget.h>

class FLFormDB;
class FLSqlDatabase;

class FLTestFdb {

public:
  QString fdbName;
  QString fdbValue;
};

/**
Eventos para las pruebas
*/
class FL_EXPORT FLTesterEvent {

public:

  /**
  constructor
  */
  FLTesterEvent() {}

  /**
  destructor
  */
  ~FLTesterEvent() {}

  /**
  Condición actual que debe cumplirse para continuar con el guión de pruebas
  */
  int condition;

  /**
  Nombre del objeto sobre el que se aplica la condición actual
  */
  QString objectCondition;
};

/**
Clase para automatizar las pruebas de calidad del software

@author InfoSiAL S.L.
*/
class FL_EXPORT FLTester: public QObject {

  Q_OBJECT

public:

  /**
  constructor.
  */
  FLTester();

  /**
  destructor
  */
  virtual ~FLTester() ;

  /**
  Constantes para indicar el resultado al ejecutar un paso
  */
  enum StepResult {
    /** El paso se completó correctamente */
    OK = 0,
    /** Hubo un error controlado en la ejecución del paso */
    ERR = 1,
    /** El paso debe repetirse por no estar listo todavía el entorno */
    REPEAT = 2,
    /** Hubo un error de ejecución en el paso */
    TESTER_ERROR = 3,
    /** La ejecución del guión debe saltarse la prueba actual*/
    SKIP_TEST = 4,
    /** La ejecución del guión debe continuar sin esperar*/
    NEXT = 5
  };

  /**
  Constantes para indicar la condición que debe cumplirse para continuar ejecutando el guión de pruebas
  */
  enum Condition {
    /** Cualquier hay condición */
    NO_CONDITION,
    /** FLFormDB listo */
    FORM_READY,
    /** FLFormDB cerrado */
    FORM_CLOSED,
    /** FLFormRecordDB listo */
    FORM_RECORD_READY,
    /** FLFormSearchDB listo */
    FORM_SEARCH_READY,
    /** FLFieldDB listo */
    FIELD_READY,
    /** MessageBox listo */
    MESSAGE_BOX_READY,
    /** MessageBox cerrado */
    MESSAGE_BOX_CLOSED,
    /** Registro borrado */
    RECORD_DELETED,
    /** Script finalizado*/
    SCRIPT_FINISHED,
    /** Ninguna condición*/
    NONE_IS_VALID
  };


  /**
  Obtiene la lista de sesiones cargadas en la base de datos de test
  @param bd Nombre de la base de datos de test
  @return Cadena que contiene la lista con formato idfuncional//codsessioncat//description**...
  */
  QString sessionsList( const QString &bd );

  /**
  Devuelve el identificador actual de la sesión de ejecución de tests
  */
  int currentSession() {
    return currentSession_;
  }

  /**
  Devuelve el estado de la sesión actual
  */
  QString sessionStatus() {
    return sessionStatus_;
  }

  /**
  Devuelve la conexión a la base de datos utilizada para los tests
  */
  FLSqlDatabase * dbTest() {
    return dbTest_;
  }

  /**
  Obtiene si se están ejecutando tests
  */
  bool testing() {
    return testing_;
  }

  /**
  Encola un evento en la cola de eventos para las pruebas

  @param c: Condición
  @param oC: Objeto sobre el que se impone la condición
  */
  void enqueueTesterEvent( const int c, const QString & oC );

public slots:

  /**
  Inicia sesión de tests

  @param codSessionCat: Código de la sesión a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesión
  @param bd: Nombre de la base de datos que contiene el catálogo de pruebas
  */
  void start( const QString &codSessionCat, const QString &idFuncional = QString::null, const QString &bd = QString::null );

protected:

  /**
  Crea los pasos correspondientes a una sesión de pruebas en el guión de pasos

  @param codSessionCat: Código de la sesión a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesión
  @return: Identificador de la sesión creada, 0 si hay error
  */
  uint createSession( const QString &codSessionCat, const QString &idFuncional = QString::null );

  /**
  Añade los pasos de un test al guión de pasos

  @param idTest; Identificador del test en el guión de pasos
  @param idTestCat; Identificador del test en el catálogo de pasos
  @param testLevel; Indicador del nivel de anidamiento de pruebas
  @return: True si no hay error, false en caso contrario
  */
  bool addTest( const uint &idTest, const QString idTestCat, uint testLevel );

  /**
  Abre la base de datos de tests, y la guarda en dbTest_

  @param bd: Nombre de la base de datos
  */
  void openTestDB( const QString &bd );

protected slots:

  /**
  Termina la sesión de tests.
  */
  void end();

  /**
  Ejecuta un paso del guión de pasos

  @return: True si el paso se completó correctamente y hay más pasos que procesar
  */
  bool execTest();

  /**
  Intenta ejecutar la siguiente prueba del guión de pruebas. La prueba se ejecutará si la condición y el objeto indicados son los esperados

  @param c: Condición
  @param oC: Objeto sobre el que se impone la condición
  @return True la prueba se puede ejecutar y se ejecuta correctamente, false en caso contrario
  */
  bool tryContinue( const int c, const QString & oC );

  /**
  Esencialmente hace lo mismo que el método FLTester::tryContinue( const int c, const QString & oC ).

  En este método obtiene la información de condición y objeto de la cola de eventos, ver FLTesterEvent y queueTesterEvents
  */
  bool tryContinue();

private:

  /**
  Lanza un paso del guión de pasos

  @param idStepCat: Identificador del paso en el catálogo de pasos
  @param idStep: Identificador del paso en el guión de pruebas
  @return (valor de tipo StepResult)
  */
  int execStep( const QString & idStepCat, const QString & idStep );

  /**
  Cambia el estado de la prueba actual

  @param status: estado de la prueba
  @param errorDescription: Descripción del error produducido, en su caso
  */
  void setTestStatus( const QString errorDescription );

  /**
  Cambia el estado de la sesión actual

  @param status: estado de la prueba
  */
  void setSessionStatus( const QString );

  /**
  Obtiene el valor resultado de una consulta

  @param n: Nodo XML con los datos de la consulta
  @param v: Variable donde se devolverá el resutado de la consulta (FALSE si la consulta no encuentra registros o el primer valor encontrado)
  @return Código de error similar al de execStep
  */
  int getQueryValue( QDomNode n, QVariant &v );

  /**
  Obtiene el valor resultado asociado a un nodo de comparación

  @param n: Nodo XML con los datos del elemento a comparar
  @param v: Variable donde se devolverá el valor del elemento
  @return Código de error similar al de execStep
  */
  int getValue( QDomNode n, QVariant &v );

  /**
  Preprocesa el string de parámetros sustituyendo las variables por sus valores

  @param parameters: String de parámetros a procesar
  @return True si el preproceso se completa con éxito, false en caso contrario
  */
  bool processParameters( QString &parameters );

  /**
  Establece la condición requerida para continuar con el siguiente paso del guión de pruebas

  @param e: Nodo que contiene los atributos que definen la condición
  @param defCondition: Condición por defecto
  @param defObjectCondition: Objeto asociado a la condición por defecto
  */
  void setConditions( QDomElement e, const Condition & defCondition, const QString & defObjectCondition );

  /**
  Realiza una transacción a través de la conexión de la base de datos de pruebas

  @param qrySql: Sentencia SQL a ejecutar
  @return True si la transacción se completa correctamente (se realiza el Commit), false en caso contrario (Rollback)
  */
  bool execTransaction( const QString &qrySql );

  /**
  Asigna valores a variables globales

  @param parameters: Nodo XML con las asignaciones a realizar
  @return True si la asignación se completa correctamente, false en caso contrario 
  */
  bool assignateValues( const QString &parameters );

  /**
  Asigna valores por defecto necesarios para ejecutar una determinada prueba

  @param testName: Nodo XML con los identificadores (identificador y funcionalidad) de la prueba
  @return True si la asignación se completa correctamente, false en caso contrario 
  */
  bool assignateDefaultValues( const QString & testName );

  /**
  Asigna un valor a una variable global, usando la conexión de pruebas

  @param name: Nombre de la variable
  @param value: Valor de la variable
  @return True si la asignación se completa correctamente, false en caso contrario 
  */
  bool setVar( const QString &name, const QVariant &value );

  /**
  Obtiene el valor de una variable global, usando la conexión de pruebas

  @param name: Nombre de la variable
  @param exists: Indicador de si la variable está o no definida
  @return Valor de la variable
  */
  QVariant getVar( const QString &name, bool *exists );

  /**
  Elimina una variable global, usando la conexión de pruebas

  @param name: Nombre de la variable
  @return True si la eliminación se completa correctamente, false en caso contrario 
  */
  bool delVar( const QString &name );

  /**
  Número de fallos tolerados hasta que se para la prueba
  */
  int fallos_;

  /**
  Número correspondiente a la prueba anterior
  */
  QString previousTest_;

  /**
  Número correspondiente a la prueba actual
  */
  QString currentTest_;

  /**
  Número correspondiente a la sesión actual
  */
  int currentSession_;

  /**
  Formulario actual sobre el que se realiza la prueba
  */
  FLFormDB *currentForm_;

  /**
  Conexión a la base de datos de test.
  */
  FLSqlDatabase * dbTest_;

  /**
  Nombre del módulo que se está probando
  */
  QString currentModule_;

  /**
  Estado de la sesión de pruebas actual
  */
  QString sessionStatus_;

  /**
  Descripción del error
  */
  QString errorDescription_;

  /**
  Indica si se están ejecutando tests
  */
  bool testing_;

  /**
  Condición actual que debe cumplirse para continuar con el guión de pruebas
  */
  int condition_;

  /**
  Nombre del objeto sobre el que se aplica la condición actual
  */
  QString objectCondition_;

  /**
  Cola de eventos
  */
  QPtrQueue < FLTesterEvent > * queueTesterEvents;

  /**
  Lista de campos FLFieldDB que pueden ser modificados en un único paso
  */
  QPtrQueue < FLTestFdb > * fdbQueue_;
};

class FL_EXPORT FLEventLoop: public QEventLoop {

  Q_OBJECT

public:

  FLEventLoop();
  virtual bool processEvents( ProcessEventsFlags flags );
  virtual int enterLoop();
  virtual void exitLoop();

private:
  bool aboutToEnter_;
};

#endif
