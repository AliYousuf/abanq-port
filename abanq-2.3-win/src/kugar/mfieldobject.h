/***************************************************************************
             mfieldobject.h  -  Kugar report field object
             -------------------
   begin     : Mon Aug 23 1999
   copyright : (C) 1999 by Mutiny Bay Software
   email     : info@mutinybaysoftware.com
***************************************************************************/

#ifndef MFIELDOBJECT_H
#define MFIELDOBJECT_H

#include <qregexp.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qpixmapcache.h>

#include "mlabelobject.h"

/**
Kugar report field object

@author Mutiny Bay Software
*/
class MFieldObject: public MLabelObject {

public:

  /**
  Data type constants
  */
  enum DataType {
    String = 0,
    Integer,
    Float,
    Date,
    Currency,
    Pixmap,
    Codbar,
    Bool
  };

  /**
  Constructor
  */
  MFieldObject();

  /**
  Copy constructor
  */
  MFieldObject( const MFieldObject & mFieldObject );

  /**
  Assignment operator
  */
  MFieldObject operator=( const MFieldObject & mFieldObject );

  /**
  Destructor
  */
  virtual ~ MFieldObject();

protected:

  /**
  Field name
  */
  QString fieldName;

  /**
  Field data type
  */
  int dataType;

  /**
  Code bar type
  */
  int codbarType;

  /**
  Code bar res
  */
  int codbarRes;

  /**
  Field date format
  */
  int format;

  /**
  Field precision
  */
  int precision;

  /**
  Field currency symbol
  */
  QChar currency;

  /**
  Field's negative value color
  */
  QColor negativeValueColor;

  /**
  Field's original color
  */
  QColor saveColor;

  /**
  Field's comma flag
  */
  int comma;

  /**
  Field's blank zero flag
  */
  int blankZero;

public:

  /**
  Returns the bound data field name
  */
  QString getFieldName();

  /**
  Sets the bound data field
  */
  void setFieldName( const QString field );

  /**
  Sets the field's data string - default is an empty string
  */
  void setText( const QString txt );

  /**
  Sets the field's data type
  */
  void setDataType( int t );

  /**
  Sets code bar type
  */
  void setCodBarType( const QString &t );

  /**
  Sets code bar res in dpi (dots per inch)
  */
  void setCodBarRes( int r );

  /**
  Sets the field's date formatting
  */
  void setDateFormat( int f );

  /**
  Sets the field's precision
  */
  void setPrecision( int p );

  /**
  Sets the field's currency symbol
  */
  void setCurrency( const QChar c );

  /**
  Sets the object's negative value color - default is red
  */
  void setNegValueColor( int r, int g, int b );

  /**
  Sets if object should delimit numeric values with commas
  */
  void setCommaSeparator( int c );

  /**
  Sets if zero values are substituted by spaces.
  */
  void setBlankZero( int z );

private:

  /**
  Formats a string representation of a negative number using the negative value color
  */
  void formatNegValue();

  /**
  Formats a string representation of number with comma seperators
  */
  void formatCommas();

  /**
  Copies member data from one object to another.

  Used by the copy constructor and assignment operator
  */
  void copy( const MFieldObject * mFieldObject );

  friend class MReportSection;
};

#endif
