/***************************************************************************
           mfieldobject.cpp  -  Kugar report field object
           -------------------
 begin     : Mon Aug 23 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
***************************************************************************/

#include "mfieldobject.h"
#include "mutil.h"

#include "FLCodBar.h"

/** Constructor */
MFieldObject::MFieldObject() : MLabelObject() {
  // Set the defaults
  fieldName = "";
  dataType = MFieldObject::String;
  format = MUtil::MDY_SLASH;
  precision = 0;
  currency = 36;
  negativeValueColor.setRgb( 255, 0, 0 );
  comma = 0;
  blankZero = 0;
  codbarType = FLCodBar::nameToType( "code128" );
  codbarRes = 72;
}

/** Copy constructor */
MFieldObject::MFieldObject( const MFieldObject & mFieldObject ) : MLabelObject(( MLabelObject & ) mFieldObject ) {
  copy( &mFieldObject );
}

/** Assignment operator */
MFieldObject MFieldObject::operator=( const MFieldObject & mFieldObject ) {
  if ( &mFieldObject == this )
    return * this;

  // Copy the derived class's data
  copy( &mFieldObject );

  // Copy the base class's data
  (( MLabelObject & ) * this ) = mFieldObject;

  return *this;
}

/** Destructor */
MFieldObject::~MFieldObject() {}

/** Returns the bound data field name */
QString MFieldObject::getFieldName() {
  return fieldName;
}

/** Sets the bound data field */
void MFieldObject::setFieldName( const QString field ) {
  fieldName = field;
}

/** Sets the field's data string */
void MFieldObject::setText( const QString txt ) {
  QDate d;
  int pos, ret;
  QString month, day, year;
  QRegExp regexp( "[0-9][0-9](-|//)[0-9][0-9](-|//)[0-9][0-9][0-9][0-9]" );
  double val;

  // Set the data
  switch ( dataType ) {
    case MFieldObject::String:
      text = txt;
      break;
    case MFieldObject::Integer:
      val = txt.toDouble();
      if (( val < 0.5 ) && ( val > -0.5 ) && blankZero ) {
        text = QString( "" );
      } else {
        text.setNum( val, 'f', 0 );
        formatNegValue();
        if ( comma )
          formatCommas();
      }
      break;
    case MFieldObject::Float:
      val = txt.toDouble();
      if (( val < 0.0000001 ) && ( val > -0.0000001 ) && blankZero ) {
        text = QString( "" );
      } else {
        text.setNum( val, 'f', precision );
        formatNegValue();
        if ( comma )
          formatCommas();
      }
      break;
    case MFieldObject::Date:
      if ( txt.isEmpty() )
        text = QString::null;
      else {
        // Check if we can read the date and if so, reformat it ...
        pos = regexp.search( txt.mid( 0 ) );
        ret = regexp.matchedLength();

        if ( ret == -1 ) {
          year = txt.left( 4 );
          day = txt.right( 2 );
          month = txt.mid( 5, 2 );

          if ( year.toInt() != 0 && month.toInt() != 0 && day.toInt() != 0 ) {
            d.setYMD( year.toInt(), month.toInt(), day.toInt() );
            text = MUtil::formatDate( d, format );
          } else {
            text = QString::null;
          }
        } else
          text = txt;
      }
      break;
    case MFieldObject::Currency:
      val = txt.toDouble();
      if (( val < 0.01 ) && ( val > -0.01 ) && blankZero ) {
        text = QString( "" );
      } else {
        text.setNum( val, 'f', 2 );
        formatNegValue();
        if ( comma )
          formatCommas();
        text = QString( text + currency );
      }
      break;
    case MFieldObject::Pixmap:
      if ( !txt.isEmpty() && paintFunction.isEmpty() ) {
        if ( !pixmap )
          pixmap = new QPixmap();
        QCString cs( txt );
        if ( !QPixmapCache::find( cs.left( 100 ), *pixmap ) )
          pixmap->loadFromData( cs );
        if ( !pixmap->isNull() ) {
          QPixmapCache::insert( cs.left( 100 ), *pixmap );
          QImage img;
          img = *pixmap;
          *pixmap = img.smoothScale( width, height );
        } else {
          delete pixmap;
          pixmap = 0;
        }
      } else {
        if ( pixmap ) {
          delete pixmap;
          pixmap = 0;
        }
      }
      break;
    case MFieldObject::Codbar: {
        if ( !txt.isEmpty() && paintFunction.isEmpty() ) {
          FLCodBar cb( txt, codbarType, 10, 1, 0, 0, true, Qt::black, Qt::white, codbarRes );
          if ( !pixmap )
            pixmap = new QPixmap();
          if ( !cb.pixmap().isNull() )
            *pixmap = cb.pixmap();
          else {
            delete pixmap;
            pixmap = 0;
          }
        } else {
          if ( pixmap ) {
            delete pixmap;
            pixmap = 0;
          }
        }
      }
      break;
    case MFieldObject::Bool:
      bool ok;
      double d = txt.toDouble( &ok );
      if ( txt.upper() == "FALSE" || txt.upper() == "F" || ( d == 0 && ok ) )
        text = QString( tr( "No" ) );
      else
        text = QString( tr( "Sí" ) );
      break;
  }
}

