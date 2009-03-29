/***************************************************************************
                          FLApplication.cpp
                         -------------------
begin                : Mon Jul 2 2001
copyright            : (C) 2001-2005 by InfoSiAL, S.L.
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

#include "FLApplication.h"
#include "FLAbout.h"
#include "FLHelpWindow.h"
#include "FLFormDB.h"
#include "FLObjectFactory.h"
#include "FLWidgetAction.h"
#include "FLAction.h"
#include "FLTranslator.h"
#ifdef FL_TEST
#include "FLTester.h"
#endif
#include "FLAccessControlLists.h"
#include "FLSqlDatabase.h"
#include "FLSqlSavePoint.h"
#include "FLSqlConnections.h"
#include "FLDiskCache.h"

// Uso interno
class FL_EXPORT FLWorkspace : public QWorkspace {

public:

  FLWorkspace( QWidget * parent = 0, const char * name = 0 );

protected:

  void paintEvent( QPaintEvent * pe );

private:

  QImage logo;
  QColor fColor, pColor;
};

class FL_EXPORT FLWidget : public QWidget {

public:

  FLWidget( QWidget * parent = 0, const char * name = 0,  WFlags f = 0 );

protected:

  void paintEvent( QPaintEvent * pe );

private:

  QImage logo;
  QColor fColor, pColor;
};

class FL_EXPORT FLTimerDbLock : public QTimer {

public:

  FLTimerDbLock( QObject * parent = 0, const char * name = 0 )
      : QTimer( parent, name ) {}

  int dbLocksLapsus_;
  int dbLocksChecks_;
  bool dbLocksShowWarn_;
  QString dbLocksMsgWarn_;
  QString dbLocksConn_;
};

class FL_EXPORT FLPopupWarnDbLock : public QWhatsThis {

public:

  FLPopupWarnDbLock( QWidget * widget )
      : QWhatsThis( widget ) {}

  bool clicked( const QString & href );
  QMap<QString, QSArgumentList> scriptCalls_;
};

bool FLPopupWarnDbLock::clicked( const QString & href ) {
  if ( !href.isEmpty() )
    ::qt_cast<FLApplication *>( qApp )->call( href.section( '.', 1, 1 ),  scriptCalls_[href], href.section( '.', 0, 0 ) );
  return false;
}

FLApplication::FLApplication( int &argc, char **argv ) :
    QApplication( argc, argv ), pWorkspace( 0 ), mainWidget_( 0 ), toolBox( 0 ),
    toggleBars( 0 ), project_( 0 ), timeUser_( QDateTime::currentDateTime() ),
    lastTextCaption_( QString::null ), formAlone_( false ), notExit_( false ),
    acl_( 0 ), printProgram_( QString::null ), noGUI_( false ) {
#ifdef FL_TEST
  tester_ = 0;
#endif
  container = new QMainWindow( 0 );
  container->setName( "container" );
  container->setIcon( QPixmap::fromMimeSource( "icono_abanq.png" ) );
  container->setCaption( "AbanQ " VERSION );
  popupWarnDbLock_ = new FLPopupWarnDbLock( container );

  dictMainWidgets = new QDict < QWidget >( 37 );

  dictObjectsNamesScripts = new QDict < QString > ( 101 );
  dictObjectsNamesScripts->setAutoDelete( true );

  dictReimpMethods = new QDict < QString > ( 37 );
  dictReimpMethods->setAutoDelete( true );

  timeUser_ = QDateTime::currentDateTime();

  setName( "main" );

  FLSqlConnections * conns = new FLSqlConnections( this, "fluxConns" );
}

FLApplication::~FLApplication() {
#ifdef FL_DEBUG
  qWarning( "*************************************************" );
  qWarning( "FLSqlQuery::countRefQuery" );
  qWarning( "*************************************************" );
  qWarning( QString::number( FLSqlQuery::countRefQuery ) );
  qWarning( "*************************************************" );
  qWarning( "FLSqlQuery::countRefQuery" );
  qWarning( "*************************************************" );

  qWarning( "*************************************************" );
  qWarning( "FLSqlCursor::countRefCursor" );
  qWarning( "*************************************************" );
  qWarning( QString::number( FLSqlCursor::countRefCursor ) );
  qWarning( "*************************************************" );
  qWarning( "FLSqlCursor::countRefCursor" );
  qWarning( "*************************************************" );
#endif
}

bool FLApplication::eventFilter( QObject * obj, QEvent * ev ) {
  if ( !obj || !ev )
    return false;

  if ( formAlone_ ) {
    if ( ev->type() == QEvent::Close ) {
      generalExit();
      return true;
    }
  }

#if defined(Q_OS_WIN32)
  if ( pWorkspace && pWorkspace->activeWindow() && pWorkspace->activeWindow() != obj &&
       ev->type() != QEvent::Resize && ev->type() != QEvent::Close  && obj->isWidgetType() ) {
    obj->removeEventFilter( this );
    if ( obj->inherits( "QMainWindow" ) && ev->type() == QEvent::WindowActivate ) {
      if ( obj == container )
        activateModule( "" );
      else
        activateModule( obj->name() );
    }
    if ( notify( pWorkspace->activeWindow(), ev ) ) {
      obj->installEventFilter( this );
      return true;
    }
    obj->installEventFilter( this );
  }
#endif

  if ( !obj->isWidgetType() || !obj->inherits( "QMainWindow" ) )
    return QApplication::eventFilter( obj, ev );

  if ( obj == container || db()->managerModules()->listAllIdModules().contains( obj->name() ) ) {
    switch ( ev->type() ) {
      case QEvent::KeyPress:
        if ( obj == mainWidget_ ) {
          QKeyEvent * ke = static_cast<QKeyEvent *>( ev );
          if ( ke->key() == Key_Shift && ( ke->state() & ControlButton ) ) {
            activateModule( "" );
            return true;
          }
          if ( ke->key() == Key_Q && ( ke->state() & ControlButton ) ) {
            generalExit();
            return true;
          }
          if ( ke->key() == Key_Escape ) {
            static_cast<QWidget *>( obj )->hide();
            return true;
          }
        }
        break;
      case QEvent::Close:
        if ( obj == container ) {
          generalExit();
          return true;
        } else {
          static_cast<QWidget *>( obj )->hide();
          return true;
        }
        break;
      case QEvent::WindowActivate:
        if ( obj == container ) {
          activateModule( "" );
          return true;
        } else {
          activateModule( obj->name() );
          return true;
        }
        break;
      case QEvent::MouseButtonPress:
        if ( modulesMenu ) {
          QMouseEvent * me = static_cast<QMouseEvent *>( ev );
          if ( me->button() == RightButton ) {
            modulesMenu->popup( QCursor::pos() );
            return true;
          } else
            return false;
        } else
          return false;
        break;
      default:
        return false;
        break;
    }
  }

  return QApplication::eventFilter( obj, ev );
}

void FLApplication::init( const QString & n, const QString & callFunction, const QString & arguments, bool quitAfterCall, bool noMax ) {
  if ( n.isEmpty() || ( !callFunction.isEmpty() && quitAfterCall ) ) {
    windowMenu = new QPopupMenu( container, "windowMenu" );
    windowCascadeAction = new QAction( tr( "Cascada" ), tr( "Cascada" ), QKeySequence(), container );
    windowCascadeAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "cascada.png" ) ) );
    windowCascadeAction->addTo( windowMenu );
    windowTileAction = new QAction( tr( "Mosaico" ), tr( "Mosaico" ), QKeySequence(), container );
    windowTileAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "mosaico.png" ) ) );
    windowTileAction->addTo( windowMenu );
    windowCloseAction = new QAction( tr( "Cerrar" ), tr( "Cerrar" ), QKeySequence(), container );
    windowCloseAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "cerrar.png" ) ) );
    windowCloseAction->addTo( windowMenu );

    modulesMenu = new QPopupMenu( container, "modulesMenu" );
    modulesMenu->setCheckable( false );

    QWidget *w = new QWidget( container, "widgetContainer" );
    QVBoxLayout *vL = new QVBoxLayout( w );
    exitButton = new QPushButton( QPixmap::fromMimeSource( "exit.png" ), tr( "Salir" ), w, "pbSalir" );
    exitButton->setAccel( QKeySequence( tr( "Ctrl+Q" ) ) );
    exitButton->setFocusPolicy( QWidget::NoFocus );
    QToolTip::add( exitButton, tr( "Salir de la aplicación (Ctrl+Q)" ) );
    QWhatsThis::add( exitButton, tr( "Salir de la aplicación (Ctrl+Q)" ) );
    connect( exitButton, SIGNAL( clicked() ), this, SLOT( generalExit() ) );
    toolBox = new QToolBox( w, "toolBox" );
    vL->addWidget( exitButton );
    vL->addWidget( toolBox );
    container->setCentralWidget( w );
  }

  qInitNetworkProtocols();

  project_ = new QSProject();

  db()->manager()->init();
  db()->managerModules()->init();

  if ( n.isEmpty() || ( !callFunction.isEmpty() && quitAfterCall ) ) {
    initStyles();
    initMenuBar();
  }

  db()->manager()->loadTables();
  db()->managerModules()->loadKeyFiles();
  db()->managerModules()->loadAllIdModules();
  db()->managerModules()->loadIdAreas();

  acl_ = new FLAccessControlLists();
  acl_->init();

  loadScripts();
  db()->managerModules()->setShaLocalFromGlobal();
  loadTranslations();

  QSInterpreter *i = project_->interpreter();
  if ( i ) {
    i->addObjectFactory( new FLObjectFactory() );
    i->addObjectFactory( new QSInputDialogFactory() );
    i->addObjectFactory( new QSUtilFactory() );
#ifdef FL_DEBUGGER
    i->setErrorMode( QSInterpreter::AskForDebug );
#else
    i->setErrorMode( QSInterpreter::Notify );
#endif
  }

  if ( !callFunction.isEmpty() ) {
    QStringList argumentList = QStringList::split( ":", arguments, false );
    QSArgumentList arglist;
    for ( QStringList::Iterator it = argumentList.begin(); it != argumentList.end(); ++it ) {
      arglist.append( QSArgument( *it ) );
    }
    call( callFunction, arglist, 0 );
    if ( quitAfterCall ) {
      if ( !db()->driverName().isEmpty() )
        FLVar::clean();
      db()->managerModules()->finish();
      db()->manager()->finish();
      QTimer::singleShot( 2000, this, SLOT( quit() ) );
    }
  }

  if ( !quitAfterCall ) {
    if ( n.isEmpty() ) {
      call( "init", QSArgumentList(), "sys" );
      initToolBox();
      readState();
      container->installEventFilter( this );
    } else {
      FLFormDB *f = new FLFormDB( n, 0, ( noMax ? 0 : Qt::WStyle_Customize ) );
      formAlone_ = true;
      f->installEventFilter( this );
      f->setMainWidget();
      QApplication::setMainWidget( f );
      if ( f->mainWidget() )
        if ( noMax )
          f->show();
        else
          f->showMaximized();
      else
        f->close();
    }
  }
}

#ifdef FL_DEBUGGER
void FLApplication::openIDE( QObject * ctx ) {
  if ( project_ )
    project_->openIde( ctx );
}
#endif

void FLApplication::initMainWidget() {
  if ( !mainWidget_ )
    return ;

  static_cast<QMainWindow *>( mainWidget_ )->menuBar() ->insertItem( tr( "&Ventana" ), windowMenu );
  static_cast<QMainWindow *>( mainWidget_ ) ->setCentralWidget( 0 );

  initView();
  initActions();
  initToolBar();
  initStatusBar();

  readStateModule();
}

void FLApplication::showMainWidget( QWidget * w ) {
  QWidget * focusW = focusWidget();

  if ( w == container || !w ) {
    if ( container->isMinimized() )
      container->showNormal();
    else
      if ( !container->isVisible() ) {
        container->setFont( font() );
        container->show();
      }
    if ( focusW && focusW->isA( "QMainWindow" ) && focusW != container )
      container->setFocus();
    if ( !container->isActiveWindow() ) {
      container->raise();
      container->setActiveWindow();
    }
    container->setCaption( "AbanQ " VERSION );
    return ;
  }

  if ( w->isMinimized() )
    w->showNormal();
  else
    if ( !w->isVisible() ) {
      w->show();
      w->setFont( font() );
    }
  if ( focusW && focusW->isA( "QMainWindow" ) && focusW != w )
    w->setFocus();
  if ( !w->isActiveWindow() ) {
    w->raise();
    w->setActiveWindow();
  }

  QWidget *view_back = static_cast<QMainWindow *>( w ) ->centralWidget();
  if ( view_back ) {
    pWorkspace = static_cast<FLWorkspace *>( view_back->child( w->name(), "QWorkspace" ) );
    view_back->show();
  }

  setCaptionMainWidget( "" );
  QString descripArea = db()->managerModules()->idAreaToDescription( db()->managerModules()->activeIdArea() );
  w->setIcon( db()->managerModules()->iconModule( w->name() ) );
  toolBox->setCurrentItem( static_cast<QToolBar *>( toolBox->child( descripArea, "QToolBar" ) ) );
}

void FLApplication::setMainWidget( QWidget * w ) {
  if ( w == container || !w ) {
    QApplication::setMainWidget( container );
    mainWidget_ = 0;
    return ;
  }

  QApplication::setMainWidget( w );
  mainWidget_ = w;

  QAction *a, *b;
  if ( toggleBars ) {
    a = static_cast<QAction *>( container->child( "agToggleBars" ) ->child( "Herramientas" ) );
    b = static_cast<QAction *>( container->child( "agToggleBars" ) ->child( "Estado" ) );
    QToolBar * tB = static_cast<QToolBar *>( mainWidget_->child( "toolBar" ) );
    if ( tB )
      a->setOn( tB->isShown() );
    b->setOn( static_cast<QMainWindow *>( mainWidget_ )->statusBar()->isShown() );
  }
}

void FLApplication::makeStyle( const QString & style ) {
  QPalette p = palette();
  setStyle( style );
  setPalette( p, true );

  QSettings settings;
  settings.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );

  initToolBox();

  settings.writeEntry( keybase + "estilo", style );
}

void FLApplication::chooseFont() {
  setFont( QFontDialog::getFont( 0, font(), mainWidget() ), true );
  QWidgetList *list = allWidgets();
  QWidgetListIt it( *list );
  QWidget *w;

  while ( ( w = it.current() ) != 0 ) {
    ++it;
    w->setFont( font() );
  }

  QSettings settings;
  settings.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );

  settings.writeEntry( keybase + "fuente/familia", font().family() );
  settings.writeEntry( keybase + "fuente/puntos", font().pointSize() );
  settings.writeEntry( keybase + "fuente/negrita", font().bold() );
  settings.writeEntry( keybase + "fuente/cursiva", font().italic() );
  settings.writeEntry( keybase + "fuente/subrayado", font().underline() );
  settings.writeEntry( keybase + "fuente/tachado", font().strikeOut() );
}

void FLApplication::showStyles() {
  if ( style )
    style->exec( QCursor::pos() );
}

void FLApplication::showToggleBars() {
  if ( toggleBars )
    toggleBars->exec( QCursor::pos() );
}

void FLApplication::initToolBox() {
  if ( !toolBox || !modulesMenu )
    return ;

  modulesMenu->clear();

  while ( toolBox->count() ) {
    QWidget * item = toolBox->item( 0 );
    if ( item ) {
      if ( item->isA( "QToolBar" ) )
        static_cast<QToolBar *>( item ) ->clear();
      toolBox->removeItem( item );
      delete item;
    }
  }

  QStringList listAreas = db()->managerModules()->listIdAreas(), listModules;
  QString descripArea, descripModule;
  QToolBar * newAreaBar;
  FLWidgetAction * newModuleAction;
  QWidget * w;
  int c = 65;


  for ( QStringList::Iterator itA = listAreas.begin(); itA != listAreas.end(); ++itA ) {
    descripArea = db()->managerModules()->idAreaToDescription( *itA );
    newAreaBar = new QToolBar( tr( descripArea ), container, toolBox, false, descripArea );
    newAreaBar->setFrameStyle( QFrame::NoFrame );
    newAreaBar->setOrientation( Qt::Vertical );
    newAreaBar->boxLayout() ->setSpacing( 3 );
    toolBox->addItem( newAreaBar, tr( descripArea ) );

    QActionGroup * ag = new QActionGroup( newAreaBar, descripArea );
    ag->setMenuText( descripArea );
    ag->setUsesDropDown( true );

    listModules = db()->managerModules()->listIdModules( *itA );
    listModules.sort();
    for ( QStringList::Iterator itM = listModules.begin(); itM != listModules.end(); ++itM, ++c ) {
      if ( QChar( c ) == 'Q' )
        ++c;
#if defined (FL_QUICK_CLIENT) //|| (FL_DEBUGGER)
      if ( *itM == "sys" )
        continue;
#endif

      descripModule = QString( QChar( c ) ) + ": " + db()->managerModules()->idModuleToDescription( *itM );
      newModuleAction = new FLWidgetAction( tr( descripModule ), tr( descripModule ),
                                            tr( descripModule ), QKeySequence( QString( "Ctrl+Shift+" ) + QString( QChar( c ) ) ), newAreaBar, ( *itM ) );
      newModuleAction->setIconSet( db()->managerModules()->iconModule( *itM ) );
      newModuleAction->setIdModule( *itM );
      newModuleAction->addTo( newAreaBar );
      ag->add( newModuleAction );
      connect( newModuleAction, SIGNAL( activated() ), this, SLOT( activateModule() ) );
    }

    w = new FLWidget( newAreaBar );
    w->setBackgroundMode( Qt::NoBackground );
    newAreaBar->setStretchableWidget( w );
    ag->addTo( modulesMenu );
  }

  QToolBar *configToolBar = new QToolBar( tr( "Configuracion" ), container, toolBox, false, "Configuracion" );
  configToolBar->setFrameStyle( QFrame::NoFrame );
  configToolBar->setOrientation( Qt::Vertical );
  configToolBar->boxLayout() ->setSpacing( 3 );
  toolBox->addItem( configToolBar, tr( "Configuración" ) );

  FLWidgetAction *fontAction = new FLWidgetAction( tr( "Fuente" ), tr( "Fuente" ), tr( "Fuente" ), QKeySequence(), configToolBar, "fuente" );
  fontAction->setIconSet( QPixmap::fromMimeSource( "font.png" ) );
  connect( fontAction, SIGNAL( activated() ), this, SLOT( chooseFont() ) );
  fontAction->addTo( configToolBar );

  FLWidgetAction *styleAction = new FLWidgetAction( tr( "Estilo" ), tr( "Estilo" ), tr( "Estilo" ), QKeySequence(), configToolBar, "estilo" );
  styleAction->setIconSet( QPixmap::fromMimeSource( "estilo.png" ) );
  styleAction->addTo( configToolBar );
  connect( styleAction, SIGNAL( activated() ), this, SLOT( showStyles() ) );

  w = new FLWidget( configToolBar );
  w->setBackgroundMode( Qt::NoBackground );
  configToolBar->setStretchableWidget( w );

  QToolBar *helpToolBar = new QToolBar( tr( "Ayuda" ), container, toolBox, false, "Ayuda" );
  helpToolBar->setFrameStyle( QFrame::NoFrame );
  helpToolBar->setOrientation( Qt::Vertical );
  helpToolBar->boxLayout() ->setSpacing( 3 );
  toolBox->addItem( helpToolBar, tr( "Ayuda" ) );

  FLWidgetAction *helpIndexAction = new FLWidgetAction( tr( "Ayuda" ), tr( "Índice" ), tr( "Indice" ), QKeySequence(), helpToolBar, "help" );
  helpIndexAction->setIconSet( QPixmap::fromMimeSource( "help_index.png" ) );
  connect( helpIndexAction, SIGNAL( activated() ), this, SLOT( helpIndex() ) );
  helpIndexAction->addTo( helpToolBar );

  FLWidgetAction *aboutAction = new FLWidgetAction( tr( "Ayuda" ), tr( "Acerca de AbanQ" ), tr( "&Acerca de AbanQ" ), QKeySequence(), helpToolBar, "acercaflux" );
  aboutAction->setIconSet( QPixmap::fromMimeSource( "about.png" ) );
  connect( aboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );
  aboutAction->addTo( helpToolBar );

  FLWidgetAction *aboutQt = new FLWidgetAction( tr( "Ayuda" ), tr( "Acerca de Qt" ), tr( "&Acerca Qt" ), QKeySequence(), helpToolBar, "acercaqt" );
  aboutQt->setIconSet( QPixmap::fromMimeSource( "aboutqt.png" ) );
  connect( aboutQt, SIGNAL( activated() ), this, SLOT( aboutQt() ) );
  aboutQt->addTo( helpToolBar );

  w = new FLWidget( helpToolBar );
  w->setBackgroundMode( Qt::NoBackground );
  helpToolBar->setStretchableWidget( w );

  if ( acl_ )
    acl_->process( container );
}

void FLApplication::initActions() {
  connect( windowCascadeAction, SIGNAL( activated() ), pWorkspace, SLOT( cascade() ) );
  connect( windowTileAction, SIGNAL( activated() ), pWorkspace, SLOT( tile() ) );
  connect( windowCloseAction, SIGNAL( activated() ), pWorkspace, SLOT( closeActiveWindow() ) );
}

void FLApplication::initMenuBar() {
  connect( windowMenu, SIGNAL( aboutToShow() ), this, SLOT( windowMenuAboutToShow() ) );
}

void FLApplication::initToolBar() {
  if ( !mainWidget_ )
    return ;

  QObject * obj = mainWidget_->child( "toolBar" );

  if ( !obj )
    return ;

  static_cast<QToolBar *>( obj ) ->setMovingEnabled( false );
  QAction *a, *b;

  if ( obj->child( "automatic what's this? button" ) )
    return ;

  static_cast<QToolBar *>( obj ) ->addSeparator();
  QWhatsThis::whatsThisButton( static_cast<QToolBar *>( obj ) );

  if ( !toggleBars ) {
    toggleBars = new QPopupMenu( container, "toggleBars" );
    toggleBars->setCheckable( true );
    QActionGroup *ag = new QActionGroup( container, "agToggleBars", false );
    a = new QAction( tr( "Barra de Herramientas" ), QKeySequence(), ag, "Herramientas" );
    a->setToggleAction( true );
    a->setOn( true );
    connect( a, SIGNAL( toggled( bool ) ), this, SLOT( toggleToolBar( bool ) ) );
    b = new QAction( tr( "Barra de Estado" ), QKeySequence(), ag, "Estado" );
    b->setToggleAction( true );
    b->setOn( true );
    connect( b, SIGNAL( toggled( bool ) ), this, SLOT( toggleStatusBar( bool ) ) );
    ag->addTo( toggleBars );
  }

  static_cast<QMainWindow *>( mainWidget_ )->menuBar()->insertItem( tr( "&Ver" ), toggleBars );
  static_cast<QMainWindow *>( mainWidget_ )->menuBar()->insertItem( tr( "&Módulos" ), modulesMenu );
}

void FLApplication::initStatusBar() {
  static_cast<QMainWindow *>( mainWidget_ )->statusBar()->message( tr( "Listo." ) );
  static_cast<QMainWindow *>( mainWidget_ )->statusBar()->setSizeGripEnabled( false );
}

void FLApplication::initView() {
  if ( mainWidget_ ) {
    QWidget * view_back = static_cast<QMainWindow *>( mainWidget_ )->centralWidget();
    if ( !view_back ) {
      view_back = new QVBox( mainWidget_ );
      static_cast<QVBox *>( view_back )->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
      pWorkspace = new FLWorkspace( view_back, db()->managerModules()->activeIdModule() );
      pWorkspace->setBackgroundMode( Qt::NoBackground );
      pWorkspace->setScrollBarsEnabled( true );
      static_cast<QMainWindow *>( mainWidget_ )->setCentralWidget( view_back );
    }
  }
}

void FLApplication::initStyles() {
  style = new QPopupMenu( container, "styles" );
  style->setCheckable( true );

  QActionGroup *ag = new QActionGroup( container, 0 );

  ag->setExclusive( true );
  QSignalMapper *styleMapper = new QSignalMapper( container );
  connect( styleMapper, SIGNAL( mapped( const QString & ) ), this, SLOT( makeStyle( const QString & ) ) );

  QStringList list = QStyleFactory::keys();
  list.sort();
  QDict < int >stylesDict( 17, false );

  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    QString styleStr = *it;
    QString styleAccel = styleStr;

    if ( stylesDict[ styleAccel.left( 1 )] ) {
      for ( uint i = 0; i < styleAccel.length(); ++i ) {
        if ( !stylesDict[ styleAccel.mid( i, 1 )] ) {
          stylesDict.insert( styleAccel.mid( i, 1 ), ( const int * ) 1 );
          styleAccel = styleAccel.insert( i, '&' );
          break;
        }
      }
    } else {
      stylesDict.insert( styleAccel.left( 1 ), ( const int * ) 1 );
      styleAccel = "&" + styleAccel;
    }
    QAction *a = new QAction( styleStr, QIconSet(), styleAccel, 0, ag, 0,
                              ag->isExclusive() );
    QSettings settings;
    settings.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
    QString keybase( "/facturalux/lite/" );
#if defined(Q_OS_WIN32)

    if ( styleStr == settings.readEntry( keybase + "estilo", "Bluecurve" ) )
#else

    if ( styleStr == settings.readEntry( keybase + "estilo", "Plastik" ) )
#endif

      a->setOn( true );
    connect( a, SIGNAL( activated() ), styleMapper, SLOT( map() ) );
    styleMapper->setMapping( a, a->text() );
  }
  ag->addTo( style );
}

bool FLApplication::queryExit() {
  if ( notExit_ )
    return false;
  return ( QMessageBox::Yes == QMessageBox::question( mainWidget(), tr( "Salir..." ),
           tr( "¿ Quiere salir de la aplicación ?" ),
           QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape ) );
}

void FLApplication::generalExit() {
  if ( queryExit() ) {
    if ( !formAlone_ ) {
      writeState();
      writeStateModule();
    }
    if ( !db()->driverName().isEmpty() )
      FLVar::clean();
    db()->managerModules()->finish();
    db()->manager()->finish();
    quit();
  }
}

QString FLApplication::getTabWidgetPages( const QString & wN, const QString & n ) {
  QString actionName;
  FLAction *a;
  QWidget *w;
  if ( wN.left( 10 ) == "formRecord" ) {
    actionName = wN.mid( 10, wN.length() );
    a = db()->manager()->action( actionName );
    w = db()->managerModules()->createFormRecord( a, 0, 0, 0 );
  } else
    if ( wN.left( 10 ) == "formSearch" ) {
      actionName = wN.mid( 10, wN.length() );
      a = db()->manager()->action( actionName );
      w = db()->managerModules()->createForm( a, 0, 0, 0 );
    } else {
      actionName = wN.mid( 4, wN.length() );
      a = db()->manager()->action( actionName );
      w = db()->managerModules()->createForm( a, 0, 0, 0 );
    }

  if ( !w )
    return "";

  QTabWidget *tabW = static_cast<QTabWidget * >( w->child( n ) );
  if ( !tabW )
    return "";

  QString s;
  QWidget * page = 0;
  for ( int i = 0; i < tabW->count(); ++i ) {
    page = tabW->page( i );
    s += QString( page->name() ) + "/" + QString( page->caption() ) + "*";
  }
  return s;
}

QString FLApplication::getWidgetList( const QString & wN, const QString & c ) {
  QString actionName;
  FLAction *a;
  QWidget *w;
  if ( wN.left( 10 ) == "formRecord" ) {
    actionName = wN.mid( 10, wN.length() );
    a = db()->manager()->action( actionName );
    w = db()->managerModules()->createFormRecord( a, 0, 0, 0 );
  } else
    if ( wN.left( 10 ) == "formSearch" ) {
      actionName = wN.mid( 10, wN.length() );
      a = db()->manager()->action( actionName );
      w = db()->managerModules()->createForm( a, 0, 0, 0 );
    } else {
      actionName = wN.mid( 4, wN.length() );
      a = db()->manager()->action( actionName );
      w = db()->managerModules()->createForm( a, 0, 0, 0 );
    }

  if ( !w )
    return "";

  const QObjectList * oL = w->queryList( c );
  QObjectListIt it( *oL );
  QObject *o;
  QString s;
  QString name;
  QString fieldTable;

  while ( ( o = it.current() ) != 0 ) {
    ++it;
    name = o->name();
    if ( c == "FLFieldDB" ) {
      fieldTable = static_cast<FLFieldDB *>( o ) ->tableName();
      if ( !fieldTable.isEmpty() && fieldTable != a->table() )
        continue;
      s += name + "/" + static_cast<FLFieldDB *>( o ) ->fieldName() + "*";
    } else
      if ( c == "FLTableDB" ) {
        s += name + "/" + static_cast<FLTableDB *>( o ) ->tableName() + "*";
      } else
        if ( c == "QButton" || c == "Button" ) {
          if ( name == "pushButtonDB" || name == "pbAux" || name == "qt_left_btn" || name == "qt_right_btn" )
            continue;
          s += name + "/" + static_cast<QButton *>( o ) ->text() + "*";
        } else {
          if ( name == "textLabelDB" || name == "componentDB" || name == "tab pages" || name == "editor" || name == "FrameFind" || name == "TextLabelSearch" || name == "TextLabelIn" || name == "lineEditSearch" || name == "in-combo" || name == "voidTable" )
            continue;
          if ( o->isA( "QGroupBox" ) )
            s += name + "/" + ( ( QGroupBox* ) o ) ->title() + "*";
          else
            s += name + "/ " + "*";
        }
  }
  delete o;
  delete oL;
  delete w;
  return s;
}


void FLApplication::toggleToolBar( bool toggle ) {
  if ( mainWidget_ ) {
    if ( toggle == false ) {
      QObject * obj = mainWidget_->child( "toolBar" );
      if ( obj )
        static_cast<QToolBar *>( obj )->hide();
    } else {
      QObject *obj = mainWidget_->child( "toolBar" );
      if ( obj )
        static_cast<QToolBar *>( obj ) ->show();
    }
  }
}

void FLApplication::toggleStatusBar( bool toggle ) {
  if ( mainWidget_ )
    if ( static_cast<QMainWindow *>( mainWidget_ ) ->statusBar() ) {
      if ( toggle == false )
        static_cast<QMainWindow *>( mainWidget_ ) ->statusBar() ->hide();
      else
        static_cast<QMainWindow *>( mainWidget_ ) ->statusBar() ->show();
    }
}

void FLApplication::helpAbout() {
  FLAbout * about = new FLAbout( VERSION, mainWidget() );
  about->exec();
}

void FLApplication::helpIndex() {
  QString index = FLDOC "/index.html";
  FLHelpWindow *help = new FLHelpWindow( index, ".", 0, "help viewer" );
  help->show();
}

void FLApplication::statusHelpMsg( const QString & text ) {
  if ( mainWidget_ )
    if ( static_cast<QMainWindow *>( mainWidget_ ) ->statusBar() )
      static_cast<QMainWindow *>( mainWidget_ ) ->statusBar() ->message( text, 2000 );
}

void FLApplication::windowMenuAboutToShow() {
  windowMenu->clear();
  windowCascadeAction->addTo( windowMenu );
  windowTileAction->addTo( windowMenu );
  windowCloseAction->addTo( windowMenu );

  if ( pWorkspace->windowList().isEmpty() ) {
    windowCascadeAction->setEnabled( false );
    windowTileAction->setEnabled( false );
    windowCloseAction->setEnabled( false );
  } else {
    windowCascadeAction->setEnabled( true );
    windowTileAction->setEnabled( true );
    windowCloseAction->setEnabled( true );
    windowMenu->insertSeparator();
  }

  QWidgetList windows = pWorkspace->windowList();

  for ( int i = 0; i < int( windows.count() ); ++i ) {
    int id = windowMenu->insertItem( windows.at( i ) ->caption(), this, SLOT( windowMenuActivated( int ) ) );
    windowMenu->setItemParameter( id, i );
    windowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at( i ) );
  }
}

void FLApplication::windowMenuActivated( int id ) {
  QWidget * w = pWorkspace->windowList().at( id );

  if ( w )
    w->setFocus();
}

bool FLApplication::existsFormInMDI( const QString & id ) {
  if ( id.isEmpty() || !pWorkspace )
    return false;

  if ( !pWorkspace->windowList().isEmpty() ) {
    QWidgetList windows = pWorkspace->windowList();

    for ( int i = 0; i < int( windows.count() ); ++i ) {
      if ( static_cast<FLFormDB *>( windows.at( i ) ) ->idMDI() == id ) {
        windows.at( i ) ->showNormal();
        windows.at( i ) ->setFocus();
        return true;
      }
    }
  }

  return false;
}

void FLApplication::openMasterForm( const QString & n, const QPixmap & /*p*/ ) {
  if ( existsFormInMDI( n ) ) {
#ifdef FL_TEST
    continueTesting( FLTester::FORM_READY, QString( "form" ) + n );
#endif

    return ;
  }

  bool showMaximized = false;
  if ( pWorkspace->activeWindow() )
    showMaximized = pWorkspace->activeWindow() ->isMaximized();
  FLFormDB *f = new FLFormDB( n, pWorkspace );

  f->setMainWidget();
  if ( f->mainWidget() ) {
    if ( pWorkspace->windowList().isEmpty() || showMaximized )
      f->showMaximized();
    else
      f->show();
#ifdef FL_TEST
    continueTesting( FLTester::FORM_READY, QString( "form" ) + n );
#endif
  } else
    f->close();
}

