/***************************************************************************
           mlabelobject.cpp  -  Kugar report label object
           -------------------
 begin     : Wed Aug 11 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
copyright : (C) 2002-2003 InfoSiAL S.L.
email     : mail@infosial.com
***************************************************************************/

#include <qapplication.h>
#include <qpaintdevicemetrics.h>

#include "FLObjectFactory.h"

#include "mlabelobject.h"

/** Constructor */
MLabelObject::MLabelObject() : MReportObject(), xMargin( 0 ), yMargin( 0 ) {
  // Set the default label text
  text = "";

  // Set the default font
  fontFamily = "times";
  fontSize = 10;
  fontWeight = MLabelObject::Normal;
  fontItalic = false;

  // Set the default alignment
  hAlignment = MLabelObject::Left;
  vAlignment = MLabelObject::Top;
  wordWrap = false;
  labelFunction = QString::null;
  changeHeight = false;
  paintFunction = QString::null;
  pixmap = 0;
}

/** Copy constructor */
MLabelObject::MLabelObject( const MLabelObject & mLabelObject ) : MReportObject(( MReportObject & ) mLabelObject ), xMargin( 0 ), yMargin( 0 ) {
  copy( &mLabelObject );
}

/** Assignment operator */
MLabelObject MLabelObject::operator =( const MLabelObject & mLabelObject ) {
  if ( &mLabelObject == this )
    return * this;

  // Copy the derived class's data
  copy( &mLabelObject );

  // Copy the base class's data
  (( MReportObject & ) * this ) = mLabelObject;

  return *this;
}

/** Destructor */
MLabelObject::~MLabelObject() {
  if ( pixmap )
    delete pixmap;
}

/** Sets the label's text string */
void MLabelObject::setText( const QString txt ) {
  if ( labelFunction.isEmpty() ) {
    text = txt;
  } else {
    FLDomNodeInterface * dni = 0;
    QSArgumentList l;
    l << QVariant( txt );
    if ( !domNodeData.isNull() ) {
      dni = new FLDomNodeInterface( domNodeData );
      l << dni;
    }
    QVariant v = (( FLApplication * ) qApp ) ->call( labelFunction, l, 0 ).variant();
    if ( v.isValid() )
      text = v.toString();
    if ( dni )
      delete dni;
  }
}

/** Gets the label's text string */
QString MLabelObject::getText() {
  return text;
}

/** Gets the label's text string */
bool MLabelObject::getChangeHeight() {
  return changeHeight;
}

/** Sets the label's pixmap */
void MLabelObject::setPixmap( const QPixmap & pix ) {
  if ( paintFunction.isEmpty() ) {
    if ( pixmap ) {
      delete pixmap;
      pixmap = 0;
    }
    if ( !pix.isNull() )
      pixmap = new QPixmap( pix );
  } else {
    if ( pixmap ) {
      delete pixmap;
      pixmap = 0;
    }
  }
}

/** Sets the label's text font */
void MLabelObject::setFont( const QString family, float size, int weight,
                            bool italic ) {
  fontFamily = family;
  fontSize = size;
  fontWeight = weight;
  fontItalic = italic;
}

/** Sets the label's horizontal alignment */
void MLabelObject::setHorizontalAlignment( int a ) {
  hAlignment = a;
}

/** Sets the label's vertical alignment */
void MLabelObject::setVerticalAlignment( int a ) {
  vAlignment = a;
}

/** Sets the label's word wrap flag */
void MLabelObject::setWordWrap( bool state ) {
  wordWrap = state;
}

/** Sets the label's change height flag */
void MLabelObject::setChangeHeight( bool state ) {
  changeHeight = state;
}

/** Sets the label's function */
void MLabelObject::setLabelFunction( const QString & lF ) {
  labelFunction = lF;
}

void MLabelObject::setPaintFunction( const QString & pF ) {
  paintFunction = pF;
}

void MLabelObject::setDomNodeData( const QDomNode & nD ) {
  domNodeData = nD;
}

