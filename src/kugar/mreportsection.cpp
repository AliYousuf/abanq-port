/***************************************************************************
           mreportsection.cpp  -  Kugar report section
           -------------------
 begin     : Mon Aug 23 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
 copyright : (C) 2002 Alexander Dymo
 email     : cloudtemple@mksat.net
***************************************************************************/

#include "mreportsection.h"
#include "mutil.h"
#include "FLApplication.h"
#include "FLObjectFactory.h"

uint MReportSection::idSecGlob_ = 0;

/** Constructor */
MReportSection::MReportSection( const QString & strIdSec ) : strIdSec_( strIdSec ) {
  idSec_ = idSecGlob_++;

  // Set geometry
  height = 1;
  width = 584;
  level = 0;

  // Set print frequency
  frequency = MReportSection::EveryPage;

  // Set special field data
  reportDate = QDate::currentDate();
  pageNumber = 0;

  // Set the line list to AutoDelete
  lines.setAutoDelete( true );
  // Set the label list to AutoDelete
  labels.setAutoDelete( true );
  // Set the special field list to AutoDelete
  specialFields.setAutoDelete( true );
  // Set the label list to AutoDelete
  fields.setAutoDelete( true );
  // Set the calc field list to AutoDelete
  calculatedFields.setAutoDelete( true );
}

/** Copy constructor */
MReportSection::MReportSection( const MReportSection & mReportSection ) {
  copy( &mReportSection );
}

/** Assignment operator */
MReportSection MReportSection::
operator= ( const MReportSection & mReportSection ) {
  if ( &mReportSection == this )
    return *this;

  // Copy the derived class's data
  copy( &mReportSection );

  return *this;
}

/** Destructor */
MReportSection::~MReportSection() {
  clear();
}

/** Frees all resources allocated by the report section */
void
MReportSection::clear() {
  clearBase();

  // Clear the field collection
  fields.clear();
}

/** Frees base resources allocated by the report section */
void MReportSection::clearBase() {
  // Clear the line collection
  lines.clear();
  // Clear the label collection
  labels.clear();
  // Clear the special field collection
  specialFields.clear();
  // Clear the calculated field collection
  calculatedFields.clear();
}

/** Adds a new line object to the section's line collection */
void MReportSection::addLine( MLineObject * line ) {
  lines.append( line );
}

/** Adds a new label object to the section's label collection */
void MReportSection::addLabel( MLabelObject * label ) {
  labels.append( label );
}

/** Adds a new special field object to the section's special field collection */
void MReportSection::addSpecialField( MSpecialObject * special ) {
  specialFields.append( special );
}

/** Adds a new calculated field object to the section's calculated field collection */
void MReportSection::addCalculatedField( MCalcObject * calc ) {
  calculatedFields.append( calc );
}

/** Adds a new field object to the section's field collection */
void MReportSection::addField( MFieldObject * field ) {
  fields.append( field );
}


/** Gets the height of the section */
int MReportSection::getHeight() {
  return height;
}

/** Gets the width of the section */
int MReportSection::getWidth() {
  return width;
}

void MReportSection::setFieldData( int idx, QString data ) {
  MFieldObject * field = fields.at( idx );

  field->setText( data );
}

/** Sets the height of the section */
void MReportSection::setHeight( int h ) {
  height = h;
}

/** Sets the width of the section */
void MReportSection::setWidth( const int w ) {
  width = w;
}

/** Sets the condition for drawing the section*/
void MReportSection::setDrawIf( QString dI ) {
  drawIf = dI;
}

/** Gets the condition for drawing the section*/
QString MReportSection::getDrawIf() {
  return drawIf;
}

/** Sets the level of the section */
void MReportSection::setLevel( int l ) {
  level = l;
}

/** Gets the level of the section */
int MReportSection::getLevel() {
  return level;
}

/** Sets new page at end of the section */
void MReportSection::setNewPage( bool b ) {
  newPage_ = b;
}

/** Sets new page at end of the section */
void MReportSection::setPlaceAtBottom( bool b ) {
  placeAtBottom_ = b;
}

/** Sets if the detail is on all pages */
void MReportSection::setDrawAllPages( bool b ) {
  drawAllPages_ = b;
}

/** Gets if there is new page at end of the section */
bool MReportSection::newPage() {
  return newPage_;
}