void FLApplication::openDefaultForm() {
  QObject * s = const_cast<QObject *>( sender() );
  if ( s && !s->inherits( "QAction" ) ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLApplication : El objeto que envia seï¿?ales para abrir formularios debe ser de la clase QAction" ) );
#endif
    return ;
  }
  QPixmap icon = static_cast<QAction *>( s ) ->iconSet().pixmap();
  openMasterForm( s->name(), icon );
}

void FLApplication::execMainScript( const QString & n ) {
  FLAction * a = db()->manager()->action( n );
  if ( !a ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLApplication : No existe la acción %1" ).arg( n ) );
#endif

    return ;
  }
  call( "main", QSArgumentList(), "form" + a->name() );
}

void FLApplication::execDefaultScript() {
  QObject * s = const_cast<QObject *>( sender() );
  if ( s && !s->inherits( "QAction" ) ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLApplication : El objeto que envia seï¿?ales para abrir scripts debe ser de la clase QAction" ) );
#endif
    return ;
  }
  execMainScript( s->name() );
}

void FLApplication::windowClose() {
  if ( !pWorkspace )
    return ;

  pWorkspace->closeActiveWindow();
}

void FLApplication::writeState() {
  if ( container ) {
    QSettings config;
    config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
    QString keybase( "/facturalux/lite/" );

    QStringList windowsOpened;
    QWidgetList * list = topLevelWidgets();
    QWidgetListIt it( *list );
    QWidget * w;
    while ( ( w = it.current() ) != 0 ) {
      ++it;
      if ( w != container && w->isA( "QMainWindow" ) && !w->isHidden() )
        windowsOpened << w->name();
    }
    delete list;
    config.writeEntry( keybase + "windowsOpened/Main", windowsOpened, ',' );

    config.writeEntry( keybase + "Geometry/MainwindowMaximized", container->isMaximized() );
    config.writeEntry( keybase + "Geometry/MainwindowX", container->x() );
    config.writeEntry( keybase + "Geometry/MainwindowY", container->y() );
    config.writeEntry( keybase + "Geometry/MainwindowWidth", container->width() );
    config.writeEntry( keybase + "Geometry/MainwindowHeight", container->height() );
  }
}

