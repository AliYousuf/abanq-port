/***************************************************************************
           mreportobject.cpp  -  Kugar report object baseclass
           -------------------
 begin     : Wed Aug 11 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
***************************************************************************/

#include "mreportobject.h"

/** Constructor */
MReportObject::MReportObject() : QObject() {
  // Set the object's default geometry
  xpos = 0;
  ypos = 0;
  width = 40;
  height = 23;

  // Set the object's default colors
  backgroundColor.setRgb( 255, 255, 255 );
  foregroundColor.setRgb( 0, 0, 0 );

  // Set the object's default border attributes
  borderColor.setRgb( 0, 0, 0 );
  borderWidth = 1;
  borderStyle = MReportObject::SolidLine;
}

/** Copy constructor */
MReportObject::MReportObject( const MReportObject & mReportObject ) {                /*: QObject((QObject &) mReportObject) */
  copy( &mReportObject );
}

/** Assignment operator */
MReportObject MReportObject::operator=( const MReportObject & mReportObject ) {
  if ( &mReportObject == this )
    return * this;

  // Copy the derived class's data
  copy( &mReportObject );

  // Copy the base class's data
  //((QObject &) *this) = mReportObject;

  return *this;
}

/** Destructor */
MReportObject::~MReportObject() {}

/** Draws the object to the specified painter & x/y offsets */
int
MReportObject::draw( FLStylePainter * p ) {
  drawBase( p );
}

/** Draws the base object to the specified painter & x/y offsets */
void MReportObject::drawBase( FLStylePainter * p ) {
  if ( p->drawRect( this ) )
    return;

  bool restore = false;
  if ( p->errCode() == FLStylePainter::IdNotFound ) {
    p->painter()->save( QObject::name() );
    p->applyTransforms();
    p->painter()->translate( xpos, ypos );
    restore = true;
  }

  QBrush bgBrush( backgroundColor );
  QPen borderPen( borderColor, borderWidth, ( QPen::PenStyle ) borderStyle );

  // Set background in specified color
  if ( transparent ) {
    p->painter()->setBrush( Qt::NoBrush );
  } else {
    p->painter()->setBrush( bgBrush );
  }

  // Set border
  if ( borderStyle != 0 )
    p->painter()->setPen( borderPen );
  else
    p->painter()->setPen( QPen( Qt::NoPen ) );

  p->painter()->drawRect( 0, 0, width, height );

  if ( restore )
    p->painter()->restore();
}

/** Set the object's position and size */
void MReportObject::setGeometry( int x, int y, int w, int h ) {
  xpos = x;
  ypos = y;
  width = w;
  height = h;
}

/** Set the object's position */
void MReportObject::move( int x, int y ) {
  xpos = x;
  ypos = y;
}

/** Gets the object's x position */
int MReportObject::getX() {
  return xpos;
}

/** Gets the object's y position */
int MReportObject::getY() {
  return ypos;
}

/** Sets the object's background color */
void MReportObject::setBackgroundColor( int r, int g, int b ) {
  backgroundColor.setRgb( r, g, b );
}

/** Sets the object's foreground color */
void MReportObject::setForegroundColor( int r, int g, int b ) {
  foregroundColor.setRgb( r, g, b );
}

/** Sets the object's border color */
void MReportObject::setBorderColor( int r, int g, int b ) {
  borderColor.setRgb( r, g, b );
}

/** Sets the object's border width */
void MReportObject::setBorderWidth( int width ) {
  borderWidth = width;
}

/** Sets the object's border style */
void MReportObject::setBorderStyle( int style ) {
  borderStyle = style;
}

/** Sets the object's transparent mode */
void MReportObject::setTransparent( bool t ) {
  transparent = t;
}

/** Copies member data from one object to another.
    Used by the copy constructor and assignment operator */
void MReportObject::copy( const MReportObject * mReportObject ) {
  // Copy the object's geometry
  xpos = mReportObject->xpos;
  ypos = mReportObject->ypos;
  width = mReportObject->width;
  height = mReportObject->height;

  // Copy the colors
  backgroundColor = mReportObject->backgroundColor;
  foregroundColor = mReportObject->foregroundColor;

  // Copy the border attributes
  borderColor = mReportObject->borderColor;
  borderWidth = mReportObject->borderWidth;
  borderStyle = mReportObject->borderStyle;
}