/** Gets if the footer must be drawed at the bottom of the page */
bool MReportSection::placeAtBottom() {
  return placeAtBottom_;
}

/** Gets if the detail is on all pages */
bool MReportSection::drawAllPages() {
  return drawAllPages_;
}

/** Set the current page number - used by special fields */
void MReportSection::setPageNumber( int page ) {
  pageNumber = page;
}

/** Set the current date - used by special fields */
void MReportSection::setReportDate( QDate date ) {
  reportDate = date;
}

/** Sets the print frequency of the section */
void MReportSection::setPrintFrequency( int printFrequency ) {
  frequency = printFrequency;
}

/** Returns the print frequency of the section */
int MReportSection::printFrequency() {
  return frequency;
}

/** Returns the number of fields in the detail section */
int MReportSection::getFieldCount() {
  return fields.count();
}


/** Returns the name of the bound field for field object at the given index */
QString MReportSection::getFieldName( int idx ) {
  MFieldObject * field = fields.at( idx );

  return field->getFieldName();
}


/** Returns the name of the bound field for the calculated field object at the given index */
QString MReportSection::getCalcFieldName( int idx ) {
  MCalcObject * field = calculatedFields.at( idx );

  return field->getFieldName();
}

/** Sets the data for the specified calculated field */
void MReportSection::setCalcFieldData( int idx, QString data ) {
  MCalcObject * field = calculatedFields.at( idx );

  field->setText( data );
}

/** Sets the data for the all calculated fields - list size must number of calculated fields */
void MReportSection::setCalcFieldData( QPtrList < QMemArray < double > > *values, QValueVector < QString > *valueS, QDomNode *record ) {
  MCalcObject * field;
  int i = 0;
  int calcType;
  QString value = "";

  // Calculate and set the calculated field's data

  for ( field = calculatedFields.first(); field != 0;
        field = calculatedFields.next() ) {

    if ( field->getFromGrandTotal() )
      continue;

    calcType = field->getCalculationType();

    if ( calcType == MCalcObject::NoOperation ) {
      if ( valueS )
        value = ( *valueS )[ i ];

      calculateField( field, 0, value, record );
    } else if ( calcType == MCalcObject::CallFunction ) {
      calculateField( field, 0, value, record );
    } else {
      if ( values )
        calculateField( field, values->at( i ), value, record );
    }

    i++;
  }
}

/** Sets the data for the calculated fields whose calculation source is the grand total */
void MReportSection::setCalcFieldDataGT( QPtrList < QMemArray < double > > *values, MReportSection *rFooter, QValueVector < QString > *valueS, QDomNode *record ) {
  MCalcObject * field;
  int grandTotalIndex = 0;
  // Calculate and set the calculated field's data

  for ( field = calculatedFields.first(); field != 0;
        field = calculatedFields.next() ) {

    if ( !field->getFromGrandTotal() && level > -1 )
      continue;

    grandTotalIndex = rFooter->getCalcFieldIndex( field->getFieldName() );

    if ( grandTotalIndex != -1 ) {
      calculateField( field, values->at( grandTotalIndex ), "", record );
    }
  }
}

/** Sets the data for the a calculated field
*/
void MReportSection::calculateField( MCalcObject *field, QMemArray < double > *values, QString valueS, QDomNode *record ) {
  switch ( field->getCalculationType() ) {

    case MCalcObject::Count:

      if ( values )
        field->setText( QString::number( MUtil::count( values ) ) );

      break;

    case MCalcObject::Sum:
      if ( values )
        field->setText( QString::number( MUtil::sum( values ), 'f' ) );

      break;

    case MCalcObject::Average:
      if ( values )
        field->setText( QString::number( MUtil::average( values ), 'f' ) );

      break;

    case MCalcObject::Variance:
      if ( values )
        field->setText( QString::number( MUtil::variance( values ), 'f' ) );

      break;

    case MCalcObject::StandardDeviation:
      if ( values )
        field->setText( QString::number( MUtil::stdDeviation( values ), 'f' ) );

      break;

    case MCalcObject::NoOperation:
      field->setText( valueS );

      break;

    case MCalcObject::CallFunction: {
        if ( record ) {
          QString fN = field->getCalculationFunction();
          FLDomNodeInterface * dni = new FLDomNodeInterface( *record );
          QSArgumentList l;
          l << dni;
          l << QVariant( field->getFieldName() );
          QVariant v = (( FLApplication * ) qApp ) ->call( fN, l, 0 ).variant();

          if ( v.isValid() )
            field->setText( v.toString() );

          delete dni;
        }
      }

      break;
  }

  if ( record )
    field->setDomNodeData( *record );
}