void FLApplication::writeStateModule() {
  QString idM = db()->managerModules()->activeIdModule();

  if ( !mainWidget_ || idM.isEmpty() || mainWidget_->name() != idM )
    return ;

  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );

  if ( pWorkspace ) {
    QStringList windowsOpened;
    if ( !pWorkspace->windowList().isEmpty() ) {
      QWidgetList windows = pWorkspace->windowList();

      for ( int i = 0; i < int( windows.count() ); ++i )
        windowsOpened << static_cast<FLFormDB *>( windows.at( i ) ) ->idMDI();
    }
    config.writeEntry( keybase + "windowsOpened/" + idM, windowsOpened, ',' );
  }

  if ( mainWidget_ ) {
    keybase += "Geometry/" + idM;
    config.writeEntry( keybase + "/Maximized", mainWidget_->isMaximized() );
    config.writeEntry( keybase + "/X", mainWidget_->x() );
    config.writeEntry( keybase + "/Y", mainWidget_->y() );
    config.writeEntry( keybase + "/Width", mainWidget_->width() );
    config.writeEntry( keybase + "/Height", mainWidget_->height() );
  }
}

void FLApplication::readState() {
  if ( container ) {
    QSettings config;
    config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
    QString keybase( "/facturalux/lite/" );
    QRect r( container->pos(), container->size() );

    staticDirFiles_ = config.readEntry( keybase + "Modules/staticDirFiles", QString::null );

    if ( !config.
         readBoolEntry( keybase + "Geometry/MainwindowMaximized", false ) ) {
      r.setX( config.readNumEntry( keybase + "Geometry/MainwindowX", r.x() ) );
      r.setY( config.readNumEntry( keybase + "Geometry/MainwindowY", r.y() ) );
      r.setWidth( config.readNumEntry( keybase + "Geometry/MainwindowWidth", r.width() ) );
      r.setHeight( config.readNumEntry( keybase + "Geometry/MainwindowHeight", r.height() ) );

      QRect desk = QApplication::desktop() ->geometry();
      QRect inter = desk.intersect( r );
      container->resize( r.size() );
      if ( inter.width() * inter.height() > ( r.width() * r.height() / 20 ) )
        container->move( r.topLeft() );
    } else
      container->resize( QApplication::desktop() ->geometry().size() );

    QString activeIdModule = db()->managerModules()->activeIdModule();

#if defined (FL_QUICK_CLIENT)
    if ( activeIdModule == "sys" )
      activeIdModule = QString::null;
#endif

    QStringList windowsOpened;
    windowsOpened = config.readListEntry( keybase + "windowsOpened/Main", ',' );

    if ( !windowsOpened.isEmpty() ) {
      QStringList::Iterator it = windowsOpened.end();
      do {
        --it;
#if defined (FL_QUICK_CLIENT)
        if ( ( *it ) == "sys" )
          continue;
#endif
        QWidget * w = 0;
        if ( ( *it ) != activeIdModule && db()->managerModules()->listAllIdModules().contains( *it ) ) {
          w = ( *dictMainWidgets )[ *it ];
          if ( !w ) {
            w = db()->managerModules()->createUI( *it + ".ui", this, 0, *it );
            dictMainWidgets->replace( *it, w );
            setName( *it );
            if ( acl_ )
              acl_->process( w );
            call( "init", QSArgumentList(), *it );
            db()->managerModules()->setActiveIdModule( *it );
            setMainWidget( w );
            initMainWidget();
          }
        }
      } while ( it != windowsOpened.begin() );
    }

    QDictIterator < QWidget > itD( *dictMainWidgets );
    QWidget * w;
    while ( ( w = itD.current() ) != 0 ) {
      ++itD;
      if ( w->name() != activeIdModule ) {
        QWidget * view_back = static_cast<QMainWindow *>( w ) ->centralWidget();
        if ( view_back ) {
          pWorkspace = static_cast<FLWorkspace *>( view_back->child( w->name(), "QWorkspace" ) );
          view_back->show();
        }
        w->show();
        w->setFont( font() );
        w->installEventFilter( this );
      }
    }

    if ( !activeIdModule.isEmpty() ) {
      container->show();
      container->setFont( font() );
    }

    activateModule( activeIdModule );
  }
}

