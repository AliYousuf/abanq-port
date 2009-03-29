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
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
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
  Condici�n actual que debe cumplirse para continuar con el gui�n de pruebas
  */
  int condition;

  /**
  Nombre del objeto sobre el que se aplica la condici�n actual
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
    /** El paso se complet� correctamente */
    OK = 0,
    /** Hubo un error controlado en la ejecuci�n del paso */
    ERR = 1,
    /** El paso debe repetirse por no estar listo todav�a el entorno */
    REPEAT = 2,
    /** Hubo un error de ejecuci�n en el paso */
    TESTER_ERROR = 3,
    /** La ejecuci�n del gui�n debe saltarse la prueba actual*/
    SKIP_TEST = 4,
    /** La ejecuci�n del gui�n debe continuar sin esperar*/
    NEXT = 5
  };

  /**
  Constantes para indicar la condici�n que debe cumplirse para continuar ejecutando el gui�n de pruebas
  */
  enum Condition {
    /** Cualquier hay condici�n */
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
    /** Ninguna condici�n*/
    NONE_IS_VALID
  };


  /**
  Obtiene la lista de sesiones cargadas en la base de datos de test
  @param bd Nombre de la base de datos de test
  @return Cadena que contiene la lista con formato idfuncional//codsessioncat//description**...
  */
  QString sessionsList( const QString &bd );

  /**
  Devuelve el identificador actual de la sesi�n de ejecuci�n de tests
  */
  int currentSession() {
    return currentSession_;
  }

  /**
  Devuelve el estado de la sesi�n actual
  */
  QString sessionStatus() {
    return sessionStatus_;
  }

  /**
  Devuelve la conexi�n a la base de datos utilizada para los tests
  */
  FLSqlDatabase * dbTest() {
    return dbTest_;
  }

  /**
  Obtiene si se est�n ejecutando tests
  */
  bool testing() {
    return testing_;
  }

  /**
  Encola un evento en la cola de eventos para las pruebas

  @param c: Condici�n
  @param oC: Objeto sobre el que se impone la condici�n
  */
  void enqueueTesterEvent( const int c, const QString & oC );

public slots:

  /**
  Inicia sesi�n de tests

  @param codSessionCat: C�digo de la sesi�n a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesi�n
  @param bd: Nombre de la base de datos que contiene el cat�logo de pruebas
  */
  void start( const QString &codSessionCat, const QString &idFuncional = QString::null, const QString &bd = QString::null );

protected:

  /**
  Crea los pasos correspondientes a una sesi�n de pruebas en el gui�n de pasos

  @param codSessionCat: C�digo de la sesi�n a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesi�n
  @return: Identificador de la sesi�n creada, 0 si hay error
  */
  uint createSession( const QString &codSessionCat, const QString &idFuncional = QString::null );

  /**
  A�ade los pasos de un test al gui�n de pasos

  @param idTest; Identificador del test en el gui�n de pasos
  @param idTestCat; Identificador del test en el cat�logo de pasos
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
  Termina la sesi�n de tests.
  */
  void end();

  /**
  Ejecuta un paso del gui�n de pasos

  @return: True si el paso se complet� correctamente y hay m�s pasos que procesar
  */
  bool execTest();

  /**
  Intenta ejecutar la siguiente prueba del gui�n de pruebas. La prueba se ejecutar� si la condici�n y el objeto indicados son los esperados

  @param c: Condici�n
  @param oC: Objeto sobre el que se impone la condici�n
  @return True la prueba se puede ejecutar y se ejecuta correctamente, false en caso contrario
  */
  bool tryContinue( const int c, const QString & oC );

  /**
  Esencialmente hace lo mismo que el m�todo FLTester::tryContinue( const int c, const QString & oC ).

  En este m�todo obtiene la informaci�n de condici�n y objeto de la cola de eventos, ver FLTesterEvent y queueTesterEvents
  */
  bool tryContinue();