/** Returns wether the Report Section must be drawed or not depending on the DrawIf attribute and the current record values */
bool MReportSection::mustBeDrawed( QDomNode * record ) {
  QString value;
  QDomNamedNodeMap fields = record->attributes();
  QString drawIfField = getDrawIf();

  if ( !drawIfField.isEmpty() ) {
    QDomNode n = fields.namedItem( drawIfField );

    if ( n.isNull() )
      return false;

    value = n.toAttr().value();

    if ( value.isEmpty() )
      return false;

    bool b = true;

    float f = value.toFloat( &b );

    if ( f == 0 && b )
      return false;
  }

  return true;
}

/** Returns the index of the calculated field object for the given bound field */
int MReportSection::getCalcFieldIndex( QString field ) {
  MCalcObject * tmpField;

  // Find the field in the calculated field collection and return the index

  for ( tmpField = calculatedFields.first(); tmpField != 0;
        tmpField = calculatedFields.next() ) {
    if ( tmpField->getFieldName() == field )
      break;
  }

  return calculatedFields.at();
}

/** Returns the number of calculated fields in the section */
int MReportSection::getCalcFieldCount() {
  return calculatedFields.count();
}

/** Draws the section to the specified painter & x/y-offsets */
void MReportSection::draw( FLStylePainter * p, int xoffset, int yoffset, int &newHeight ) {
  drawObjects( p, xoffset, yoffset, newHeight );
  lastXOffset = xoffset;
  lastYOffset = yoffset;
}


/** Draws the section base objects to the specified painter & x/y offsets */
void MReportSection::drawHeaderObjects( FLStylePainter *p, MPageCollection *pages, MReportSection * header ) {
  MCalcObject * calcfield;

  // Set the offsets
  int xcalc = header->getLastXOffset();
  int ycalc = header->getLastYOffset();

  QPicture * currentPage = ( QPicture * ) p->painter()->device();
  QPicture * lastPage = header->onPage();
  QPicture * currentPageCopy = 0;
  QPicture * lastPageCopy = 0;

  if ( currentPage != lastPage ) {
    p->painter()->end();
    currentPageCopy = new QPicture( *currentPage );
    lastPageCopy = new QPicture( *lastPage );
    p->painter()->begin( lastPage );
    lastPageCopy->play( p->painter() );
  }

  QObject::setName( QString( "_##H%1-%2" ).arg( strIdSec_ ).arg( level ) );
  p->beginSection( xcalc, ycalc, width, height, this );
  uint countObj = 0;

  // Draw the calculated field collection
  for ( calcfield = calculatedFields.first(); calcfield != 0;
        calcfield = calculatedFields.next() ) {
    if ( calcfield->getDrawAtHeader() ) {
      calcfield->QObject::setName( QString( "_##H%1-Calc.%2-%3" ).arg( idSec_ ).arg( calcfield->fieldName ).arg( countObj++ ) );
      p->beginMark( calcfield->getX(), calcfield->getY(), calcfield );
      calcfield->draw( p );
      p->endMark();
    }
  }

  p->endSection();

  if ( currentPage != lastPage ) {
    p->painter()->end();
    p->painter()->begin( currentPage );
    currentPageCopy->play( p->painter() );
    delete lastPageCopy;
    delete currentPageCopy;
  }
}