void FLApplication::readStateModule() {
  QString idM = db()->managerModules()->activeIdModule();

#if defined (FL_QUICK_CLIENT)
  if ( idM == "sys" )
    return;
#endif

  if ( !mainWidget_ || idM.isEmpty() || mainWidget_->name() != idM )
    return ;

  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );

  if ( mainWidget_ ) {
    QStringList windowsOpened;
    windowsOpened = config.readListEntry( keybase + "windowsOpened/" + idM, ',' );
    if ( !windowsOpened.isEmpty() ) {
      QAction * act = 0;
      for ( QStringList::Iterator it = windowsOpened.begin();
            it != windowsOpened.end(); ++it ) {
        QString n( *it );
        act = ::qt_cast<QAction *>( mainWidget_->child( n, "QAction" ) );
        if ( act && act->isVisible() )
          openMasterForm( n, act->iconSet().pixmap() );
      }
    }

    QRect r( mainWidget_->pos(), mainWidget_->size() );
    keybase += "Geometry/" + idM;
    if ( !config.
         readBoolEntry( keybase + "/Maximized", false ) ) {
      r.setX( config.readNumEntry( keybase + "/X", r.x() ) );
      r.setY( config.readNumEntry( keybase + "/Y", r.y() ) );
      r.setWidth( config.readNumEntry( keybase + "/Width", r.width() ) );
      r.setHeight( config.readNumEntry( keybase + "/Height", r.height() ) );
      QRect desk = QApplication::desktop() ->geometry();
      QRect inter = desk.intersect( r );
      mainWidget_->resize( r.size() );
      if ( inter.width() * inter.height() > ( r.width() * r.height() / 20 ) )
        mainWidget_->move( r.topLeft() );
    } else
      mainWidget_->resize( QApplication::desktop() ->geometry().size() );
  }
}

