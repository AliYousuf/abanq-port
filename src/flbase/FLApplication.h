/***************************************************************************
                            FLApplication.h
                          -------------------
 begin                : Mon Jul 2 2001
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

#ifndef FLAPPLICATION_H
#define FLAPPLICATION_H

#include <config.h>

#include <qapplication.h>
#include <qworkspace.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qptrlist.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qstatusbar.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qsignalmapper.h>
#include <qdict.h>
#include <qstylefactory.h>
#include <qstyle.h>
#include <qfontdialog.h>
#include <qmenubar.h>
#include <qsettings.h>
#include <qpainter.h>
#include <qimage.h>
#include <qtimer.h>
#include <qdockarea.h>
#include <qdockwindow.h>
#include <qpushbutton.h>
#include <qsinterpreter.h>
#include <qsproject.h>
#include <qsargument.h>
#include <qtoolbox.h>
#include <qsinputdialogfactory.h>
#include <qsutilfactory.h>
#include <qtextcodec.h>
#include <qtextedit.h>
#include <qprinter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>
#include <qpalette.h>
#include <qmutex.h>
#include <qgroupbox.h>
#include <qnetwork.h>

#ifdef FL_TEST
class FLTester;
#endif
class FLAccessControlLists;
class FLSqlDatabase;
class FLWorkspace;
class FLPopupWarnDbLock;

/**
Objeto aplicaci�n para FacturaLUX.

@author InfoSiAL S.L.
*/
class FL_EXPORT FLApplication: public QApplication {

  Q_OBJECT

public:

  /**
  constructor
  */
  FLApplication( int &argc, char **argv );

  /**
  destructor
  */
  ~FLApplication();

  /**
  Inicializa la aplicaci�n.

  Realiza acciones de inicializaci�n.
  Es indispensable ejecutar este m�todo antes de utilizar las clases de la librer�a flbase.

  Si se indica el nombre de una acci�n s�lo muestra el formulario maestro asociado a esta como
  �nica interfaz, sin el selector de m�dulos para activar otros m�dulos. Esta forma de inicializaci�n
  es �til para aplicaciones que s�lo necesitan un fomulario simple para interactuar, como puede ser
  un tpv o un terminal industrial en una planta de producci�n. Al cerrar este formulario se cierra
  la aplicaci�n.

  @param n    Nombre de la acci�n cuyo formulario maestro se desea que sea la �nica ventana de aplicaci�n
  @param callFunction Nombre de la funci�n que se desea llamar al inicio. Ejemplo; flfactppal.init
  @param arguments  Cadena con los argumentos separados por dos puntos, para la funci�n indicada en callFunction.Ejemplo: 10:cliente2:false
  @param quitAfterCall Si es TRUE sale de la aplicaci�n despu�s de ejecutar la funci�n indicada en callFunction
  */
  void init( const QString & n = QString::null, const QString & callFunction = QString::null,
             const QString & arguments = QString::null, bool quitAfterCall = false, bool noMax = false );

  /**
  Para obtener el espacio de trabajo
  */
  FLWorkspace * workspace() const;

  /**
  Para establecer el widget pricipal.

  @param w Widget principal
  */
  void setMainWidget( QWidget * w );

  /**
  Para mostrar el widget principal del formulario.

  @param w Widget principal
  */
  void showMainWidget( QWidget * w );

  /**
  Para obtener el proyecto de scripts.

  @return Objeto QSProject correspondiente al proyecto para manejar scripts
  */
  QSProject * project() const;

  /**
  Devuelve el nombre del objeto al que est� asociado un fichero de script.

  @param nFS Nombre del fichero del script (*.qs)
  */
  QString * nameObjectScript( const QString & nFS );

#ifdef FL_TEST
  /**
  Inicia las pruebas automatizadas

  @param codSessionCat: C�digo de la sesi�n a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesi�n
  @param bd: Nombre de la base de datos que contiene el cat�logo de pruebas
  */
  void startTest( const QString &codSessionCat, const QString &idFuncional = QString::null, const QString &bd = QString::null );

  /**
  Obtiene la lista de sesiones cargadas en la base de datos de test
  @param bd Nombre de la base de datos de test
  @return Cadena que contiene la lista con formato idfuncional//codsessioncat//description**...
  */
  QString testSessionsList( const QString &bd );
#endif