/** Sets the field's data type */
void MFieldObject::setDataType( int t ) {
  dataType = t;
  if ( dataType == MFieldObject::Integer || dataType == MFieldObject::Float
       || dataType == MFieldObject::Currency )
    saveColor = foregroundColor;
}

/** Sets code bar type */
void MFieldObject::setCodBarType( const QString & t ) {
  codbarType = FLCodBar::nameToType( t );
}

/** Sets code bar res in dpi (dots per inch) */
void MFieldObject::setCodBarRes( int r ) {
  codbarRes = r;
}

/** Sets the field's date formatting */
void MFieldObject::setDateFormat( int f ) {
  format = f;
}

/** Sets the field's precision */
void MFieldObject::setPrecision( int p ) {
  precision = p;
}

/** Sets the field's currency symbol */
void MFieldObject::setCurrency( const QChar c ) {
  if ( c.isNull() )
    currency = 36;
  else
    currency = c;
}

/** Sets the object's negative value color - default is red*/
void MFieldObject::setNegValueColor( int r, int g, int b ) {
  negativeValueColor.setRgb( r, g, b );
}

/** Sets if object should delimit numeric values with commas */
void MFieldObject::setCommaSeparator( int c ) {
  comma = c;
}

/** Sets if zero values are substituted by spaces. */
void MFieldObject::setBlankZero( int z ) {
  blankZero = z;
}

/** Formats a string representation of a negative number using the negative value color */
void MFieldObject::formatNegValue() {
  if ( text.toDouble() < 0 )
    foregroundColor = negativeValueColor;
  else
    foregroundColor = saveColor;
}

/** Formats the string representation of a number with comma separators */
void MFieldObject::formatCommas() {
  QString tmp;
  int i, j;
  int offset;

  // If a neg value
  if ( text.toDouble() < 0 )
    offset = 1;
  else
    offset = 0;

  // Look for decimal point
  int pos = text.findRev( QApplication::tr( "." ) );

  // If a decimal was not found, start at end of string
  if ( pos == -1 )
    pos = text.length();
  else
    tmp = QApplication::tr( "," ) + text.mid( pos + 1, text.length() );

  // Move through the string and insert the commas
  for ( i = pos - 1, j = 0; i >= offset; i-- ) {
    tmp = text.mid( i, 1 ) + tmp;
    j++;
    if (( j == 3 ) && ( i - 1 >= offset ) ) {
      tmp = QApplication::tr( "." ) + tmp;
      j = 0;
    }
  }

  // If neg value, add back the sign
  if ( offset )
    tmp = "-" + tmp;

  // Set the new string to the field
  text = tmp;
}

/** Copies member data from one object to another.
      Used by the copy constructor and assignment operator */
void MFieldObject::copy( const MFieldObject * mFieldObject ) {
  // Copy the fields's data type and format
  fieldName = mFieldObject->fieldName;
  dataType = mFieldObject->dataType;
  format = mFieldObject->format;
  precision = mFieldObject->precision;
  currency = mFieldObject->currency;
  negativeValueColor = mFieldObject->negativeValueColor;
  saveColor = mFieldObject->saveColor;
  comma = mFieldObject->comma;
  blankZero = mFieldObject->blankZero;
  codbarType = mFieldObject->codbarType;
  codbarRes = mFieldObject->codbarRes;
}