void FLApplication::loadScripts() {
  dictObjectsNamesScripts->clear();
  dictReimpMethods->clear();

  QStringList listModules = db()->managerModules()->listAllIdModules();
  uint i = 1;
  for ( QStringList::Iterator it = listModules.begin(); it != listModules.end(); ++it, ++i ) {
    loadScriptsFromModule( *it );
    if ( !( i % 4 ) )
      processEvents();
  }
  processEvents();
}

void FLApplication::loadTranslations() {
  QStringList listModules = db()->managerModules()->listAllIdModules();
  uint i = 1;
  for ( QStringList::Iterator it = listModules.begin(); it != listModules.end(); ++it, ++i ) {
    loadTranslationFromModule( *it );
    if ( !( i % 4 ) )
      processEvents();
  }
  processEvents();
}

void FLApplication::loadTranslationFromModule( const QString & idM ) {
  QString Language( QTextCodec::locale() );
  QString fileTs = idM + "." + Language.left( 2 ) + ".ts";
  QString key = db()->managerModules()->shaOfFile( fileTs );
  QString contentTs;

  if ( !key.isEmpty() ) {
    if ( FLDiskCache::absoluteFilePath( key ).isEmpty() ) {
      contentTs = db()->managerModules()->content( fileTs );
      FLDiskCache::insert( key, contentTs );
    }

    FLTranslator * tor = new FLTranslator( 0 );
    tor->loadTsContent( key );
    installTranslator( tor );
  }
}

