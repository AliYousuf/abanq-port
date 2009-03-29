/***************************************************************************
           mcalcobject.cpp  -  Kugar report calculation field object
           -------------------
 begin     : Thu Sep 2 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
***************************************************************************/

#include "mcalcobject.h"

/** Constructor */
MCalcObject::MCalcObject() : MFieldObject() {
  // Set the default calculation type
  calcType = MCalcObject::Count;
}

/** Copy constructor */
MCalcObject::MCalcObject( const MCalcObject & mCalcObject ) : MFieldObject(( MFieldObject & ) mCalcObject ) {
  copy( &mCalcObject );
}

/** Assignment operator */
MCalcObject MCalcObject::operator=( const MCalcObject & mCalcObject ) {
  if ( &mCalcObject == this )
    return *this;

  // Copy the derived class's data
  copy( &mCalcObject );

  // Copy the base class's data
  (( MFieldObject & ) * this ) = mCalcObject;

  return *this;
}

/** Destructor */
MCalcObject::~MCalcObject() {
}

/** Sets the field's  calculation type */
void
MCalcObject::setCalculationType( int type ) {
  calcType = type;
}

/** Sets the field's  calculation function */
void
MCalcObject::setCalculationFunction( QString fN ) {
  calcFunction = fN;
}

/** Sets the field's  'draw at header' property */
void MCalcObject::setDrawAtHeader( bool b ) {
  drawAtHeader = b;
}

/** Sets the field's 'from grand total' property*/
void MCalcObject::setFromGrandTotal( bool b ) {
  fromGrandTotal = b;
}

/** Returns the field's calculation type */
int MCalcObject::getCalculationType() {
  return calcType;
}

/** Returns the field's calculation function */
QString MCalcObject::getCalculationFunction() {
  return calcFunction;
}

/** Returns the field's 'draw at header' property*/
bool MCalcObject::getDrawAtHeader() {
  return drawAtHeader;
}

/** Returns the field's 'from grand total' property*/
bool MCalcObject::getFromGrandTotal() {
  return fromGrandTotal;
}


/** Copies member data from one object to another.
      Used by the copy constructor and assignment operator */
void MCalcObject::copy( const MCalcObject * mCalcObject ) {
  // Copy the fields's calculation type
  calcType = mCalcObject->calcType;
}
