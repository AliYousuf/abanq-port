/***************************************************************************
             mcalcobject.h  -  Kugar report calculation field object
             -------------------
   begin     : Thu Sep 2 1999
   copyright : (C) 1999 by Mutiny Bay Software
   email     : info@mutinybaysoftware.com
***************************************************************************/

#ifndef MCALCOBJECT_H
#define MCALCOBJECT_H

#include "mfieldobject.h"

/**
Kugar report calculation field object

@author Mutiny Bay Software
*/
class MCalcObject: public MFieldObject {

public:

  /**
  Calculation type constants
  */
  enum CalculationType {
    Count = 0,
    Sum,
    Average,
    Variance,
    StandardDeviation,
    NoOperation,
    CallFunction
  };

  /**
  Constructor
  */
  MCalcObject();

  /**
  Copy constructor
  */
  MCalcObject( const MCalcObject & mCalcObject );

  /**
  Assignment operator
  */
  MCalcObject operator=( const MCalcObject & mCalcObject );

  /**
  Destructor
  */
  ~MCalcObject();

  /**
  Sets the field's  calculation type
  */
  void setCalculationType( int type );

  /**
  Returns the field's calculation type
  */
  int getCalculationType();

  /**
  Sets the field's  calculation function
  */
  void setCalculationFunction( QString fN );

  /** Returns the field's 'draw at header' property*/
  bool getDrawAtHeader();

  /** Sets the field's  'draw at header' property */
  void setDrawAtHeader( bool b );

  /** Returns the field's 'from grand total' property*/
  bool getFromGrandTotal();

  /** Sets the field's  'from grand total' property */
  void setFromGrandTotal( bool b );

  /**
  Returns the field's calculation function
  */
  QString getCalculationFunction();

private:

  /**
  Calculation type
  */
  int calcType;

  /**
  Calculation function
  */
  QString calcFunction;

private:

  /**
  Copies member data from one object to another.

  Used by the copy constructor and assignment operator
  */
  void copy( const MCalcObject * mCalcObject );

  /**
  Tells the engine to draw the field at the Header
  */
  bool drawAtHeader;

  /**
  Tells the engine to take the value from the grandTotal, not from the current level total
  */
  bool fromGrandTotal;

};

#endif