void FLApplication::loadScriptsFromModule( const QString & idM ) {
  QDomDocument doc( "scripts" );

  QString actionsXml( db()->managerModules()->contentCached( idM + ".xml" ) );
  if ( idM == "sys" ) {
    QString actionsXmlPlus( db()->managerModules()->contentCached( "plus_sys.xml" ) );
    if ( !actionsXmlPlus.isEmpty() )
      actionsXml.replace( "</ACTIONS>", actionsXmlPlus );
  }

  if ( !FLUtil::domDocumentSetContent( doc, actionsXml ) ) {
#ifdef FL_DEBUG
    qWarning( "FLApplication : " +
              QApplication::tr( "Error al cargar las acciones del módulo %1" ).arg( idM ) );
#endif

    return ;
  }

  QString stream( db()->managerModules()->getScript( idM + ".qs" ) );
  if ( idM == "sys" ) {
    QString streamPlus( db()->managerModules()->getScript( "plus_sys.qs" ) );
    if ( !streamPlus.isEmpty() )
      stream += streamPlus;
  }

  QString stream2( "var form = Application." + idM + "; " + stream );
  FLApplicationInterface * ap = static_cast<FLApplicationInterface *>( project_->object( idM ) );
  if ( !ap ) {
    ap = new FLApplicationInterface( this );
    ap->setName( idM );
    project_->addObject( ap );
    project_->createScript( ap, stream2 );
  } else {
    QSScript *scr = project_->script( ap );
    if ( scr )
      scr->setCode( stream2 );
    else
      project_->createScript( ap, stream2 );
  }
  dictObjectsNamesScripts->replace( idM + ".qs", new QString( idM ) );

  QDomElement docElem = doc.documentElement();
  QDomNode no = docElem.firstChild();
  QString scriptForm, scriptFormRecord, name;
  stream = QString::null;
  stream2 = QString::null;

  while ( !no.isNull() ) {
    QDomElement e = no.toElement();
    if ( !e.isNull() ) {
      if ( e.tagName() == "action" ) {
        QDomNode no2 = e.firstChild();
        scriptForm = scriptFormRecord = name = QString::null;

        while ( !no2.isNull() ) {
          QDomElement e2 = no2.toElement();

          if ( !e2.isNull() ) {
            if ( e2.tagName() == "name" ) {
              name = e2.text();
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "scriptformrecord" ) {
              scriptFormRecord = e2.text() + ".qs";
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "scriptform" ) {
              scriptForm = e2.text() + ".qs";
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "table" ) {
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "form" ) {
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "formrecord" ) {
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "caption" ) {
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "description" ) {
              no2 = no2.nextSibling();
              continue;
            }
            if ( e2.tagName() == "alias" ) {
              no2 = no2.nextSibling();
              continue;
            }

          }
          no2 = no.nextSibling();
        }

        if ( !scriptForm.isEmpty() ) {
          FLFormDBInterface * f = static_cast<FLFormDBInterface *>( project_->object( "form" + name ) );
          if ( !f ) {
            f = new FLFormDBInterface( 0 );
            f->setName( "form" + name );
            project_->addObject( f );
            stream = db()->managerModules()->getScript( scriptForm );
            stream2 = "var form = Application.form" + name + "; " + stream;
            f->setScript( project_->createScript( f, stream2 ) );
          } else {
            QSScript * scr = f->script();
            if ( !scr ) {
              scr = project_->script( f );
              if ( scr )
                f->setScript( scr );
              else {
                stream = db()->managerModules()->getScript( scriptForm );
                stream2 = "var form = Application.form" + name + "; " + stream;
                f->setScript( project_->createScript( f, stream2 ) );
              }
            } else {
              stream = db()->managerModules()->getScript( scriptForm );
              stream2 = "var form = Application.form" + name + "; " + stream;
              if ( QString::compare( scr->code(), stream2 ) )
                scr->setCode( stream2 );
            }
          }
          dictObjectsNamesScripts->replace( scriptForm, new QString( "form" + name ) );

          FLFormSearchDBInterface * fs = static_cast<FLFormSearchDBInterface *>( project_->object( "formSearch" + name ) );
          if ( !fs ) {
            fs = new FLFormSearchDBInterface( 0 );
            fs->setName( "formSearch" + name );
            project_->addObject( fs );
            if ( stream.isEmpty() )
              stream = db()->managerModules()->getScript( scriptForm );
            stream2 = "var form = Application.formSearch" + name + "; " + stream;
            fs->setScript( project_->createScript( fs, stream2 ) );
          } else {
            QSScript * scr = fs->script();
            if ( !scr ) {
              scr = project_->script( fs );
              if ( scr )
                fs->setScript( scr );
              else {
                if ( stream.isEmpty() )
                  stream = db()->managerModules()->getScript( scriptForm );
                stream2 = "var form = Application.formSearch" + name + "; " + stream;
                fs->setScript( project_->createScript( fs, stream2 ) );
              }
            } else {
              if ( stream.isEmpty() )
                stream = db()->managerModules()->getScript( scriptForm );
              stream2 = "var form = Application.formSearch" + name + "; " + stream;
              if ( QString::compare( scr->code(), stream2 ) )
                scr->setCode( stream2 );
            }
          }
        }

        if ( !scriptFormRecord.isEmpty() ) {
          FLFormRecordDBInterface * fr = static_cast<FLFormRecordDBInterface *>( project_->object( "formRecord" + name ) );
          if ( !fr ) {
            fr = new FLFormRecordDBInterface( 0 );
            fr->setName( "formRecord" + name );
            project_->addObject( fr );
            stream = db()->managerModules()->getScript( scriptFormRecord );
            stream2 = "var form = Application.formRecord" + name + "; " + stream;
            fr->setScript( project_->createScript( fr, stream2 ) );
          } else {
            QSScript * scr = fr->script();
            if ( !scr ) {
              scr = project_->script( fr );
              if ( scr )
                fr->setScript( scr );
              else {
                stream = db()->managerModules()->getScript( scriptFormRecord );
                stream2 = "var form = Application.formRecord" + name + "; " + stream;
                fr->setScript( project_->createScript( fr, stream2 ) );
              }
            } else {
              stream = db()->managerModules()->getScript( scriptFormRecord );
              stream2 = "var form = Application.formRecord" + name + "; " + stream;
              if ( QString::compare( scr->code(), stream2 ) )
                scr->setCode( stream2 );
            }
          }
          dictObjectsNamesScripts->replace( scriptFormRecord, new QString( "formRecord" + name ) );
        }

        no = no.nextSibling();
        continue;
      }
    }
    no = no.nextSibling();
  }
}

void FLApplication::activateModule( const QString & idM ) {
#if defined (FL_QUICK_CLIENT)
  if ( idM == "sys" )
    return;
#endif
  writeStateModule();

  QWidget * w = 0;
  if ( db()->managerModules()->listAllIdModules().contains( idM ) ) {
    w = ( *dictMainWidgets )[ idM ];
    if ( !w ) {
      w = db()->managerModules()->createUI( idM + ".ui", this, 0, idM );
      if ( !w )
        return;
      dictMainWidgets->replace( idM, w );
      setName( idM );
      if ( acl_ )
        acl_->process( w );
      call( "init", QSArgumentList(), idM );
      w->removeEventFilter( this );
      db()->managerModules()->setActiveIdModule( idM );
      setMainWidget( w );
      initMainWidget();
      showMainWidget( w );
      w->installEventFilter( this );
      return ;
    } else
      if ( !staticDirFiles_.isEmpty() )
        evaluateStaticScript( project_->object( idM ) );
  }

  if ( !w )
    db()->managerModules()->setActiveIdModule( "" );
  else
    db()->managerModules()->setActiveIdModule( idM );
  setMainWidget( w );
  showMainWidget( w );
}

void FLApplication::activateModule() {
  FLWidgetAction * s = ::qt_cast<FLWidgetAction *>( sender() );
  if ( !s ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLApplication : El objeto que envia señales activar módulos debe ser de la clase FLWidgetAction" ) );
#endif
    return ;
  }
  if ( db()->managerModules()->shaLocal() != db()->managerModules()->shaGlobal() )
    reinit();
  QString idM( s->idModule() );
#if defined (FL_QUICK_CLIENT)
  if ( idM == "sys" )
    return;
#endif
  activateModule( idM );
}