  /**
  Obtiene el tiempo que inicio sesion el usuario actual.

  @return Regresa el tiempo de inicio de sesion.
  */
  const QDateTime & timeUser() const;

  /**
  Ejecuta una funci�n del script en el contexto de un objeto.

  @param function Nombre de la funci�n.
  @param arguments Argumentos de la funci�n.
  @param context Objeto contexto para el script.
  */
  QSArgument call( const QString & function, const QSArgumentList & arguments = QSArgumentList(), QObject * objectContext = 0 );

  /**
  Ejecuta una funci�n del script en el contexto de un objeto.

  @param function Nombre de la funci�n.
  @param arguments Argumentos de la funci�n.
  @param context Nombre del objeto contexto para el script.
  */
  QSArgument call( const QString & function, const QSArgumentList & arguments = QSArgumentList(), const QString & nameObjectContext = QString::null );

  /**
  Establece el t�tulo de la ventana principal.

  El t�tulo de la ventana principal siempre tendr� una parte fija establecida
  autom�ticamente por el motor y se le a�adira el texto que se quiera establecer

  @text Texto a establecer en el t�tulo de la ventana principal
  */
  void setCaptionMainWidget( const QString & text );

  /**
  Obtiene el �ltimo texto establecido con FLApplication::setCaptionMainWidget para el t�tulo de la ventana principal
  */
  const QString & lastTextCaption() const;

  /**
  Establece la posibilidad de poder salir o no de la aplicaci�n.
  */
  void setNotExit( const bool & b );

  /**
  Establece que la ejecuci�n es en batch, sin interfaz gr�fica de usuario
  */
  void setNoGUI( const bool & b );

  /**
  Obtiene la lista de control de acceso actualmente instalada.
  */
  FLAccessControlLists * acl();

  /**
  Intenta ejecutar la siguiente prueba del gui�n de pruebas. La prueba se ejecutar� si la condici�n y el objeto indicados son los esperados

  @param c: Condici�n
  @param oC: Objeto sobre el que se impone la condici�n
  @return True la prueba se puede ejecutar y se ejecuta correctamente, false en caso contrario
  */
#ifdef FL_TEST
  void continueTesting( const int c, const QString & oC );
#endif

  /**
  Imprime el contenido de un editor de texto ( QTextEdit ).

  @param editor Objeto QTextEdit del que imprimir su contenido
  */
  void printTextEdit( QTextEdit * editor );

  /**
  Obtiene el objeto tester asociado a la aplicaci�n
  @return Objeto tester
  */
#ifdef FL_TEST
  FLTester * tester() {
    return tester_;
  }
#endif

  /**
  Establece la ruta y el nombre del programa a utilizar para imprimir
  */
  void setPrintProgram( const QString & printProgram );

  /**
  Obtiene la ruta y el nombre del programa a utilizar para imprimir
  */
  const QString & printProgram() const;

#ifdef FL_DEBUGGER
  /**
  Abre el entorno integrado de desarrollo de scripts
  */
  void openIDE( QObject * ctx = 0 );
#endif

  /**
  Obtiene si la ejecuci�n es en batch, sin interfaz gr�fica de usuario
  */
  bool noGUI() const;

  /**
  Devuelve el nombre de la conexi�n a base de datos a usar para una accion dada.

  Esta funci�n se puede reimplementar en los scripts para controlar que conexi�n
  usar en cada momento. Por defecto devuelve una cadena vac�a.

  @param actionName Nombre de la acci�n
  @return Nombre de la conexi�n a utilizar para la acci�n
  */
  QString connectionNameForAction( const QString & actionName = QString::null );

  /**
  Establece la funcion de script que reimplementa un m�todo del motor

  @param method Nombre del m�todo a reimplementar
  @param scrFunction Nombre de la funci�n. Ejemplo; flfactppal.init
  */
  void setReimpMethod( const QString & method, const QString & scrFunction );

  /**
  @return True si existe reimplementacion en los scripts para el m�todo indicado
  */
  bool existsReimpMethod( const QString & method ) const;

  /**
  Ejecuta la funci�n de script que reimplementa al m�todo dado
  */
  QSArgument callReimp( const QString & method, const QSArgumentList & arguments = QSArgumentList() );

  /**
  A�ade c�digo script al objeto sys, para poder ejecutarlo din�micamente.

  @param code C�digo script
  @param scriptEntryFunction Funcion del c�digo del script a ejecutar despues de evaluarlo
  @return TRUE si se a�adi� el c�digo y este no tiene errores.
  */
  bool addSysCode( const QString & code, const QString & scriptEntryFunction = QString::null );

