/***************************************************************************
                             FLFormDB.cpp
                         -------------------
begin                : Sat Jul 28 2001
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

#include "FLFormDB.h"
#include "FLSqlCursor.h"
#include "FLTableMetaData.h"
#include "FLObjectFactory.h"
#include "FLAction.h"
#include "FLManager.h"
#include "FLApplication.h"
#include "FLManagerModules.h"
#include "FLAccessControlLists.h"
#include "FLSqlConnections.h"

FLFormDB::FLFormDB( QWidget * parent, const char * name, WFlags f )
    : QWidget( parent ? parent : static_cast<FLApplication *>( qApp )->mainWidget(), name, f ),
    cursor_( 0 ), layout( 0 ),
    mainWidget_( 0 ), layoutButtons( 0 ),
    pushButtonCancel( 0 ), showed( false ), iface( 0 ),
    oldCursorCtxt( 0 ), isClosing_( false ),
    initFocusWidget_( 0 ) {
}

FLFormDB::FLFormDB( const QString & actionName, QWidget * parent, WFlags f )
    : QWidget( parent ? parent : static_cast<FLApplication *>( qApp )->mainWidget(), actionName, f ),
    layout( 0 ), mainWidget_( 0 ),
    layoutButtons( 0 ), pushButtonCancel( 0 ), showed( false ),
    iface( 0 ), oldCursorCtxt( 0 ),
    isClosing_( false ), initFocusWidget_( 0 ) {
#ifdef FL_DEBUGGER
  pushButtonIDE = 0;
#endif

  setFocusPolicy( QWidget::NoFocus );

  if ( actionName.isEmpty() ) {
    action_ = 0;
#ifdef FL_DEBUG
    qWarning( tr( "FLFormDB : Nombre de acción vacío" ) );
#endif
    return ;
  } else
    action_ = FLSqlConnections::database()->manager()->action( actionName );
  if ( !action_ ) {
#ifdef FL_DEBUG
    qWarning( tr( "FLFormDB : No existe la acción " + actionName ) );
#endif
    return ;
  }
  cursor_ = new FLSqlCursor( action_->table(), true, "default", 0, 0, this );
  name_ = action_->name();

  initForm();
}

FLFormDB::FLFormDB( FLSqlCursor * cursor, const QString & actionName, QWidget * parent, WFlags f )
    : QWidget( parent ? parent : static_cast<FLApplication *>( qApp )->mainWidget(), actionName, f ),
    cursor_( cursor ), layout( 0 ),
    mainWidget_( 0 ), layoutButtons( 0 ),
    pushButtonCancel( 0 ), showed( false ), iface( 0 ),
    oldCursorCtxt( 0 ), isClosing_( false ),
    initFocusWidget_( 0 ) {
#ifdef FL_DEBUGGER
  pushButtonIDE = 0;
#endif

  setFocusPolicy( QWidget::NoFocus );

  if ( actionName.isEmpty() )
    action_ = 0;
  else
    if ( cursor )
      action_ = cursor->db()->manager()->action( actionName );
    else
      action_ = FLSqlConnections::database()->manager()->action( actionName );
  name_ = QString::null;

  initForm();
}

FLFormDB::~FLFormDB() {
  if ( iface )
    iface->setObj( 0 );
}

bool FLFormDB::close() {
  if ( isClosing_ )
    return true;
  isClosing_ = true;
  isClosing_ = QWidget::close();
#ifdef FL_TEST
  if ( isClosing_ ) {
    static_cast<FLApplication *>( qApp )->continueTesting( FLTester::FORM_CLOSED, this->name() );
  }
#endif
}

void FLFormDB::initForm() {
  if ( cursor_ && cursor_->metadata() ) {
    QString caption = " [ " + static_cast<FLApplication *>( qApp )->lastTextCaption() + " ]";
    if ( action_ ) {
      cursor_->setAction( action_ );
      if ( action_->caption() != QString::null )
        setCaption( action_->caption() + caption );
      idMDI_ = action_->name();
    }

    setCaption( cursor_->metadata() ->alias() + caption );
    setName( "form" + action_->name() );
    QSProject * p = static_cast<FLApplication *>( qApp ) ->project();
    iface = static_cast<FLFormDBInterface *>( p->object( name() ) );
    if ( iface ) {
      iface->setObj( this );
      if ( !oldCursorCtxt )
        oldCursorCtxt = cursor_->context();
      cursor_->setContext( iface );
    }
  } else
    setCaption( tr( "No hay metadatos" ) );
}

void FLFormDB::openIde() {
#ifdef FL_DEBUGGER
  static_cast<FLApplication *>( qApp )->openIDE( iface );
#endif
}

void FLFormDB::setMainWidget( QWidget * w ) {
  if ( !cursor_ || !w )
    return ;

  if ( showed ) {
    if ( mainWidget_ && mainWidget_ != w )
      initMainWidget( w );
  } else
    w->hide();

  if ( layout )
    delete layout;

  w->setFont( qApp->font() );
  layout = new QVBoxLayout( this, 2, 3, "vlay" + name_ );

  layout->add( w );

  layoutButtons = new QHBoxLayout( layout, 3, "hlay" + name_ ) ;
  QSpacerItem *spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

  QSize pbSize( 26, 26 );

#ifdef FL_DEBUGGER
  pushButtonIDE = new QPushButton( this, "pushButtonIDE" );
  pushButtonIDE->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                pushButtonIDE->sizePolicy().hasHeightForWidth() ) );
  pushButtonIDE->setMinimumSize( pbSize );
  pushButtonIDE->setMaximumSize( pbSize );
  QPixmap qsa( QPixmap::fromMimeSource( "bug.png" ) );
  pushButtonIDE->setIconSet( qsa );
  pushButtonIDE->setAccel( QKeySequence( Qt::Key_F3 ) );
  QToolTip::add( pushButtonIDE, tr( "Abrir Depurador (F3)" ) );
  QWhatsThis::add( pushButtonIDE, tr( "Abrir Depurador (F3)" ) );
  pushButtonIDE->setFocusPolicy( QWidget::NoFocus );
  layoutButtons->addWidget( pushButtonIDE );
  connect( pushButtonIDE, SIGNAL( clicked() ), this, SLOT( openIde() ) );
#endif

  layoutButtons->addItem( spacer_3 );

  pushButtonCancel = new QPushButton( this, "pushButtonCancel" );
  pushButtonCancel->setSizePolicy( QSizePolicy(( QSizePolicy::SizeType ) 0, ( QSizePolicy::SizeType ) 0, 0, 0,
                                   pushButtonCancel->sizePolicy().hasHeightForWidth() ) );
  pushButtonCancel->setMinimumSize( pbSize );
  pushButtonCancel->setMaximumSize( pbSize );
  QPixmap cancel( QPixmap::fromMimeSource( "button_cancel.png" ) );
  pushButtonCancel->setIconSet( cancel );
  pushButtonCancel->setFocusPolicy( QWidget::NoFocus );
  pushButtonCancel->setAccel( QKeySequence( tr( "Esc" ) ) );
  QToolTip::add( pushButtonCancel, tr( "Cerrar formulario (Esc)" ) );
  QWhatsThis::add( pushButtonCancel, tr( "Cerrar formulario (Esc)" ) );
  layoutButtons->addWidget( pushButtonCancel );
  connect( pushButtonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
  pushButtonCancel->show();

  mainWidget_ = w;
}

void FLFormDB::initScript() {
  if ( iface )
    static_cast<FLApplication *>( qApp ) ->call( "init", QSArgumentList(), iface );
}

void FLFormDB::setMainWidget() {
  if ( !action_ )
    return ;
  if ( cursor_ )
    setMainWidget( cursor_->db()->managerModules()->createForm( action_, this, this ) );
  else
    setMainWidget( FLSqlConnections::database()->managerModules()->createForm( action_, this, this ) );
}


void FLFormDB::setMainWidget( const QString & uiFileName ) {
  if ( cursor_ )
    setMainWidget( cursor_->db()->managerModules()->createUI( uiFileName, this, this ) );
  else
    setMainWidget( FLSqlConnections::database()->managerModules()->createUI( uiFileName, this, this ) );
}

void FLFormDB::closeEvent( QCloseEvent * e ) {
  if ( focusWidget() ) {
    FLFieldDB * fdb = ::qt_cast<FLFieldDB *>( focusWidget()->parentWidget() );
    if ( fdb && fdb->autoComFrame_ && fdb->autoComFrame_->isVisible() ) {
      fdb->autoComFrame_->hide();
      return;
    }
  }

  if ( cursor_ && oldCursorCtxt )
    cursor_->setContext( oldCursorCtxt );
  emit closed();
  QWidget::closeEvent( e );
  deleteLater();
}

void FLFormDB::showEvent( QShowEvent * e ) {
  QWidget::showEvent( e );
  this->showForm();
}

void FLFormDB::showForm() {
  if ( !showed && mainWidget_ ) {
    showed = true;
    initMainWidget();
    this->initScript();
  }
}

void FLFormDB::initMainWidget( QWidget * w ) {
  QWidget *mWidget = w ? w : mainWidget_;
  if ( mWidget ) {
    QObjectList * l =  static_cast<QObject *>( mWidget )->queryList( "FLTableDB" );
    QObjectListIt itt( *l );
    FLTableDB * tdb;
    while (( tdb = static_cast<FLTableDB *>( itt.current() ) ) != 0 ) {
      ++itt;
      tdb->initCursor();
    }
    delete l;

    l = static_cast<QObject *>( mWidget )->queryList( "FLFieldDB" );
    QObjectListIt itf( *l );
    FLFieldDB * fdb;
    while (( fdb = static_cast< FLFieldDB *>( itf.current() ) ) != 0 ) {
      ++itf;
      fdb->initCursor();
      fdb->initEditor();
    }

    initFocusWidget_ = static_cast<QWidget *>( mWidget->child( mWidget->caption() ) );
    if ( initFocusWidget_ )
      initFocusWidget_->setFocus();

    mWidget->show();
    FLAccessControlLists * acl = static_cast<FLApplication *>( qApp ) ->acl();
    if ( acl )
      acl->process( this );
    this->resize( size().expandedTo( mWidget->size() ) );

    if ( !initFocusWidget_ ) {
      itf.toFirst();
      while (( fdb = static_cast< FLFieldDB *>( itf.current() ) ) != 0 ) {
        ++itf;
        if ( fdb->isEnabled() ) {
          initFocusWidget_ = fdb;
          break;
        }
      }
      if ( !initFocusWidget_ )
        initFocusWidget_ = static_cast<QWidget *>( mWidget->focusWidget() );
      if ( initFocusWidget_ )
        initFocusWidget_->setFocus();
    }

    delete l;

    QWidget * focWid = qApp->focusWidget();
    if ( focWid ) {
      QWidget * topWidget = focWid->topLevelWidget();
      if ( topWidget && !topWidget->inherits( "FLFormDB" ) ) {
        QWidget * topWid = focWid->parentWidget();
        while ( topWid && !topWid->inherits( "FLFormDB" ) )
          topWid = topWid->parentWidget();
        topWidget = topWid;
      }
      if ( topWidget != this )
        setFocus();
    } else
      setFocus();
  }
}

void FLFormDB::setCursor( FLSqlCursor * c ) {
  if ( !c )
    return ;
  cursor_ = c;
  if ( iface )
    iface->setObjCursor( c );
}

QImage FLFormDB::snapShot() {
  QPixmap pix = QPixmap::grabWidget( this );
  return pix.convertToImage();
}

void FLFormDB::saveSnapShot( const QString & pathFile ) {
  QFile fi( pathFile );
  if ( !fi.open( IO_WriteOnly ) ) {
#ifdef FL_DEBUG
    qWarning( "FLFormDB : " + tr( "Error I/O al intentar escribir el fichero %1" ).arg( pathFile ) );
#endif
    return ;
  }
  snapShot().save( &fi, "PNG" );
}

void FLFormDB::showForDocument() {
  showed = true;
  mainWidget_->show();
  this->resize( size().expandedTo( mainWidget_->size() ) );
  QWidget::show();
}

void FLFormDB::setMaximized() {
  setWindowState( windowState() | WindowMaximized );
}