void FLApplication::reinit() {
  container->setDisabled( true );
  QDictIterator < QWidget > itD( *dictMainWidgets );
  QWidget * w;
  while ( ( w = itD.current() ) != 0 ) {
    ++itD;
    w->setDisabled( true );
  }
  QTimer::singleShot( 300, this, SLOT( reinitP() ) );
}

void FLApplication::reinitP() {
  if ( container ) {
    container->setDisabled( true );
    container->removeEventFilter( this );

    QObjectList probjList( project_->objects() );
    QPtrList<QSScript> prscrList( project_->scripts() );

    QSInterpreter *i = project_->interpreter();
    if ( i ) {
      i->stopExecution();
      i->clear();
    }
    project_->clearObjects();

    QPtrListIterator<QObject> itPrObj( probjList );
    QObject * prObj;
    while ( ( prObj = itPrObj() ) != 0 )
      prObj->deleteLater();

    QPtrListIterator<QSScript> itScrObj( prscrList );
    QSScript * scrObj;
    while ( ( scrObj = itScrObj() ) != 0 )
      scrObj->deleteLater();

    writeState();
    writeStateModule();

    db()->managerModules()->finish();
    db()->manager()->finish();

    setMainWidget( 0 );
    db()->managerModules()->setActiveIdModule( "" );

    QDictIterator < QWidget > itD( *dictMainWidgets );
    QWidget * w;
    while ( ( w = itD.current() ) != 0 ) {
      ++itD;
      w->removeEventFilter( this );
      w->hide();
      w->deleteLater();
    }

    dictObjectsNamesScripts->clear();
    dictReimpMethods->clear();
    dictMainWidgets->clear();

    QWidgetList * list = topLevelWidgets();
    if ( list ) {
      QWidgetListIt it( *list );
      while ( ( w = it.current() ) != 0 ) {
        ++it;
        if ( w != container && w->isA( "QMainWindow" ) && w->isHidden() )
          w->deleteLater();
      }
      delete list;
    }

    db()->manager()->init();
    db()->managerModules()->init();
    db()->manager()->cleanupMetaData();

    if ( acl_ )
      acl_->init();

    processEvents();

    loadScripts();

    db()->managerModules()->setShaLocalFromGlobal();

    call( "init", QSArgumentList(), "sys" );
    initToolBox();
    readState();
    container->installEventFilter( this );
    container->setDisabled( false );

    if ( !scriptEntryFunction_.isEmpty() ) {
      call( scriptEntryFunction_, QSArgumentList(), 0 );
      scriptEntryFunction_ = QString::null;
    }
  }
}

QString * FLApplication::nameObjectScript( const QString & nFS ) {
  return ( *dictObjectsNamesScripts )[ nFS ];
}

#ifdef FL_TEST
void FLApplication::startTest( const QString &codSessionCat, const QString &idFuncional, const QString &bd ) {
  if ( tester_ )
    tester_->deleteLater();
  tester_ = new FLTester( );
  tester_->start( codSessionCat, idFuncional, bd );
  QString status = tester_->sessionStatus();
}

QString FLApplication::testSessionsList( const QString &bd ) {
  if ( !tester_ )
    tester_ = new FLTester();
  return tester_->sessionsList( bd );
}
#endif

void FLApplication::showDocPage( const QString & url ) {
  FLHelpWindow * docPage = new FLHelpWindow( url, ".", 0, "help viewer" );
  docPage->show();
}

FLWorkspace * FLApplication::workspace() const {
  return pWorkspace;
}

QSProject * FLApplication::project() const {
  return project_;
}

const QDateTime & FLApplication::timeUser() const {
  return timeUser_;
}

void FLApplication::evaluateStaticScript( QObject * objectContext ) {
  if ( objectContext ) {
    QString scriptFile( objectContext->name() );
    bool evaluate = false;

    if ( scriptFile.startsWith( "form" ) ) {
      scriptFile.remove( 0, 4 );
      if ( scriptFile.startsWith( "Record" ) )
        scriptFile.remove( 0, 6 );
      else
        if ( scriptFile.startsWith( "Search" ) )
          scriptFile.remove( 0, 6 );
      evaluate = true;
    } else
      if ( scriptFile.startsWith( "fl" ) )
        evaluate = true;

    if ( evaluate && !db()->managerModules()->contentStaticDir( scriptFile + ".qs" ).isEmpty() )
      loadScripts();
  }
}

QSArgument FLApplication::call( const QString & function, const QSArgumentList & arguments, QObject * objectContext ) {
  QSArgument ret;
  QSInterpreter *i = project_->interpreter();

  if ( i ) {
    if ( !objectContext ) {
      if ( i->hasFunction( function ) )
        ret = i->call( function, arguments, objectContext );
      else {
        QStringList sL = QStringList::split( ".", function );
        QStringList classes = i->classes( sL[ 0 ] );
        for ( QStringList::Iterator it = classes.begin(); it != classes.end(); ++it ) {
          if ( i->hasFunction( sL[ 0 ] + "." + *it + "." + sL[ 1 ] ) ) {
            ret = i->call( sL[ 0 ] + "." + "iface." + sL[ 1 ], arguments, objectContext );
            break;
          }
        }
      }
    } else {
      QString objName( objectContext->name() );
      if ( i->hasFunction( objName + "." + function ) ) {
        ret = i->call( function, arguments, objectContext );
      } else {
        QStringList classes = i->classes( objectContext );
        for ( QStringList::Iterator it = classes.begin(); it != classes.end(); ++it ) {
          if ( i->hasFunction( objName + "." + *it + "." + function ) ) {
            ret = i->call( "iface." + function, arguments, objectContext );
            break;
          }
        }
      }
    }
  }
  return ret;
}

QSArgument FLApplication::call( const QString & function, const QSArgumentList & arguments, const QString & nameObjectContext ) {
  QSArgument ret;
  QObject *obj = project_->object( nameObjectContext );
  ret = call( function, arguments, obj );
  return ret;
}

#ifdef FL_TEST
void FLApplication::continueTesting( const int c, const QString & oC ) {
  if ( tester_ && tester_->testing() )
    tester_->enqueueTesterEvent( c, oC );
}
#endif

void FLApplication::setCaptionMainWidget( const QString & text ) {
  if ( !text.isEmpty() )
    lastTextCaption_ = text;

  if ( !mainWidget_ )
    return ;

  QString bd = db()->driverNameToDriverAlias( db()->driverName() );
  QString descripArea = db()->managerModules()->idAreaToDescription( db()->managerModules()->activeIdArea() );
  QString descripModule = db()->managerModules()->idModuleToDescription( mainWidget_->name() );
  mainWidget_->setCaption( descripArea + "::" + descripModule + "  - [ " + lastTextCaption_ + " ] - [" + bd + " [*] " + db()->database() + " [*] " + db()->user() + "]" );
}

const QString & FLApplication::lastTextCaption() const {
  return lastTextCaption_;
}

void FLApplication::setNotExit( const bool & b ) {
  notExit_ = b;
}

void FLApplication::setNoGUI( const bool & b ) {
  noGUI_ = b;
}

FLAccessControlLists * FLApplication::acl() {
  return acl_;
}