private:

  /**
  Lanza un paso del gui�n de pasos

  @param idStepCat: Identificador del paso en el cat�logo de pasos
  @param idStep: Identificador del paso en el gui�n de pruebas
  @return (valor de tipo StepResult)
  */
  int execStep( const QString & idStepCat, const QString & idStep );

  /**
  Cambia el estado de la prueba actual

  @param status: estado de la prueba
  @param errorDescription: Descripci�n del error produducido, en su caso
  */
  void setTestStatus( const QString errorDescription );

  /**
  Cambia el estado de la sesi�n actual

  @param status: estado de la prueba
  */
  void setSessionStatus( const QString );

  /**
  Obtiene el valor resultado de una consulta

  @param n: Nodo XML con los datos de la consulta
  @param v: Variable donde se devolver� el resutado de la consulta (FALSE si la consulta no encuentra registros o el primer valor encontrado)
  @return C�digo de error similar al de execStep
  */
  int getQueryValue( QDomNode n, QVariant &v );

  /**
  Obtiene el valor resultado asociado a un nodo de comparaci�n

  @param n: Nodo XML con los datos del elemento a comparar
  @param v: Variable donde se devolver� el valor del elemento
  @return C�digo de error similar al de execStep
  */
  int getValue( QDomNode n, QVariant &v );

  /**
  Preprocesa el string de par�metros sustituyendo las variables por sus valores

  @param parameters: String de par�metros a procesar
  @return True si el preproceso se completa con �xito, false en caso contrario
  */
  bool processParameters( QString &parameters );

  /**
  Establece la condici�n requerida para continuar con el siguiente paso del gui�n de pruebas

  @param e: Nodo que contiene los atributos que definen la condici�n
  @param defCondition: Condici�n por defecto
  @param defObjectCondition: Objeto asociado a la condici�n por defecto
  */
  void setConditions( QDomElement e, const Condition & defCondition, const QString & defObjectCondition );

  /**
  Realiza una transacci�n a trav�s de la conexi�n de la base de datos de pruebas

  @param qrySql: Sentencia SQL a ejecutar
  @return True si la transacci�n se completa correctamente (se realiza el Commit), false en caso contrario (Rollback)
  */
  bool execTransaction( const QString &qrySql );

  /**
  Asigna valores a variables globales

  @param parameters: Nodo XML con las asignaciones a realizar
  @return True si la asignaci�n se completa correctamente, false en caso contrario 
  */
  bool assignateValues( const QString &parameters );

  /**
  Asigna valores por defecto necesarios para ejecutar una determinada prueba

  @param testName: Nodo XML con los identificadores (identificador y funcionalidad) de la prueba
  @return True si la asignaci�n se completa correctamente, false en caso contrario 
  */
  bool assignateDefaultValues( const QString & testName );

  /**
  Asigna un valor a una variable global, usando la conexi�n de pruebas

  @param name: Nombre de la variable
  @param value: Valor de la variable
  @return True si la asignaci�n se completa correctamente, false en caso contrario 
  */
  bool setVar( const QString &name, const QVariant &value );

  /**
  Obtiene el valor de una variable global, usando la conexi�n de pruebas

  @param name: Nombre de la variable
  @param exists: Indicador de si la variable est� o no definida
  @return Valor de la variable
  */
  QVariant getVar( const QString &name, bool *exists );

  /**
  Elimina una variable global, usando la conexi�n de pruebas

  @param name: Nombre de la variable
  @return True si la eliminaci�n se completa correctamente, false en caso contrario 
  */
  bool delVar( const QString &name );

  /**
  N�mero de fallos tolerados hasta que se para la prueba
  */
  int fallos_;

  /**
  N�mero correspondiente a la prueba anterior
  */
  QString previousTest_;

  /**
  N�mero correspondiente a la prueba actual
  */
  QString currentTest_;

  /**
  N�mero correspondiente a la sesi�n actual
  */
  int currentSession_;

  /**
  Formulario actual sobre el que se realiza la prueba
  */
  FLFormDB *currentForm_;

  /**
  Conexi�n a la base de datos de test.
  */
  FLSqlDatabase * dbTest_;

  /**
  Nombre del m�dulo que se est� probando
  */
  QString currentModule_;

  /**
  Estado de la sesi�n de pruebas actual
  */
  QString sessionStatus_;

  /**
  Descripci�n del error
  */
  QString errorDescription_;

  /**
  Indica si se est�n ejecutando tests
  */
  bool testing_;

  /**
  Condici�n actual que debe cumplirse para continuar con el gui�n de pruebas
  */
  int condition_;

  /**
  Nombre del objeto sobre el que se aplica la condici�n actual
  */
  QString objectCondition_;

  /**
  Cola de eventos
  */
  QPtrQueue < FLTesterEvent > * queueTesterEvents;

  /**
  Lista de campos FLFieldDB que pueden ser modificados en un �nico paso
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
