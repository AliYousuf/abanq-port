/***************************************************************************
           mpagecollection.cpp  -  Kugar report page collection
           -------------------
 begin     : Fri Aug 20 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
***************************************************************************/

#include "mpagecollection.h"
#include "mreportengine.h"

/** Constructor */
MPageCollection::MPageCollection() : QObject() {
  // Set page list to AutoDelete
  pages = new QPtrList < QPicture > ();
  pages->setAutoDelete( true );

  // Set the metrics
  size = MReportEngine::Letter;
  orientation = MReportEngine::Portrait;
  dimensions.setWidth( 0 );
  dimensions.setHeight( 0 );

  printToPos_ = false;
}

/** Copy constructor */
MPageCollection::MPageCollection( const MPageCollection & mPageCollection ) {
  copy( &mPageCollection );
}

/** Assignment operator */
MPageCollection MPageCollection::operator=( const MPageCollection & mPageCollection ) {
  if ( &mPageCollection == this )
    return * this;

  // Copy the derived class's data
  copy( &mPageCollection );

  return *this;
}

/** Destructor */
MPageCollection::~MPageCollection() {
  if ( pages ) {
    pages->clear();
    delete pages;
  }
}

/** Clears the page collection */
void
MPageCollection::clear() {
  pages->clear();
}

/** Appends a new page to the page collection */
void MPageCollection::appendPage() {
  pages->append( new QPicture() );
}

/** Gets the current page in the page collection,
  * the current page may be null
  */
QPicture *MPageCollection::getCurrentPage() {
  return pages->current();
}

/** Gets the first page in the page collection,
  * returns NULL if the list is empty
  */
QPicture *MPageCollection::getFirstPage() {
  return pages->first();
}

/** Gets the next page in the page collection,
  * returns NULL if the end of the list has been reached
  */
QPicture *MPageCollection::getNextPage() {
  return pages->next();
}

/** Get the previous page in the page collection,
  * returns NULL if the beginning of the list has been reached
  */
QPicture *MPageCollection::getPreviousPage() {
  return pages->prev();
}

/** Gets the last page in the page collection,
  * returns NULL if the list is empty
  */
QPicture *MPageCollection::getLastPage() {
  return pages->last();
}

/** Gets the page at the given index in the page collection,
  * returns NULL if the list is empty
  */
QPicture *MPageCollection::getPageAt( uint i ) {
  return pages->at( i );
}

/** Get the index of the current page */
int MPageCollection::getCurrentIndex() {
  return pages->at();
}

/** Set the current page to page at idx */
void MPageCollection::setCurrentPage( int idx ) {
  pages->at( idx );
}

/** Sets the page size */
void MPageCollection::setPageSize( int s ) {
  size = s;
}

/** Sets the page orientation */
void MPageCollection::setPageOrientation( int o ) {
  orientation = o;
}

/** Sets the page dimensions  */
void MPageCollection::setPageDimensions( QSize dim ) {
  dimensions = dim;
}

/** Returns the page size */
int MPageCollection::pageSize() {
  return size;
}

/** Returns the page orientation */
int MPageCollection::pageOrientation() {
  return orientation;
}

/** Returns the page dimensions  */
QSize MPageCollection::pageDimensions() {
  return dimensions;
}

/** Returns the number of pages in the page collection */
int MPageCollection::pageCount() {
  return pages->count();
}

/** Copies member data from one object to another.
  Used by the copy constructor and assignment operator */
void MPageCollection::copy( const MPageCollection * mPageCollection ) {
  // Copy the page collection
  pages = mPageCollection->pages;

  // Copy the page metrics
  dimensions = mPageCollection->dimensions;
  size = mPageCollection->size;
  orientation = mPageCollection->orientation;
}

bool MPageCollection::printToPos() {
  return printToPos_;
}

void MPageCollection::setPrintToPos( bool ptp ) {
  printToPos_ = ptp;
}