void FLApplication::printTextEdit( QTextEdit * editor ) {
  if ( !editor || !mainWidget_ )
    return;

  QPrinter printer( QPrinter::ScreenResolution );
  printer.setFullPage( true );
  if ( printer.setup( mainWidget_ ) ) {
    QPainter p( &printer );
    if ( !p.device() )
      return;
    QPaintDeviceMetrics metrics( p.device() );
    int dpiy = metrics.logicalDpiY();
    int margin = ( int )( ( 2 / 2.54 ) * dpiy );
    QRect view( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
    QFont font( editor->QWidget::font() );
    font.setPointSize( 10 );

    QSimpleRichText richText( editor->text(), font, editor->context(), editor->styleSheet(),
                              editor->mimeSourceFactory(), view.height() );
    richText.setWidth( &p, view.width() );
    int page = 1;
    do {
      richText.draw( &p, margin, margin, view, mainWidget_->colorGroup() );
      view.moveBy( 0, view.height() );
      p.translate( 0 , -view.height() );
      p.setFont( font );
      p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
                  view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
      if ( view.top() - margin >= richText.height() )
        break;
      printer.newPage();
      ++page;
    } while ( true );
  }
}

void FLApplication::setPrintProgram( const QString & printProgram ) {
  printProgram_ = printProgram;
}

const QString & FLApplication::printProgram() const {
  return printProgram_;
}

bool FLApplication::noGUI() const {
  return noGUI_;
}

FLSqlDatabase * FLApplication::db() {
  return FLSqlConnections::database();
}

QString FLApplication::connectionNameForAction( const QString & actionName ) {
  if ( existsReimpMethod( "App::connectionNameForAction" ) ) {
    QVariant ret;
    QSArgumentList arglist( actionName );
    ret = callReimp( "App::connectionNameForAction", arglist ).variant();
    if ( ret.isValid() )
      return ret.toString();
  }
  return QString::null;
}

void FLApplication::setReimpMethod( const QString & method, const QString & scrFunction ) {
  dictReimpMethods->replace( method, new QString( scrFunction ) );
}

bool FLApplication::existsReimpMethod( const QString & method ) const {
  QString * scrFunc = ( *dictReimpMethods )[ method ];
  if ( scrFunc )
    return !scrFunc->isEmpty();
  return false;
}

QSArgument FLApplication::callReimp( const QString & method, const QSArgumentList & arguments ) {
  QString * scrFunc = ( *dictReimpMethods )[ method ];
  if ( scrFunc )
    return call( *scrFunc, arguments, 0 );
  return QSArgument();
}

bool FLApplication::addSysCode( const QString & code, const QString & scriptEntryFunction ) {
  if ( code.isEmpty() || !project_->interpreter()->checkSyntax( code ) )
    return false;

  QString sha1( FLUtil::sha1( code ) );
  QSScript * scr = project_->script( "sys" );
  QString scrCode( scr->code() );
  QString addCode = "\n\n// " + sha1 + "\n" + code + "\n";

  if ( !scrCode.contains( sha1 ) )
    scrCode += addCode;

  if ( !project_->interpreter()->checkSyntax( scrCode ) )
    return false;

  db()->managerModules()->setContent( "plus_sys.qs", "sys", addCode );

  if ( !scriptEntryFunction.isEmpty() )
    scriptEntryFunction_ = scriptEntryFunction;

  return true;
}

void FLApplication::setScriptEntryFunction( const QString & scriptEntryFunction ) {
  scriptEntryFunction_ = scriptEntryFunction;
}

void FLApplication::setDatabaseLockDetection( bool on, int msecLapsus, int limChecks,
    bool showWarn, const QString & msgWarn, const QString & connectionName ) {

  QString timerId( "TimerDbLock_" + connectionName );
  FLTimerDbLock * timerDbLock = ::qt_cast<FLTimerDbLock *>( child( timerId, "FLTimerDbLock", false ) );

  if ( timerDbLock ) {
    timerDbLock->stop();
    if ( on ) {
      QString defaultMsgWarn( tr( "<p><img source=\"locked.png\" align=\"right\"><b><u>Bloqueo Detectado</u></b><br><br>"
                                  "Su transacción actual está bloqueando a otros usuarios que han solicitado "
                                  "acceder a datos que acaba de crear o modificar pero que aún no han sido "
                                  "aceptados.<br><br>Si le es posible postponer la operación que está realizando se "
                                  "recomienda guardar los cambios que ha efectuado aceptando todos los formularios "
                                  "y esperar unos segundos antes de continuar." ) );
      QString moreInfo( tr( "<br><br><em><a href=\"sys.statusDbLocksDialog\">Más información</a></em></p>" ) );

      timerDbLock->dbLocksChecks_ = limChecks;
      timerDbLock->dbLocksLapsus_ = ( msecLapsus < 0 ? 0 : msecLapsus );
      timerDbLock->dbLocksShowWarn_ = showWarn;
      timerDbLock->dbLocksMsgWarn_ = ( !msgWarn.isEmpty() ? msgWarn : defaultMsgWarn ) + moreInfo;
      timerDbLock->dbLocksConn_ = connectionName;
      if ( timerDbLock->dbLocksLapsus_ )
        timerDbLock->start( timerDbLock->dbLocksLapsus_ );
      else
        checkDatabaseLocks( timerDbLock );
    } else
      timerDbLock->deleteLater();
  } else
    if ( on ) {
      QString defaultMsgWarn( tr( "<p><img source=\"locked.png\" align=\"right\"><b><u>Bloqueo Detectado</u></b><br><br>"
                                  "Su transacción actual está bloqueando a otros usuarios que han solicitado "
                                  "acceder a datos que acaba de crear o modificar pero que aún no han sido "
                                  "aceptados.<br><br>Si le es posible postponer la operación que está realizando se "
                                  "recomienda guardar los cambios que ha efectuado aceptando todos los formularios "
                                  "y esperar unos segundos antes de continuar." ) );
      QString moreInfo( tr( "<br><br><em><a href=\"sys.statusDbLocksDialog\">Más información</a></em></p>" ) );

      timerDbLock = new FLTimerDbLock( this, timerId );
      timerDbLock->dbLocksChecks_ = limChecks;
      timerDbLock->dbLocksLapsus_ = ( msecLapsus < 0 ? 0 : msecLapsus );
      timerDbLock->dbLocksShowWarn_ = showWarn;
      timerDbLock->dbLocksMsgWarn_ = ( !msgWarn.isEmpty() ? msgWarn : defaultMsgWarn ) + moreInfo;
      timerDbLock->dbLocksConn_ = connectionName;
      connect( timerDbLock, SIGNAL( timeout() ), this, SLOT( checkDatabaseLocks() ) );
      if ( timerDbLock->dbLocksLapsus_ )
        timerDbLock->start( timerDbLock->dbLocksLapsus_ );
      else
        checkDatabaseLocks( timerDbLock );
    }
}

void FLApplication::popupWarnDbLocks( const QString & msgWarn, const QMap<QString, QSArgumentList> & scriptCalls ) {
  popupWarnDbLock_->scriptCalls_ = scriptCalls;
  QWhatsThis::display( msgWarn, container->mapToGlobal( QPoint( 0, 0 ) ), container );
}

void FLApplication::checkDatabaseLocks( QObject * timer ) {
  FLTimerDbLock * timerDbLock = ::qt_cast<FLTimerDbLock *>( timer ? timer : sender() );
  if ( timerDbLock ) {
    timerDbLock->stop();

    FLSqlDatabase * database = FLSqlConnections::database( timerDbLock->dbLocksConn_ );
    bool showed = false;

    if ( database && database->canDetectLocks() ) {
      QStringList locks( database->detectLocks() );
      if ( !locks.isEmpty() || timer ) {
        if ( timerDbLock->dbLocksShowWarn_ ) {
          showed = true;
          QMap<QString, QSArgumentList> scrCall;
          scrCall["sys.statusDbLocksDialog"] = QSArgumentList( locks );
          popupWarnDbLocks( timerDbLock->dbLocksMsgWarn_, scrCall );
        }
        emit databaseLockDetected();
      }
    }
    if ( timerDbLock->dbLocksChecks_ != 0 ) {
      if ( timerDbLock->dbLocksChecks_ > 0 )
        timerDbLock->dbLocksChecks_--;
      if ( showed )
        timerDbLock->start( 60000 );
      else
        timerDbLock->start( timerDbLock->dbLocksLapsus_ );
    } else
      timerDbLock->deleteLater();
  }
}

FLWorkspace::FLWorkspace( QWidget * parent, const char * name ) : QWorkspace( parent, name ) {
  logo = QImage::fromMimeSource( "mini_logo_abanq.png" );
  fColor.setRgb( logo.pixel( 1, 1 ) );
  pColor.setRgb( 164, 170, 180 );
}

void FLWorkspace::paintEvent( QPaintEvent * pe ) {
  QPainter p( this );
  p.fillRect( pe->rect(), fColor );

  int dx = width() - logo.width();
  int dy = height() - logo.height();
  p.drawImage( QPoint( dx, dy ), logo );

  p.setPen( pColor );
  p.setBrush( pColor );
  p.drawRect( dx + 23, 0, 2, dy );

  p.end();

  QWidget::paintEvent( pe );
}

FLWidget::FLWidget( QWidget * parent, const char * name,  WFlags f ) : QWidget( parent, name, f ) {
  logo = QImage::fromMimeSource( "mini_logo_abanq.png" );
  fColor.setRgb( logo.pixel( 1, 1 ) );
  pColor.setRgb( 164, 170, 180 );
}

void FLWidget::paintEvent( QPaintEvent * pe ) {
  QPainter p( this );
  p.fillRect( pe->rect(), fColor );

  int dx = width() - logo.width();
  int dy = height() - logo.height();
  p.drawImage( QPoint( dx, dy ), logo );

  p.setPen( pColor );
  p.setBrush( pColor );
  p.drawRect( dx + 23, 0, 2, dy );

  p.end();

  QWidget::paintEvent( pe );
}
