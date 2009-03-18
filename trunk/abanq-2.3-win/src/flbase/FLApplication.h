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
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
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
Objeto aplicación para FacturaLUX.

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
  Inicializa la aplicación.

  Realiza acciones de inicialización.
  Es indispensable ejecutar este método antes de utilizar las clases de la librería flbase.

  Si se indica el nombre de una acción sólo muestra el formulario maestro asociado a esta como
  única interfaz, sin el selector de módulos para activar otros módulos. Esta forma de inicialización
  es útil para aplicaciones que sólo necesitan un fomulario simple para interactuar, como puede ser
  un tpv o un terminal industrial en una planta de producción. Al cerrar este formulario se cierra
  la aplicación.

  @param n    Nombre de la acción cuyo formulario maestro se desea que sea la única ventana de aplicación
  @param callFunction Nombre de la función que se desea llamar al inicio. Ejemplo; flfactppal.init
  @param arguments  Cadena con los argumentos separados por dos puntos, para la función indicada en callFunction.Ejemplo: 10:cliente2:false
  @param quitAfterCall Si es TRUE sale de la aplicación después de ejecutar la función indicada en callFunction
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
  Devuelve el nombre del objeto al que está asociado un fichero de script.

  @param nFS Nombre del fichero del script (*.qs)
  */
  QString * nameObjectScript( const QString & nFS );

