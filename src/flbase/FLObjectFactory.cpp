/***************************************************************************
                         FLObjectFactory.cpp
                         -------------------
begin                : jue abr 23 2003
copyright            : (C) 2003-2008 by InfoSiAL S.L.
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

#include "FLObjectFactory.h"

class FLDomNodesCache {

public:

  FLDomNodesCache()
      : ptrDictNodes( QPtrDict <FLDomNodeInterface>( 277 ) ),
      dictNodes( QDict <FLDomNodeInterface>( 277, false ) ) {
  }

  void clear();

  FLDomNodeInterface * node( const QDomNode & n );
  FLDomNodeInterface * node( const QDomElement & n );
  FLDomNodeInterface * node( const QDomText & n );
  FLDomNodeInterface * node( const QDomDocument & n );
  FLDomNodeInterface * node( const QString & id );

  bool removeNode( const QString & id );
  void insertNode( const QString & id, FLDomNodeInterface * nIface );
  void updateNode( FLDomNodeInterface * nIface );

private:

  QPtrDict <FLDomNodeInterface>  ptrDictNodes;
  QDict    <FLDomNodeInterface>  dictNodes;
};

FL_EXPORT FLDomNodesCache * FLDomNodeInterface::nodesCache = new FLDomNodesCache();
FL_EXPORT uint FLDomNodeInterface::idNodeGlob = 0;

class FLSqlCursorsCache {

public:

  FLSqlCursorsCache()
      : ptrDictCursors( QPtrDict <FLSqlCursorInterface>( 97 ) ) {
  }

  FLSqlCursorInterface * sqlCursor( void * o );
  void remove( void * o );

private:

  QPtrDict <FLSqlCursorInterface>  ptrDictCursors;
};

FL_EXPORT FLSqlCursorsCache * FLSqlCursorInterface::cursorsCache = new FLSqlCursorsCache();

FLObjectFactory::FLObjectFactory() : QSObjectFactory(), util_( 0 ), voidObject_( 0 ) {
  registerClass( "FLSqlCursor", "FLSqlCursorInterface" );
  registerClass( "FLSqlQuery", "FLSqlQueryInterface" );
  registerClass( "FLFormDB", "FLFormDBInterface" );
  registerClass( "FLFormRecordDB", "FLFormRecordDBInterface" );
  registerClass( "FLFormSearchDB", "FLFormSearchDBInterface" );
  registerClass( "FLFieldDB", "FLFieldDBInterface" );
  registerClass( "FLTableDB", "FLTableDBInterface" );
  registerClass( "FLUtil", "FLUtilInterface" );
  registerClass( "FLReportViewer", "FLReportViewerInterface" );
  registerClass( "FLCodBar", "FLCodBarInterface" );
  registerClass( "FLApplication", "FLAplicationInterface" );
  registerClass( "QTable", "FLTableInterface" );
  registerClass( "QListView", "FLListViewInterface" );
  registerClass( "FLListViewItem", "FLListViewInterfaceItem" );
  registerClass( "FLScriptEditor", "FLScriptEditorInterface" );
  registerClass( "QTabWidget", "FLTabWidgetInterface" );
  registerClass( "FLVar", "FLVarInterface" );
  registerClass( "QDomNode", "FLDomNodeInterface" );
  registerClass( "FLDomNode", "FLDomNodeInterface" );
  registerClass( "FLDomElement", "FLDomNodeInterface" );
  registerClass( "FLDomDocument", "FLDomNodeInterface" );
  registerClass( "FLDomText", "FLDomNodeInterface" );
  registerClass( "FLDomNodeList", "FLDomNodeListInterface" );
  registerClass( "FLProcess", "FLProcessInterface" );
  registerClass( "FLPosPrinter", "FLPosPrinterInterface" );
  registerClass( "FLSmtpClient", "FLSmtpClientInterface" );
  registerClass( "FLImageViewer", "FLImageViewerInterface" );
  registerClass( "FLSerialPort", "FLSerialPortInterface" );
  registerClass( "Picture", "FLPicture" );
  registerClass( "FLNetwork", "FLNetwork" );
}

QObject *FLObjectFactory::create( const QString & className, const QSArgumentList & arguments, QObject * context ) {

  if ( className == "FLSqlCursor" ) {
    if ( arguments.count() == 1 )
      return new FLSqlCursorInterface( arguments[ 0 ].variant().toString() );
    if ( arguments.count() == 2 )
      return new FLSqlCursorInterface( arguments[ 0 ].variant().toString(), arguments[ 1 ].variant().toString() );
  }

  if ( className == "FLSqlQuery" ) {
    if ( arguments.count() == 1 )
      return new FLSqlQueryInterface( arguments[ 0 ].variant().toString() );
    if ( arguments.count() == 2 )
      return new FLSqlQueryInterface( arguments[ 0 ].variant().toString(), arguments[ 1 ].variant().toString() );
    return new FLSqlQueryInterface();
  }

  if ( className == "FLFormDB" ) {
    if ( arguments.count() == 3 )
      return new FLFormDBInterface( arguments[ 0 ].variant().toString(),
                                    static_cast<QWidget *>( arguments[ 1 ].qobject() ), arguments[ 2 ].variant().toInt() );
    if ( arguments.count() == 4 )
      return new FLFormDBInterface( static_cast<FLSqlCursorInterface *>( arguments[ 0 ].qobject() ),
                                    arguments[ 1 ].variant().toString(), static_cast<QWidget *>( arguments[ 2 ].qobject() ),
                                    arguments[ 3 ].variant().toInt() );
  }

  if ( className == "FLFormRecordDB" ) {
    if ( arguments.count() == 4 )
      return new FLFormRecordDBInterface( static_cast<FLSqlCursorInterface *>( arguments[ 0 ].qobject() ),
                                          arguments[ 1 ].variant().toString(), static_cast<QWidget *>( arguments[ 2 ].qobject() ),
                                          arguments[ 3 ].variant().toBool() );
  }

  if ( className == "FLFormSearchDB" ) {
    if ( arguments.count() == 1 )
      return new FLFormSearchDBInterface( arguments[ 0 ].variant().toString() );
    if ( arguments.count() == 2 )
      return new FLFormSearchDBInterface( static_cast<FLSqlCursorInterface *>( arguments[ 0 ].qobject() ),
                                          arguments[ 1 ].variant().toString() );
  }

  if ( className == "FLFieldDB" ) {
    if ( arguments.count() == 2 )
      return new FLFieldDBInterface( static_cast<QWidget *>( arguments[ 0 ].qobject() ), arguments[ 1 ].variant().toCString() );
  }

  if ( className == "FLTableDB" ) {
    if ( arguments.count() == 2 )
      return new FLTableDBInterface( static_cast<QWidget *>( arguments[ 0 ].qobject() ), arguments[ 1 ].variant().toCString() );
  }

  if ( className == "QTable" ) {
    if ( arguments.count() == 2 )
      return new FLTableInterface( static_cast<QWidget *>( arguments[ 0 ].qobject() ), arguments[ 1 ].variant().toCString() );
  }

  if ( className == "QListView" ) {
    if ( arguments.count() == 2 )
      return new FLListViewInterface( static_cast<QWidget *>( arguments[ 0 ].qobject() ), arguments[ 1 ].variant().toCString() );
  }

  if ( className == "FLListViewItem" ) {
    if ( arguments.count() == 1 ) {
      QObject * o = arguments[ 0 ].qobject();
      if ( o->isA( "FLListViewItemInterface" ) )
        return new FLListViewItemInterface( static_cast<FLListViewItemInterface *>( arguments[ 0 ].qobject() ) );
      else
        return new FLListViewItemInterface( static_cast<FLListViewInterface *>( arguments[ 0 ].qobject() ) );
    }
  }

  if ( className == "QTabWidget" ) {
    return new FLTabWidgetInterface();
  }

  if ( className == "FLUtil" ) {
    if ( !util_ )
      util_ = new FLUtilInterface( qApp );
    return util_;
  }

  if ( className == "FLReportViewer" ) {
    if ( arguments.count() == 1 ) {
      QWidget * w = ::qt_cast<QWidget *>( arguments[ 0 ].qobject() );
      if ( w )
        return new FLReportViewerInterface( w, true );
    }
    return new FLReportViewerInterface();
  }

  if ( className == "FLScriptEditor" ) {
    return new FLScriptEditorInterface( arguments[ 0 ].variant().toString() );
  }

  if ( className == "FLCodBar" ) {
    if ( arguments.count() == 1 ) {
      return new FLCodBarInterface( static_cast<FLCodBar *>( arguments[ 0 ].ptr() ) );
    } else {
      return new FLCodBarInterface( arguments[ 0 ].variant().toString(), arguments[ 1 ].variant().toInt(),
                                    arguments[ 2 ].variant().toInt(), arguments[ 3 ].variant().toDouble(),
                                    arguments[ 4 ].variant().toDouble(), arguments[ 5 ].variant().toInt(),
                                    arguments[ 6 ].variant().toBool() );
    }
  }

  if ( className == "FLApplication" ) {
    return new FLApplicationInterface( static_cast<FLApplication *>( arguments[ 0 ].qobject() ) );
  }

  if ( className == "FLVar" ) {
    return new FLVarInterface();
  }

  if ( className == "QDomNode" ) {
    return new FLDomNodeInterface();
  }

  if ( className == "FLDomNode" ) {
    return new FLDomNodeInterface();
  }

  if ( className == "FLDomDocument" ) {
    return new FLDomNodeInterface( QDomNode::DocumentNode );
  }

  if ( className == "FLDomElement" ) {
    return new FLDomNodeInterface( QDomNode::ElementNode );
  }

  if ( className == "FLDomText" ) {
    return new FLDomNodeInterface( QDomNode::TextNode );
  }

  if ( className == "FLDomNodeList" ) {
    return new FLDomNodeListInterface();
  }

  if ( className == "FLProcess" ) {
    return new FLProcessInterface( arguments[ 0 ].variant().toString() );
  }

  if ( className == "FLPosPrinter" ) {
    return new FLPosPrinterInterface();
  }

  if ( className == "FLSmtpClient" ) {
    return new FLSmtpClientInterface();
  }

  if ( className == "FLImageViewer" ) {
    return new FLImageViewerInterface();
  }

  if ( className == "FLSerialPort" ) {
    return new FLSerialPortInterface( arguments[ 0 ].variant().toString() );
  }

  if ( className == "Picture" ) {
    if ( arguments.count() == 1 ) {
      FLPicture * pic = ::qt_cast<FLPicture *>( arguments[ 0 ].qobject() );
      if ( pic )
        return new FLPicture( pic );
    }
    return new FLPicture();
  }

  if ( className == "FLNetwork" )
    return new FLNetwork( arguments[ 0 ].variant().toString() );

  if ( !voidObject_ )
    voidObject_ = new QObject( qApp, "voidObject" );
  return voidObject_;
}

void FLDomNodesCache::clear() {
  ptrDictNodes.clear();
  dictNodes.clear();
}

FLDomNodeInterface * FLDomNodesCache::node( const QDomNode & n ) {
  void * impl = n.internalImpl();
  if ( !impl )
    return 0;
  FLDomNodeInterface * nodeIface = ptrDictNodes.find( impl );
  if ( !nodeIface ) {
    FLDomNodeInterface * parentNodeIface = node( n.parentNode() );
    if ( parentNodeIface && parentNodeIface->type() != QDomNode::DocumentNode )
      nodeIface = new FLDomNodeInterface( n, parentNodeIface );
    else
      nodeIface = new FLDomNodeInterface( n );
    ptrDictNodes.insert( n.internalImpl(), nodeIface );
  } else if ( nodeIface->isNull() )
    nodeIface->updateNode( n );
  return nodeIface;
}

FLDomNodeInterface * FLDomNodesCache::node( const QDomElement & n ) {
  void * impl = n.internalImpl();
  if ( !impl )
    return 0;
  FLDomNodeInterface * nodeIface = ptrDictNodes.find( impl );
  if ( !nodeIface ) {
    nodeIface = new FLDomNodeInterface( n );
    ptrDictNodes.insert( n.internalImpl(), nodeIface );
  } else if ( nodeIface->isNull() || nodeIface->type() != QDomNode::ElementNode )
    nodeIface->updateNode( n );
  return nodeIface;
}

FLDomNodeInterface * FLDomNodesCache::node( const QDomText & n ) {
  void * impl = n.internalImpl();
  if ( !impl )
    return 0;
  FLDomNodeInterface * nodeIface = ptrDictNodes.find( impl );
  if ( !nodeIface ) {
    nodeIface = new FLDomNodeInterface( n );
    ptrDictNodes.insert( n.internalImpl(), nodeIface );
  } else if ( nodeIface->isNull() || nodeIface->type() != QDomNode::TextNode )
    nodeIface->updateNode( n );
  return nodeIface;
}

FLDomNodeInterface * FLDomNodesCache::node( const QDomDocument & n ) {
  void * impl = n.internalImpl();
  if ( !impl )
    return 0;
  FLDomNodeInterface * nodeIface = ptrDictNodes.find( impl );
  if ( !nodeIface ) {
    nodeIface = new FLDomNodeInterface( n );
    ptrDictNodes.insert( n.internalImpl(), nodeIface );
  } else if ( nodeIface->isNull() || nodeIface->type() != QDomNode::DocumentNode )
    nodeIface->updateNode( n );
  return nodeIface;
}

FLDomNodeInterface * FLDomNodesCache::node( const QString & id ) {
  return dictNodes.find( id );
}

void FLDomNodesCache::insertNode( const QString & id, FLDomNodeInterface * nIface ) {
  if ( nIface )
    dictNodes.insert( id, nIface );
}

void FLDomNodesCache::updateNode( FLDomNodeInterface * nIface ) {
  if ( nIface ) {
    QDomNode * n = nIface->ptroObj();
    if ( n ) {
      void * nImpl = n->internalImpl();
      if ( nImpl && ptrDictNodes.find( nImpl ) != nIface )
        ptrDictNodes.insert( nImpl, nIface );
    }
  }
}

bool FLDomNodesCache::removeNode( const QString & id ) {
  FLDomNodeInterface * nIface = dictNodes.find( id );
  if ( nIface ) {
    QDomNode * n = nIface->ptroObj();
    void * impl;
    if ( n && ( impl = n->internalImpl() ) ) {
      ptrDictNodes.remove( impl );
      QDomNodeList nl( n->childNodes() );
      FLDomNodeInterface * nodeIface;
      for ( uint i = 0; i < nl.count(); ++i ) {
        nodeIface = ptrDictNodes.find( nl.item( i ).internalImpl() );
        if ( nodeIface )
          nodeIface->deleteLater();
      }
    }
    //qWarning( "REMOVE count nods %d ptrnos %d remove " + id, dictNodes.count() - 1, ptrDictNodes.count() );
    return dictNodes.remove( id );
  }
  return false;
}

FLDomNodeInterface::~FLDomNodeInterface() {
  //qWarning( "FLDomNodeInterface::~FLDomNodeInterface() %d", idNode_ );
  nodesCache->removeNode( QString::number( idNode_ ) );
  clear();
}

FLDomNodeInterface * FLDomNodeInterface::nodeFromIdNode( const QString & id ) const {
  return nodesCache->node( id );
}

FLDomNodeInterface * FLDomNodeInterface::nodeInterface( const QDomNode & n ) {
  return nodesCache->node( n );
}

FLDomNodeInterface * FLDomNodeInterface::nodeInterface( const QDomElement & n ) {
  return nodesCache->node( n );
}

FLDomNodeInterface * FLDomNodeInterface::nodeInterface( const QDomText & n ) {
  return nodesCache->node( n );
}

FLDomNodeInterface * FLDomNodeInterface::nodeInterface( const QDomDocument & n ) {
  return nodesCache->node( n );
}

QDomNode * FLDomNodeInterface::newNode() {
  clear();
  switch ( type_ ) {
    case QDomNode::ElementNode:
      dn_ = new QDomElement();
      break;
    case QDomNode::TextNode:
      dn_ = new QDomText();
      break;
    case QDomNode::DocumentNode:
      dn_ = new QDomDocument();
      break;
    default:
      dn_ = new QDomNode();
  }
  return dn_;
}

void FLDomNodeInterface::updateNode( const QDomNode & n ) {
  if ( !dn_ )
    *dn_ = n;
  else
    dn_ = new QDomNode( n );
}

void FLDomNodeInterface::updateNode( const QDomElement & n ) {
  if ( dn_ ) {
    if ( type_ != QDomNode::ElementNode ) {
      delete dn_;
      dn_ = new QDomElement( n );
    } else
      *dn_ = n;
  } else
    dn_ = new QDomElement( n );
  type_ = QDomNode::ElementNode;
}

void FLDomNodeInterface::updateNode( const QDomText & n ) {
  if ( dn_ ) {
    if ( type_ != QDomNode::TextNode ) {
      delete dn_;
      dn_ = new QDomText( n );
    } else
      *dn_ = n;
  } else
    dn_ = new QDomText( n );
  type_ = QDomNode::TextNode;
}

void FLDomNodeInterface::updateNode( const QDomDocument & n ) {
  if ( dn_ ) {
    if ( type_ != QDomNode::DocumentNode ) {
      delete dn_;
      dn_ = new QDomDocument( n );
    } else
      *dn_ = n;
  } else
    dn_ = new QDomDocument( n );
  type_ = QDomNode::DocumentNode;
}

void FLDomNodeInterface::setIdNode() {
  idNode_ = idNodeGlob++;
  nodesCache->insertNode( QString::number( idNode_ ), this );
}

bool FLDomNodeInterface::isEqualNode( FLDomNodeInterface * n ) {
  if ( !n )
    return false;
  QDomDocument d1;
  QDomDocument d2;
  QString content = "<dd/>";
  d1.setContent( content );
  d2.setContent( content );
  d1.firstChild().appendChild( DN_->cloneNode() );
  d2.firstChild().appendChild( n->obj().cloneNode() );
  return ( d1.toString() == d2.toString() );
}

QString FLDomNodeInterface::comment() const {
  if ( !dn_ )
    return QString::null;
  QDomNodeList nl( dn_->childNodes() );
  uint ncount = nl.count();
  for ( uint i = 0; i < ncount; ++i ) {
    if ( nl.item( i ).isComment() )
      return nl.item( i ).nodeValue();
  }
  return QString::null;
}

bool FLDomNodeInterface::setContent( const QString & buffer ) {
  if ( type_ != QDomNode::DocumentNode )
    return false;
  bool ret = FLUtil::domDocumentSetContent( *( static_cast<QDomDocument *>( DN_ ) ), buffer );
  nodesCache->updateNode( this );
  return ret;
}

void FLDomNodeInterface::cleanup() {
}

void FLDomNodeInterface::clear() {
  if ( dn_ ) {
    delete dn_;
    dn_ = 0;
  }
}

void FLListViewInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( doubleClicked( QListViewItem * ) ), this, SLOT( emitDoubleClicked( QListViewItem * ) ) );
  connect( obj_, SIGNAL( expanded( QListViewItem * ) ), this, SLOT( emitExpanded( QListViewItem * ) ) );
  connect( obj_, SIGNAL( selectionChanged( QListViewItem * ) ), this, SLOT( emitSelectionChanged( QListViewItem * ) ) );
}

void FLListViewInterface::emitDoubleClicked( QListViewItem * lvI ) {
  FLListViewItemInterface * iface = new FLListViewItemInterface( static_cast<FLListViewItem *>( lvI ) );
  emit doubleClicked( iface );
}

void FLListViewInterface::emitExpanded( QListViewItem * lvI ) {
  FLListViewItemInterface * iface = new FLListViewItemInterface( static_cast< FLListViewItem *>( lvI ) );
  emit expanded( iface );
}

void FLListViewInterface::emitSelectionChanged( QListViewItem * lvI ) {
  FLListViewItemInterface * iface = new FLListViewItemInterface( static_cast<FLListViewItem *>( lvI ) );
  emit selectionChanged( iface );
}

FLListViewItemInterface * FLListViewInterface::firstChild() const {
  FLListViewItemInterface * lvI = new FLListViewItemInterface( static_cast<FLListViewItem *>( obj_->firstChild() ) );
  if ( lvI->obj() )
    return lvI;
  else
    return 0;
}

FLListViewItemInterface * FLListViewItemInterface::firstChild() const {
  FLListViewItemInterface * lvI = new FLListViewItemInterface( static_cast<FLListViewItem *>( obj_->firstChild() ) );
  if ( lvI->obj() )
    return lvI;
  else
    return 0;
}

FLListViewItemInterface * FLListViewItemInterface::nextSibling() const {
  FLListViewItemInterface * lvI = new FLListViewItemInterface( static_cast<FLListViewItem *>( obj_->nextSibling() ) );
  if ( lvI->obj() )
    return lvI;
  else
    return 0;
}

FLListViewItemInterface * FLListViewItemInterface::parent() const {
  FLListViewItemInterface * lvI = new FLListViewItemInterface( static_cast<FLListViewItem *>( obj_->parent() ) );
  if ( lvI->obj() )
    return lvI;
  else
    return 0;
}

void FLListViewItemInterface::setPixmap( int column, const QVariant & pm ) {

  QCString cs;
  if ( pm.isValid() && !pm.isNull() )
    cs = pm.toCString();
  if ( cs.isEmpty() )
    return ;

  QPixmap pix;
  if ( !QPixmapCache::find( cs.left( 100 ), pix ) ) {
    pix.loadFromData( cs );
    QPixmapCache::insert( cs.left( 100 ), pix );
  }
  if ( !pix.isNull() )
    obj_->setPixmap( column, pix );
}

QString FLApplicationInterface::dialogGetFileImage() {
  QFileDialog * fd = new QFileDialog( qApp->focusWidget(), 0, true );
  FLPixmapView * p = new FLPixmapView( fd );

  p->setAutoScaled( true );
  fd->setContentsPreviewEnabled( TRUE );
  fd->setContentsPreview( p, p );
  fd->setPreviewMode( QFileDialog::Contents );
  fd->setCaption( tr( "Elegir archivo" ) );
  fd->setFilter( "*" );

  QString filename;
  if ( fd->exec() == QDialog::Accepted )
    filename = fd->selectedFile();
  fd->deleteLater();
  return filename;
}

QPixmap FLApplicationInterface::toPixmap( const QVariant & v ) {
  QPixmap pix;
  QCString cs = v.toCString();
  if ( !cs.isEmpty() ) {
    if ( !QPixmapCache::find( cs.left( 100 ), pix ) ) {
      pix.loadFromData( cs );
      QPixmapCache::insert( cs.left( 100 ), pix );
    }
  }
  return pix;
}

QVariant FLApplicationInterface::fromPixmap( const QPixmap & p ) {
  if ( p.isNull() )
    return QVariant();

  QApplication::setOverrideCursor( waitCursor );

  QCString s;
  QBuffer buffer( s );

  buffer.open( IO_WriteOnly );
  p.save( &buffer, "XPM" );
  if ( !s.isEmpty() ) {
    if ( !QPixmapCache::find( s.left( 100 ) ) )
      QPixmapCache::insert( s.left( 100 ), p );
  }

  QApplication::restoreOverrideCursor();

  return QVariant( s );
}

void FLSqlCursorInterface::setObj( FLSqlCursor * obj ) {
  obj_ = obj;
  if ( obj_ ) {
    if ( !obj_->metadata() ) {
      qWarning( "FLObjectFactory : Imposible inicializar interface de FLSqlCursor, no existen metadatos." );
      return ;
    }
    if ( obj_->cursorRelation() )
      cursorRelation_ = sqlCursorInterface( obj_->cursorRelation() );
    connects();
  }
}

FLSqlCursorInterface * FLSqlCursorsCache::sqlCursor( void * o ) {
  FLSqlCursorInterface * c = ptrDictCursors.find( o );
  if ( !c ) {
    c = new FLSqlCursorInterface( static_cast<QObject *>( o ) );
    ptrDictCursors.insert( o, c );
  }
  return c;
}

void FLSqlCursorsCache::remove( void * o ) {
  ptrDictCursors.remove( o );
}

FLSqlCursorInterface * FLSqlCursorInterface::sqlCursorInterface( FLSqlCursor * c ) {
  if ( c ) {
    FLSqlCursorInterface * ci = cursorsCache->sqlCursor( c );
    return ci;
  }
  return new FLSqlCursorInterface();
}

void FLSqlCursorInterface::removeCache() {
  if ( obj_ )
    cursorsCache->remove( obj_ );
}

void FLSqlCursorInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( newBuffer() ), this, SLOT( emitNewBuffer() ) );
  connect( obj_, SIGNAL( bufferChanged( const QString & ) ), this, SLOT( emitBufferChanged( const QString & ) ) );
  connect( obj_, SIGNAL( cursorUpdated() ), this, SLOT( emitCursorUpdated() ) );
  connect( obj_, SIGNAL( recordChoosed() ), this, SLOT( emitRecordChoosed() ) );
  connect( obj_, SIGNAL( currentChanged( int ) ), this, SLOT( emitCurrentChanged( int ) ) );
  connect( obj_, SIGNAL( autoCommit() ), this, SLOT( emitAutoCommit() ) );
  connect( obj_, SIGNAL( bufferCommited() ), this, SLOT( emitBufferCommited() ) );
}

QObject * FLFormDBInterface::child( const char * objName ) {
  if ( !obj_ )
    return 0;
  QObject * o = obj_->child( objName );
  if ( o ) {
    if ( o->isA( "FLFieldDB" ) ) {
      return new FLFieldDBInterface( static_cast<FLFieldDB *>( o ) );
    }
    if ( o->isA( "FLTableDB" ) ) {
      return new FLTableDBInterface( static_cast<FLTableDB *>( o ) );
    }
    if ( o->isA( "QTable" ) ) {
      return new FLTableInterface( static_cast<QTable *>( o ) );
    }
    if ( o->isA( "QTabWidget" ) ) {
      return new FLTabWidgetInterface( static_cast<QTabWidget *>( o ) );
    }
    if ( o->isA( "QListView" ) ) {
      return new FLListViewInterface( static_cast<QListView *>( o ) );
    }
  }
  return o;
}

void FLFormDBInterface::connects() const {
  disconnect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
  connect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
}

QObject * FLFormRecordDBInterface::child( const char * objName ) {
  if ( !obj_ )
    return 0;
  QObject * o = obj_->child( objName );
  if ( o ) {
    if ( childrenIface ) {
      QObject * oc = childrenIface->find( o );
      if ( oc )
        return oc;
    } else
      childrenIface = new QPtrDict<QObject>();
    if ( o->isA( "FLFieldDB" ) ) {
      FLFieldDBInterface * iface = new FLFieldDBInterface( static_cast<FLFieldDB *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "FLTableDB" ) ) {
      FLTableDBInterface * iface = new FLTableDBInterface( static_cast<FLTableDB *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QTable" ) ) {
      FLTableInterface * iface = new FLTableInterface( static_cast<QTable *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QTabWidget" ) ) {
      FLTabWidgetInterface * iface = new FLTabWidgetInterface( static_cast<QTabWidget *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QListView" ) ) {
      FLListViewInterface * iface = new FLListViewInterface( static_cast<QListView *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
  }
  return o;
}

void FLFormRecordDBInterface::connects() const {
  disconnect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
  connect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
}

QObject * FLFormSearchDBInterface::child( const char * objName ) {
  if ( !obj_ )
    return 0;
  QObject * o = obj_->child( objName );
  if ( o ) {
    if ( childrenIface ) {
      QObject * oc = childrenIface->find( o );
      if ( oc )
        return oc;
    } else
      childrenIface = new QPtrDict<QObject>();
    if ( o->isA( "FLFieldDB" ) ) {
      FLFieldDBInterface * iface = new FLFieldDBInterface( static_cast<FLFieldDB *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "FLTableDB" ) ) {
      FLTableDBInterface * iface = new FLTableDBInterface( static_cast<FLTableDB *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QTable" ) ) {
      FLTableInterface * iface = new FLTableInterface( static_cast<QTable *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QTabWidget" ) ) {
      FLTabWidgetInterface * iface = new FLTabWidgetInterface( static_cast<QTabWidget *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
    if ( o->isA( "QListView" ) ) {
      FLListViewInterface * iface = new FLListViewInterface( static_cast<QListView *>( o ) );
      childrenIface->insert( o, iface );
      return iface;
    }
  }
  return o;
}

void FLFormSearchDBInterface::connects() const {
  disconnect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
  connect( obj_, SIGNAL( closed() ), this, SLOT( emitClosed() ) );
}

void FLFieldDBInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( activatedAccel( int ) ), this, SLOT( emitActivatedAccel( int ) ) );
}

QStringList FLTableDBInterface::primarysKeysChecked() {
  FLDataTable * dt = obj_->tableRecords();
  QStringList retList;
  if ( dt ) {
    QValueList<QVariant> valueList( dt->primarysKeysChecked() );
    for ( QValueList<QVariant>::const_iterator it = valueList.begin(); it != valueList.end(); ++it )
      retList.append(( *it ).toString() );
  }
  return retList;
}

void FLTableDBInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( readOnlyChanged( bool ) ), this, SLOT( emitReadOnlyChanged( bool ) ) );
  connect( obj_, SIGNAL( editOnlyChanged( bool ) ), this, SLOT( emitEditOnlyChanged( bool ) ) );
  connect( obj_, SIGNAL( insertOnlyChanged( bool ) ), this, SLOT( emitInsertOnlyChanged( bool ) ) );
  connect( obj_, SIGNAL( currentChanged() ), this, SLOT( emitCurrentChanged() ) );
  FLDataTable * dt = obj_->tableRecords();
  if ( dt ) {
    disconnect( dt, SIGNAL( primaryKeyToggled( const QVariant &, bool ) ), this, SLOT( emitPrimaryKeyToggled( const QVariant &, bool ) ) );
    connect( dt, SIGNAL( primaryKeyToggled( const QVariant &, bool ) ), this, SLOT( emitPrimaryKeyToggled( const QVariant &, bool ) ) );
  }
}

QStringList FLTableInterface::selectedRows() {
  NEW_TABLE

  QStringList listRows;
  QTableSelection ts;
  int row;

  for ( int numSel = 0; numSel < obj_->numSelections(); numSel++ ) {

    ts = obj_->selection( numSel );

    for ( row = ts.topRow(); row <= ts.bottomRow(); row++ ) {
      if ( listRows.isEmpty() )
        listRows = QString::number( row );
      else
        listRows.append( QString::number( row ) );
    }
  }

  listRows.sort();
  return listRows;
}

QStringList FLTableInterface::selectedCells() {
  NEW_TABLE

  QStringList listCells;
  QTableSelection ts;
  int row, col;

  for ( int numSel = 0; numSel < obj_->numSelections(); numSel++ ) {

    ts = obj_->selection( numSel );

    for ( row = ts.topRow(); row <= ts.bottomRow(); row++ ) {
      for ( col = ts.leftCol(); col <= ts.rightCol(); col++ ) {

        if ( listCells.isEmpty() )
          listCells = QString::number( row ) + "." + QString::number( col );
        else
          listCells.append( QString::number( row ) + "." + QString::number( col ) );
      }
    }
  }

  listCells.sort();
  return listCells;
}

void FLTableInterface::clear() {
  NEW_TABLE
  QMemArray<int> filas( obj_->numRows() );
  for ( int fila = 0; fila < obj_->numRows(); fila++ )
    filas[fila] = fila;
  obj_->removeRows( filas );
}

void FLTableInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( currentChanged( int, int ) ), this, SLOT( emitCurrentChanged( int, int ) ) );
  connect( obj_, SIGNAL( clicked( int, int, int, const QPoint & ) ), this, SLOT( emitClicked( int, int, int, const QPoint & ) ) );
  connect( obj_, SIGNAL( doubleClicked( int, int, int, const QPoint & ) ), this, SLOT( emitDoubleClicked( int, int, int, const QPoint & ) ) );
  connect( obj_, SIGNAL( valueChanged( int, int ) ), this, SLOT( emitValueChanged( int, int ) ) );
}

void FLTabWidgetInterface::setTabEnabled( const QString & name, const bool enable ) {
  QWidget * w = 0;
  for ( int i = 0; i < obj_->count(); i++ ) {
    w = obj_->page( i );
    if ( w->name() == name ) {
      obj_->setTabEnabled( w, enable );
      break;
    }
  }
}

void FLTabWidgetInterface::showPage( const QString & name ) {
  QWidget * w = 0;
  for ( int i = 0; i < obj_->count(); i++ ) {
    w = obj_->page( i );
    if ( w->name() == name ) {
      obj_->showPage( w );
      break;
    }
  }
}

void FLTabWidgetInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( currentChanged( QWidget * ) ), this, SLOT( emitCurrentChanged( QWidget * ) ) );
}

QString FLUtilInterface::fieldNameToAlias( const QString & fN, const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return fN;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return fN;
  QString ret = mtd->fieldNameToAlias( fN );
  return ret;
}

QString FLUtilInterface::tableNameToAlias( const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return QString::null;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return QString::null;
  QString ret = mtd->alias();
  return ret;
}

QString FLUtilInterface::fieldAliasToName( const QString & aN, const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return aN;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return aN;
  QString ret = mtd->fieldAliasToName( aN );
  return ret;
}

int FLUtilInterface::fieldType( const QString & fN, const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return QVariant::Invalid;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return QVariant::Invalid;
  int ret = mtd->fieldType( fN );
  return ret;
}

int FLUtilInterface::fieldLength( const QString & fN, const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return 0;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return 0;
  else {
    int ret = mtd->fieldLength( fN );
    return ret;
  }
}

bool FLUtilInterface::fieldAllowNull( const QString & fN, const QString & tN, const QString & connName ) {
  if ( tN.isEmpty() )
    return 0;
  FLTableMetaData * mtd = FLSqlConnections::database( connName )->manager()->metadata( tN );
  if ( !mtd )
    return 0;
  bool ret = mtd->fieldAllowNull( fN );
  return ret;
}

void FLSmtpClientInterface::connects() const {
  disconnect( obj_, 0, this, 0 );
  connect( obj_, SIGNAL( status( const QString & ) ), this, SLOT( emitStatus( const QString & ) ) );
  connect( obj_, SIGNAL( sendStarted() ), this, SLOT( emitSendStarted() ) );
  connect( obj_, SIGNAL( sendEnded() ), this, SLOT( emitSendEnded() ) );
  connect( obj_, SIGNAL( sendTotalSteps( int ) ), this, SLOT( emitSendTotalSteps( int ) ) );
  connect( obj_, SIGNAL( sendStepNumber( int ) ), this, SLOT( emitSendStepNumber( int ) ) );
}