  /**
  Establece el nombre de la funcion de entrada tras evaluar el c�digo
  */
  void setScriptEntryFunction( const QString & scriptEntryFunction );

  /**
  Obtiene la lista de pesta�as que contiene un control QTabWidget

  @param wN Nombre del formulario con el formato [Form|Record]<nombre de la acci�n>
  @param n Nombre del tabWidget

  @return Cadena que contiene la lista con formato nomPesta�a1/descPesta�a1*nomPesta�a2/descPesta�a2*...
  */
  QString getTabWidgetPages( const QString & wN, const QString & n );

  /**
  Obtiene una lista de controles de clase c para el formulario wN

  @param wN Nombre del formulario con el formato [Form|Record]<nombre de la acci�n>
  @param c Nombre de la clase del objeto que se busca

  @return Cadena que contiene la lista con formato nombreObjeto1/descObjeto1*nomObjeto2/descObj2*...
  */
  QString getWidgetList( const QString & wN, const QString & c );

  /**
  @return Base de datos principal
  */
  FLSqlDatabase * db();

  /**
  Para activar/desactivar la detecci�n de posibles bloqueos de una conexi�n a base
  de datos sobre el resto.

  Al activar la detecci�n se inicia un cron�metro que peri�dicamente comprueba bloqueos,
  si existe un bloqueo se emite la se�al databaseLockDetected(), y se muestra un mensaje en el
  caso que se indique hacerlo. Al desactivarla se para este cron�metro y por tanto las
  comprobaciones peri�dicas.

  La detecci�n de bloqueos solo funciona en las bases de datos que lo soportan,
  ver FLSqlDatabase::canDetectLocks().

  @param  on              True activado False desactivado
  @param  msecLapsus      Lapso de tiempo que debe transcurrir entre cada comprobaci�n, por defecto 30 seg.
  @param  limChecks       Numero l�mite de comprobaciones a realizar, al llegar a este l�mite el cron�metro
                          para. Si es -1 no hay limite.
  @param  showWarn        True para mostrar un cuadro de di�logo con el aviso de bloqueo
  @param  msgWarn         Texto a mostrar en el di�logo de aviso, si es vac�o mostrar� uno est�ndar
  @param  connectionName  Nombre de la conexion a la base de datos
  */
  void setDatabaseLockDetection( bool on = true, int msecLapsus = 30000, int limChecks = -1,
                                 bool showWarn = true, const QString & msgWarn = QString::null,
                                 const QString & connectionName = "default" );

  /**
  Muestra un mensaje emergente para avisar de bloqueos.

  @param  msgWarn     Texto a mostrar en el aviso
  @param  scriptCalls Diccionario de funciones de scripts y sus argumentos correspondientes a los hiperenlaces
                      incluidos en el mensaje
  */
  void popupWarnDbLocks( const QString & msgWarn, const QMap<QString, QSArgumentList> & scriptCalls );

signals:

  /**
  Se�al emitida cuando se ha detectado un bloqueo en la conexion a la base de datos
  */
  void databaseLockDetected();

public slots:

  /**
  Para confimar salir de la aplicacion
  */
  bool queryExit();

  /**
  Cambia el mensaje de estado en toda la barra de estado temporalmente
  */
  void statusHelpMsg( const QString & text );

  /**
  Abre en el MDI el formulario maestro del tipo FLFormDB asociado a una acci�n.

  @param n Nombre de la acci�n asociada al formulario maestro
  @param p Imagen QPixmap que sirve de icono para el formulario
  */
  void openMasterForm( const QString & n, const QPixmap & p );

  /**
  Ejecuta la funci�n "main" del script "scriptform" de una acci�n.

  @param n Nombre de la acci�n
  */
  void execMainScript( const QString & n );

  /**
  Activa un m�dulo dado.

  El activar un m�dulo significa mostrarlo en la ventana principal.
  Si es la primera vez que se activa se inicializa, se ejecuta
  la funci�n "init" de su script principal ([idM].qs) y se guarda un puntero
  a su formulario principal ([idM].ui) en FLApplication::dictMainWidgets.
  La segunda activaci�n y sucesiva ya no se ejecutan las inicializaciones,
  y s�lo se muestra el formulario principal, recuper�ndolo de
  FLApplication::dictMainWidgets.

  @param idM Identificador del m�dulo a activar
  */
  void activateModule( const QString & idM );