/** Draws the section base objects to the specified painter & x/y offsets */
void MReportSection::drawObjects( FLStylePainter * p, int xoffset, int yoffset, int &newHeight ) {
  MLineObject * line;
  MLabelObject *label;
  MSpecialObject *special;
  MCalcObject *calcfield;
  MFieldObject *field;

  // Set the offsets
  int xcalc = xoffset;
  int ycalc = yoffset;

  int modifiedHeight = 0;

  csvData_ = "";
  QString fieldValue;

  QObject::setName( QString( "_##%1-%2" ).arg( strIdSec_ ).arg( level ) );
  p->beginSection( xcalc, ycalc, width, height, this );
  uint countObj = 0;

  // Draw the line collection
  for ( line = lines.first(); line != 0; line = lines.next() ) {
    line->QObject::setName( QString( "_##Line%1-%2" ).arg( idSec_ ).arg( countObj++ ) );
    p->beginMark( line->xpos1, line->ypos1, line );
    line->draw( p );
    p->endMark();
  }

  // Draw the label collection
  for ( label = labels.first(); label != 0; label = labels.next() ) {
    label->QObject::setName( QString( "_##Label%1-%2" ).arg( idSec_ ).arg( countObj++ ) );
    p->beginMark( label->getX(), label->getY(), label );
    modifiedHeight = label->draw( p );
    p->endMark();

    if ( modifiedHeight && ( label->getY() + modifiedHeight ) > height )
      newHeight = label->getY() + modifiedHeight;
  }

  // Draw the calculated field collection
  for ( calcfield = calculatedFields.first(); calcfield != 0;
        calcfield = calculatedFields.next() ) {
    if ( !calcfield->getDrawAtHeader() ) {
      calcfield->QObject::setName( QString( "_##%1-Calc.%2-%3" ).arg( idSec_ ).arg( calcfield->fieldName ).arg( countObj++ ) );
      p->beginMark( calcfield->getX(), calcfield->getY(), calcfield );
      modifiedHeight = calcfield->draw( p );
      p->endMark();

      if ( modifiedHeight && ( calcfield->getY() + modifiedHeight ) > height )
        newHeight = calcfield->getY() + modifiedHeight;
    }

    if ( calcfield->getCalculationType() == MCalcObject::NoOperation || calcfield->getCalculationType() == MCalcObject::CallFunction ) {
      fieldValue = calcfield->getText();
      fieldValue.replace( "\n", "-" );
      csvData_ += "|" + fieldValue;
    }
  }

  // Draw the special field collection
  for ( special = specialFields.first(); special != 0;
        special = specialFields.next() ) {
    special->QObject::setName( QString( "_##SpecialField%1-%2" ).arg( idSec_ ).arg( countObj++ ) );
    p->beginMark( special->getX(), special->getY(), special );

    switch ( special->getType() ) {

      case MSpecialObject::Date:
        special->setText( reportDate );
        break;

      case MSpecialObject::PageNumber:
        special->setText( pageNumber );
        break;
    }

    special->draw( p );
    p->endMark();
  }

  // Draw the field collection
  for ( field = fields.first(); field != 0; field = fields.next() ) {
    field->QObject::setName( QString( "_##%1.%2-%3" ).arg( idSec_ ).arg( field->fieldName ).arg( countObj++ ) );
    p->beginMark( field->getX(), field->getY(), field );
    modifiedHeight = field->draw( p );
    p->endMark();

    if ( modifiedHeight && ( field->getY() + modifiedHeight ) > height )
      newHeight = field->getY() + modifiedHeight;

    fieldValue = field->getText();
    fieldValue.replace( "\n", "-" );
    csvData_ += "|" + fieldValue;
  }

  p->endSection();
}

/** Copies member data from one object to another.
      Used by the copy constructor and assignment operator */
void MReportSection::copy( const MReportSection * mReportSection ) {
  strIdSec_ = mReportSection->strIdSec_;
  idSec_ = mReportSection->idSec_;

  // Copy the section's geometry
  height = mReportSection->height;

  // Copy the print frequency
  frequency = mReportSection->frequency;

  // Copy the line list
  lines = mReportSection->lines;
  // Copy the label list
  labels = mReportSection->labels;
  // Copy the special field list
  specialFields = mReportSection->specialFields;
  // Copy the calculated field list
  calculatedFields = mReportSection->calculatedFields;
  // Copy the field list
  fields = mReportSection->fields;
}

QString MReportSection::csvData() {
  return csvData_;
}

int MReportSection::getLastXOffset() {
  return lastXOffset;
}

int MReportSection::getLastYOffset() {
  return lastYOffset;
}

uint MReportSection::getLastPageIndex() {
  return lastPageIndex;
}

void MReportSection::setLastPageIndex( const uint i ) {
  lastPageIndex = i;
}

QPicture * MReportSection::onPage() {
  return onPage_;
}

void MReportSection::setOnPage( QPicture * page ) {
  onPage_ = page;
}

void MReportSection::resetIdSecGlob() {
  idSecGlob_ = 0;
}