#ifdef FL_TEST
  /**
  Inicia las pruebas automatizadas

  @param codSessionCat: Código de la sesión a lanzar
  @param idFuncional: Identificador de la funcionalidad a la que pertenece la sesión
  @param bd: Nombre de la base de datos que contiene el catálogo de pruebas
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
  Ejecuta una función del script en el contexto de un objeto.

  @param function Nombre de la función.
  @param arguments Argumentos de la función.
  @param context Objeto contexto para el script.
  */
  QSArgument call( const QString & function, const QSArgumentList & arguments = QSArgumentList(), QObject * objectContext = 0 );

  /**
  Ejecuta una función del script en el contexto de un objeto.

  @param function Nombre de la función.
  @param arguments Argumentos de la función.
  @param context Nombre del objeto contexto para el script.
  */
  QSArgument call( const QString & function, const QSArgumentList & arguments = QSArgumentList(), const QString & nameObjectContext = QString::null );

  /**
  Establece el título de la ventana principal.

  El título de la ventana principal siempre tendrá una parte fija establecida
  automáticamente por el motor y se le añadira el texto que se quiera establecer

  @text Texto a establecer en el título de la ventana principal
  */
  void setCaptionMainWidget( const QString & text );

  /**
  Obtiene el último texto establecido con FLApplication::setCaptionMainWidget para el título de la ventana principal
  */
  const QString & lastTextCaption() const;

  /**
  Establece la posibilidad de poder salir o no de la aplicación.
  */
  void setNotExit( const bool & b );

  /**
  Establece que la ejecución es en batch, sin interfaz gráfica de usuario
  */
  void setNoGUI( const bool & b );

  /**
  Obtiene la lista de control de acceso actualmente instalada.
  */
  FLAccessControlLists * acl();

  /**
  Intenta ejecutar la siguiente prueba del guión de pruebas. La prueba se ejecutará si la condición y el objeto indicados son los esperados

  @param c: Condición
  @param oC: Objeto sobre el que se impone la condición
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
  Obtiene el objeto tester asociado a la aplicación
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
  Obtiene si la ejecución es en batch, sin interfaz gráfica de usuario
  */
  bool noGUI() const;

  /**
  Devuelve el nombre de la conexión a base de datos a usar para una accion dada.

  Esta función se puede reimplementar en los scripts para controlar que conexión
  usar en cada momento. Por defecto devuelve una cadena vacía.

  @param actionName Nombre de la acción
  @return Nombre de la conexión a utilizar para la acción
  */
  QString connectionNameForAction( const QString & actionName = QString::null );

  /**
  Establece la funcion de script que reimplementa un método del motor

  @param method Nombre del método a reimplementar
  @param scrFunction Nombre de la función. Ejemplo; flfactppal.init
  */
  void setReimpMethod( const QString & method, const QString & scrFunction );

  /**
  @return True si existe reimplementacion en los scripts para el método indicado
  */
  bool existsReimpMethod( const QString & method ) const;

  /**
  Ejecuta la función de script que reimplementa al método dado
  */
  QSArgument callReimp( const QString & method, const QSArgumentList & arguments = QSArgumentList() );

  /**
  Añade código script al objeto sys, para poder ejecutarlo dinámicamente.

  @param code Código script
  @param scriptEntryFunction Funcion del código del script a ejecutar despues de evaluarlo
  @return TRUE si se añadió el código y este no tiene errores.
  */
  bool addSysCode( const QString & code, const QString & scriptEntryFunction = QString::null );

  /**
  Establece el nombre de la funcion de entrada tras evaluar el código
  */
  void setScriptEntryFunction( const QString & scriptEntryFunction );

  /**
  Obtiene la lista de pestañas que contiene un control QTabWidget

  @param wN Nombre del formulario con el formato [Form|Record]<nombre de la acción>
  @param n Nombre del tabWidget

  @return Cadena que contiene la lista con formato nomPestaña1/descPestaña1*nomPestaña2/descPestaña2*...
  */
  QString getTabWidgetPages( const QString & wN, const QString & n );

  /**
  Obtiene una lista de controles de clase c para el formulario wN

  @param wN Nombre del formulario con el formato [Form|Record]<nombre de la acción>
  @param c Nombre de la clase del objeto que se busca

  @return Cadena que contiene la lista con formato nombreObjeto1/descObjeto1*nomObjeto2/descObj2*...
  */
  QString getWidgetList( const QString & wN, const QString & c );

  /**
  @return Base de datos principal
  */
  FLSqlDatabase * db();

  /**
  Para activar/desactivar la detección de posibles bloqueos de una conexión a base
  de datos sobre el resto.

  Al activar la detección se inicia un cronómetro que periódicamente comprueba bloqueos,
  si existe un bloqueo se emite la señal databaseLockDetected(), y se muestra un mensaje en el
  caso que se indique hacerlo. Al desactivarla se para este cronómetro y por tanto las
  comprobaciones periódicas.

  La detección de bloqueos solo funciona en las bases de datos que lo soportan,
  ver FLSqlDatabase::canDetectLocks().

  @param  on              True activado False desactivado
  @param  msecLapsus      Lapso de tiempo que debe transcurrir entre cada comprobación, por defecto 30 seg.
  @param  limChecks       Numero límite de comprobaciones a realizar, al llegar a este límite el cronómetro
                          para. Si es -1 no hay limite.
  @param  showWarn        True para mostrar un cuadro de diálogo con el aviso de bloqueo
  @param  msgWarn         Texto a mostrar en el diálogo de aviso, si es vacío mostrará uno estándar
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
  Señal emitida cuando se ha detectado un bloqueo en la conexion a la base de datos
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
  Abre en el MDI el formulario maestro del tipo FLFormDB asociado a una acción.

  @param n Nombre de la acción asociada al formulario maestro
  @param p Imagen QPixmap que sirve de icono para el formulario
  */
  void openMasterForm( const QString & n, const QPixmap & p );

  /**
  Ejecuta la función "main" del script "scriptform" de una acción.

  @param n Nombre de la acción
  */
  void execMainScript( const QString & n );

  /**
  Activa un módulo dado.

  El activar un módulo significa mostrarlo en la ventana principal.
  Si es la primera vez que se activa se inicializa, se ejecuta
  la función "init" de su script principal ([idM].qs) y se guarda un puntero
  a su formulario principal ([idM].ui) en FLApplication::dictMainWidgets.
  La segunda activación y sucesiva ya no se ejecutan las inicializaciones,
  y sólo se muestra el formulario principal, recuperándolo de
  FLApplication::dictMainWidgets.

  @param idM Identificador del módulo a activar
  */
  void activateModule( const QString & idM );

  /**
  Inicia/Reinicia la caja de herramientas.
  Tambien reinicializa el menú emergente de módulos.
  */
  void initToolBox();

  /**
  Carga todos los scripts de todos los módulos instalados.

  Reinicializa el proyecto para scripts descargando todos los
  scripts existentes.
  */
  void loadScripts();

  /**
  Carga todas las traducciones de todos los módulos instalados.

  Si una traduccion ya existe para un módulo la actualiza.
  */
  void loadTranslations();

  /**
  Muestra el documento contenido de una página html
  @param url URL de la página html a presentar
  */
  void showDocPage( const QString & url );

  /**
  Reinicializa la aplicación.

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
  Guarda el estado del módulo activo
  */
  void writeStateModule();

  /**
  Lee el estado del módulo activo
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
  Sale de la aplicacion, pidiendo confirmación
  */
  void generalExit();

  /**
  Abre el formulario por defecto para una acción.

  El uso normal es conectar la señal activated() de una accion de FLWidgetApplication.ui
  a este slot. El nombre de esa acción debe tener correspondencia en [id_modulo].xml
  */
  void openDefaultForm();

  /**
  Ejecuta el script por defecto para una acción.

  El uso normal es conectar la señal activated() de una accion de [id_modulo].ui
  a este slot. El nombre de esa acción debe tener correspondencia en [id_modulo].xml.
  De esta manera se ejecutará la función "main" del script indicado en la
  etiqueta "scriptform" de la acción, dentro de [id_modulo].xml
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
  Muestra la venta de ayuda con el índice
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
  Establece un stilo para la apariencia de la aplicación
  */
  void makeStyle( const QString & );

  /**
  Establece el tipo de fuente para la aplicación
  */
  void chooseFont();

  /**
  Muestra el menú emergente de estilos
  */
  void showStyles();

  /**
  Muestra el menú emergente para conmutar barras
  */
  void showToggleBars();

  /**
  Activa el módulo asociado a un FLWidgetAction.

  El único uso permitido, es conectar la señal activated() de FLWidgetAction a este slot.
  Invoca a FLApplication::activateModule(cons QString & idM).
  */
  void activateModule();

  /**
  Reinicializa la aplicación.

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
  Carga los scripts asociados a un módulo.

  @param idM Identificador del módulo
  */
  void loadScriptsFromModule( const QString & idM );

  /**
  Carga la traducción asociada a un módulo.

  Si una traduccion ya existe para un módulo la actualiza.

  @param idM Identificador del módulo
  */
  void loadTranslationFromModule( const QString & idM );

  /**
  Comprueba si una tabla ya está abierta, mediante su formulario, en el MDI.

  Recorre la lista de formularios del MDI, también llamado espacio de trabajo (QWorkspace),
  y comprueba si el formulari ya está abierto, en tal caso devuelve TRUE, y restaura el formulario

  @param id Identificador del  formulario. Corresponde con el nombre de la accion que lo abre
  @return TRUE si el formulario ya está abierto y FALSE en caso contrario
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
  Botón salir
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
  Menu de módulos
  */
  QPopupMenu *modulesMenu;

  /**
  Proyecto para scripts
  */
  QSProject *project_;

  /**
  Diccionario para almacenar los formularios principales de los módulos
  activados.

  En el diccionario se almacenan los punteros a los formularios principales
  de los módulos que se van activando, utilizando como palabra clave
  el identificador de módulo correspondiente.
  */
  QDict < QWidget > * dictMainWidgets;

  /**
  Diccionario para almacenar los nombres de los objetos a los que están
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
  Objeto encargado de realizar las pruebas automatizadas para el módulo 'test'
  */
#ifdef FL_TEST
  FLTester *tester_;
#endif

  /**
  Guarda el último texto establecido con FLApplication::setCaptionMainWidget para el título de la ventana principal
  */
  QString lastTextCaption_;

  /**
  Indica que la aplicación sólo tiene como ventana principal un formulario maestro, ver FLApplication::init()
  */
  bool formAlone_;

  /**
  Indica que no se puede salir de la aplicación por lo métodos normales.
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
  Indica que la ejecución es en batch, sin interfaz gráfica de usuario
  */
  bool noGUI_;

  /**
  Diccionario para almacenar los nombres de las funciones de scripts que
  reimplementan métodos del motor.
  */
  QDict < QString > * dictReimpMethods;

  /**
  Almacena el nombre de la funcion de entrada tras evaluar el código
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