  /**
  Inicia/Reinicia la caja de herramientas.
  Tambien reinicializa el men� emergente de m�dulos.
  */
  void initToolBox();

  /**
  Carga todos los scripts de todos los m�dulos instalados.

  Reinicializa el proyecto para scripts descargando todos los
  scripts existentes.
  */
  void loadScripts();

  /**
  Carga todas las traducciones de todos los m�dulos instalados.

  Si una traduccion ya existe para un m�dulo la actualiza.
  */
  void loadTranslations();

  /**
  Muestra el documento contenido de una p�gina html
  @param url URL de la p�gina html a presentar
  */
  void showDocPage( const QString & url );

  /**
  Reinicializa la aplicaci�n.

  Vuelve a cargar las traducciones, los scripts y reinicializa
  la caja de herramientas.
  */
  void reinit();

protected:

  /**
  Guarda el estado general
  */
  void writeState();

  /**
  Guarda el estado del m�dulo activo
  */
  void writeStateModule();

  /**
  Lee el estado del m�dulo activo
  */
  void readStateModule();

  /**
  Filtro de eventos
  */
  bool eventFilter( QObject * obj, QEvent * ev );

private slots:

  /**
  Lee el estado general
  */
  void readState();

  /**
  Sale de la aplicacion, pidiendo confirmaci�n
  */
  void generalExit();

  /**
  Abre el formulario por defecto para una acci�n.

  El uso normal es conectar la se�al activated() de una accion de FLWidgetApplication.ui
  a este slot. El nombre de esa acci�n debe tener correspondencia en [id_modulo].xml
  */
  void openDefaultForm();

  /**
  Ejecuta el script por defecto para una acci�n.

  El uso normal es conectar la se�al activated() de una accion de [id_modulo].ui
  a este slot. El nombre de esa acci�n debe tener correspondencia en [id_modulo].xml.
  De esta manera se ejecutar� la funci�n "main" del script indicado en la
  etiqueta "scriptform" de la acci�n, dentro de [id_modulo].xml
  */
  void execDefaultScript();

  /**
  Conmuta la barra de herramientas entre visible y oculta
  */
  void toggleToolBar( bool toggle );

  /**
  Conmuta la barra de estado entre visible y oculta
  */
  void toggleStatusBar( bool toggle );

  /**
  Muestra el dialogo "Acerca de"
  */
  void helpAbout();

  /**
  Muestra la venta de ayuda con el �ndice
  */
  void helpIndex();

  /**
  Llamado cuando el menu Ventana es activado; regenera el menu ventana con todas
  las ventanas abiertas
  */
  void windowMenuAboutToShow();

  /**
  Activa el hijo MDI que se ha seleccionado en el menu Ventana
  */
  void windowMenuActivated( int id );

  /**
  Cierra a ventana activa en el espacio de trabajo
  */
  void windowClose();

  /**
  Establece un stilo para la apariencia de la aplicaci�n
  */
  void makeStyle( const QString & );

  /**
  Establece el tipo de fuente para la aplicaci�n
  */
  void chooseFont();

  /**
  Muestra el men� emergente de estilos
  */
  void showStyles();

  /**
  Muestra el men� emergente para conmutar barras
  */
  void showToggleBars();

  /**
  Activa el m�dulo asociado a un FLWidgetAction.

  El �nico uso permitido, es conectar la se�al activated() de FLWidgetAction a este slot.
  Invoca a FLApplication::activateModule(cons QString & idM).
  */
  void activateModule();

  /**
  Reinicializa la aplicaci�n.

  Vuelve a cargar las traducciones, los scripts y reinicializa
  la caja de herramientas.
  */
  void reinitP();

  /**
  Realiza comprobacion para detectar bloqueos en la conexion a la base de datos.
  */
  void checkDatabaseLocks( QObject * timer = 0 );

private:

  /**
  Inicia las acciones
  */
  void initActions();

  /**
  Inicia la barra de menus
  */
  void initMenuBar();

  /**
  Inicia la barra de herramientas
  */
  void initToolBar();

  /**
  Inicia la barra de estado
  */
  void initStatusBar();

  /**
  Inicia el espacio de trabajo
  */
  void initView();

  /**
  Inicia el submenu de estilos
  */
  void initStyles();