/** Draws the label using the specificed painter & x/y-offsets */
int MLabelObject::draw( FLStylePainter * p ) {
  uint originalHeight = height;

  // Draw the pixmap
  if ( !paintFunction.isEmpty() ) {
    FLDomNodeInterface * dni = 0;
    QSArgumentList l;
    l << QVariant( text );
    if ( !domNodeData.isNull() ) {
      dni = new FLDomNodeInterface( domNodeData );
      l << dni;
    }
    QSArgument v = (( FLApplication * ) qApp ) ->call( paintFunction, l, 0 );
    QSArgument::Type tp = v.type();
    if ( tp !=  QSArgument::Invalid ) {
      QPixmap pix;
      if ( tp == QSArgument::VoidPointer ) {
        QPixmap * vPix = static_cast<QPixmap *>( v.ptr() );
        if ( vPix )
          pix = *vPix;
      } else
        if ( tp == QSArgument::Variant )
          pix = v.variant().toPixmap();
      if ( !pix.isNull() ) {
        if ( !changeHeight ) {
          int sy = pix.height() - ( height + yMargin );
          if ( sy < 0 )
            sy = 0;
          if ( !p->drawPixmap( pix, 0, sy, width - xMargin, height - yMargin, this ) )
            p->painter()->drawPixmap( xMargin, yMargin, pix, 0, sy, width - xMargin, height - yMargin );
          return 0;
        } else {
          int pixH = pix.height();
          if ( pixH > height ) {
            height = pixH;
            if ( !p->drawPixmap( pix, 0, 0, width - xMargin, height - yMargin, this ) )
              p->painter()->drawPixmap( xMargin, yMargin, pix, 0, 0, width - xMargin, height - yMargin );
            height = originalHeight;
            return pixH;
          }
        }
      }
    }
  }

  if ( pixmap ) {
    if ( !pixmap->isNull() ) {
      if ( !changeHeight ) {
        int sy = pixmap->height() - ( height + yMargin );
        if ( sy < 0 )
          sy = 0;
        if ( !p->drawPixmap( *pixmap, 0, sy, width - xMargin, height - yMargin, this ) )
          p->painter()->drawPixmap( xMargin, yMargin, *pixmap, 0, sy, width - xMargin, height - yMargin );
        return 0;
      } else {
        int pixH = pixmap->height();
        if ( pixH > height ) {
          height = pixH;
          if ( !p->drawPixmap( *pixmap, 0, 0, width - xMargin, height - yMargin, this ) )
            p->painter()->drawPixmap( xMargin, yMargin, *pixmap, 0, 0, width - xMargin, height - yMargin );
          height = originalHeight;
          return pixH;
        }
      }
    } else {
      delete pixmap;
      pixmap = 0;
    }
  }

  int retVal = 0;
  int tf;

  // Draw the base
  drawBase( p );

  // Draw the text
  QFont fnt;
  fnt.setFamily( fontFamily );
  fnt.setPointSizeFloat( fontSize );
  fnt.setWeight( fontWeight );
  fnt.setItalic( fontItalic );
  p->painter()->setFont( fnt );
  QFontMetrics fm = p->painter()->fontMetrics();
  p->painter()->setPen( foregroundColor );

  // Horizontal
  switch ( hAlignment ) {
    case MLabelObject::Left:
      tf = QPainter::AlignLeft;
      break;
    case MLabelObject::Center:
      tf = QPainter::AlignHCenter;
      break;
    case MLabelObject::Right:
      tf = QPainter::AlignRight;
  }

  // Vertical
  switch ( vAlignment ) {
    case MLabelObject::Top:
      tf = tf | QPainter::AlignTop;
      break;
    case MLabelObject::Bottom:
      tf = tf | QPainter::AlignBottom;
      break;
    case MLabelObject::Middle:
      tf = tf | QPainter::AlignVCenter;
  }

  // Word wrap
  if ( wordWrap )
    tf = tf | QPainter::WordBreak;

  if ( changeHeight ) {
    QRect maxRect = p->painter()->boundingRect( xMargin, yMargin, width - xMargin, height - yMargin, tf, text );
    if ( maxRect.height() > height ) {
      height = maxRect.height();
      drawBase( p );
      retVal = height;
    }
  }

  if ( !p->drawText( text, tf, this ) ) {
    bool restore = false;
    if ( p->errCode() == FLStylePainter::IdNotFound ) {
      p->painter()->save( QObject::name() );
      p->applyTransforms();
      p->painter()->translate( xpos, ypos );
      restore = true;
    }

    p->painter()->drawText( xMargin, yMargin, width - xMargin, height - yMargin, tf, text );

    if ( restore )
      p->painter()->restore();
  }

  height = originalHeight;
  return retVal;
}

/** Copies member data from one object to another.
      Used by the copy constructor and assignment operator */
void MLabelObject::copy( const MLabelObject * mLabelObject ) {
  // Copy the label's text
  text = mLabelObject->text;

  // Copy the label's font data
  fontFamily = mLabelObject->fontFamily;
  fontSize = mLabelObject->fontSize;
  fontWeight = mLabelObject->fontWeight;
  fontItalic = mLabelObject->fontItalic;

  // Copy the label's alignment data
  vAlignment = mLabelObject->vAlignment;
  hAlignment = mLabelObject->hAlignment;
  wordWrap = mLabelObject->wordWrap;
  labelFunction = mLabelObject->labelFunction;
  changeHeight = mLabelObject->changeHeight;
  paintFunction = mLabelObject->paintFunction;

  if ( mLabelObject->pixmap && !mLabelObject->pixmap->isNull() ) {
    if ( !pixmap )
      pixmap = new QPixmap();
    *pixmap = *( mLabelObject->pixmap );
  }

  domNodeData = mLabelObject->domNodeData;
}