  /**
  Inicia el widget principal del formulario
  */
  void initMainWidget();

  /**
  Carga los scripts asociados a un m�dulo.

  @param idM Identificador del m�dulo
  */
  void loadScriptsFromModule( const QString & idM );

  /**
  Carga la traducci�n asociada a un m�dulo.

  Si una traduccion ya existe para un m�dulo la actualiza.

  @param idM Identificador del m�dulo
  */
  void loadTranslationFromModule( const QString & idM );

  /**
  Comprueba si una tabla ya est� abierta, mediante su formulario, en el MDI.

  Recorre la lista de formularios del MDI, tambi�n llamado espacio de trabajo (QWorkspace),
  y comprueba si el formulari ya est� abierto, en tal caso devuelve TRUE, y restaura el formulario

  @param id Identificador del  formulario. Corresponde con el nombre de la accion que lo abre
  @return TRUE si el formulario ya est� abierto y FALSE en caso contrario
  */
  bool existsFormInMDI( const QString & id );

  /**
  Marco MDI frame que engancha los widget hijo MDI, inicializado en FLApplication::initView()
  */
  FLWorkspace *pWorkspace;

  /**
  Widget principal del formulario
  */
  QWidget *mainWidget_;

  /**
  Contenedor principal
  */
  QMainWindow *container;

  /**
  Caja de herramientas
  */
  QToolBox *toolBox;

  /**
  Bot�n salir
  */
  QPushButton *exitButton;

  /**
  Menu ventana
  */
  QPopupMenu *windowMenu;
  QAction *windowCascadeAction;
  QAction *windowTileAction;
  QAction *windowCloseAction;

  /**
  Menu estilos
  */
  QPopupMenu *style;

  /**
  Menu de barras
  */
  QPopupMenu *toggleBars;

  /**
  Menu de m�dulos
  */
  QPopupMenu *modulesMenu;

  /**
  Proyecto para scripts
  */
  QSProject *project_;

  /**
  Diccionario para almacenar los formularios principales de los m�dulos
  activados.

  En el diccionario se almacenan los punteros a los formularios principales
  de los m�dulos que se van activando, utilizando como palabra clave
  el identificador de m�dulo correspondiente.
  */
  QDict < QWidget > * dictMainWidgets;

  /**
  Diccionario para almacenar los nombres de los objetos a los que est�n
  asociados los scripts.

  En el diccionario se almacenan los nombres de los objetos del proyecto de
  scripts, utilizando como palabra clave el nombre de fichero del script (*.qs)
  asociado.
  */
  QDict < QString > * dictObjectsNamesScripts;

  /**
  Tiempo de sesion del usuario
  */
  QDateTime timeUser_;

  /**
  Objeto encargado de realizar las pruebas automatizadas para el m�dulo 'test'
  */
#ifdef FL_TEST
  FLTester *tester_;
#endif

  /**
  Guarda el �ltimo texto establecido con FLApplication::setCaptionMainWidget para el t�tulo de la ventana principal
  */
  QString lastTextCaption_;

  /**
  Indica que la aplicaci�n s�lo tiene como ventana principal un formulario maestro, ver FLApplication::init()
  */
  bool formAlone_;

  /**
  Indica que no se puede salir de la aplicaci�n por lo m�todos normales.
  */
  bool notExit_;

  /**
  Mantiene la lista de control de acceso actualmente instalada
  */
  FLAccessControlLists * acl_;

  /**
  Ruta y nombre del programa a utilizar para imprimir, p.e.: /usr/bin/kprinter
  */
  QString printProgram_;

  /**
  Indica que la ejecuci�n es en batch, sin interfaz gr�fica de usuario
  */
  bool noGUI_;

  /**
  Diccionario para almacenar los nombres de las funciones de scripts que
  reimplementan m�todos del motor.
  */
  QDict < QString > * dictReimpMethods;

  /**
  Almacena el nombre de la funcion de entrada tras evaluar el c�digo
  */
  QString scriptEntryFunction_;

  /**
  Fuerza a leer los ficheros desde un directorio local, en vez desde la base de datos,
  si contiene una ruta valida a un directorio
  */
  QString staticDirFiles_;
  void evaluateStaticScript( QObject * objectContext );

  /**
  Auxiliar para mensajes de bloqueos
  **/
  FLPopupWarnDbLock * popupWarnDbLock_;
};

#endif
