/***************************************************************************
           mreportviewer.cpp  -  Kugar QT report viewer widget
           -------------------
 begin     : Fri Aug 13 1999
 copyright : (C) 1999 by Mutiny Bay Software
 email     : info@mutinybaysoftware.com
 copyright : (C) 2002 Alexander Dymo
 email     : cloudtemple@mksat.net
 copyright : (C) 2002-2003 InfoSiAL S.L.
 email     : mail@infosial.com
***************************************************************************/

#include <stdio.h>

#include "mreportviewer.h"
#include "config.h"

#if defined(Q_OS_WIN32)
#include "psprinter.h"
#endif

#include "posprinter.h"
#include "FLApplication.h"

/** Constructor */
MReportViewer::MReportViewer( QWidget * parent, const char *name )
    : QWidget( parent, name ), progress( 0 ), totalSteps( 0 ), printer( 0 ),
    posprinter( 0 ), numCopies_( 1 ), printToPos_( false ),
    printerName_( QString::null ), dpi_( 300 ) {
#if defined(Q_OS_WIN32)
  psprinter = 0;
#endif
  // Create the scrollview
  scroller = new QScrollView( this );

  // Create the report engine
  rptEngine = 0;
  report = new MPageCollection();

  // Get the current color palette
  QPalette p = palette();
  QColorGroup g = p.active();

  // Set the scroller's background color
  scroller->viewport() ->setBackgroundColor( g.mid() );

  // Create the report display widget
  display = new MPageDisplay( scroller->viewport() );

  // Set the display's default background color
  display->setBackgroundColor( white );

  // Add the display to the scrollview
  scroller->addChild( display );

  //Hide the display, we don't have a report yet ..
  display->hide();
}

/** Destructor */
MReportViewer::~MReportViewer() {
  clearReport();
  if ( rptEngine )
    delete rptEngine;
  if ( progress )
    delete progress;
}

/** Report viewer's paint event */
void MReportViewer::paintEvent( QPaintEvent * event ) {
  event = 0;
}

/** Report viewer's resize event */
void MReportViewer::resizeEvent( QResizeEvent * event ) {
  scroller->resize( event->size() );
}


// Set the report's data from an in-line string.

bool MReportViewer::setReportData( const QString & data ) {
  if ( rptEngine )
    return rptEngine->setReportData( data );
  else
    return false;
}


// Set the report's data from an i/o device.

bool MReportViewer::setReportData( QIODevice * dev ) {
  if ( rptEngine )
    return rptEngine->setReportData( dev );
  else
    return false;
}


// Set the report's template from an in-line string.

bool MReportViewer::setReportTemplate( const QString & tpl ) {
  if ( rptEngine )
    return rptEngine->setReportTemplate( tpl );
  else
    return false;
}


// Set the report's template from an i/o device.

bool MReportViewer::setReportTemplate( QIODevice * dev ) {
  if ( rptEngine )
    return rptEngine->setReportTemplate( dev );
  else
    return false;
}


/** Generates the report's page collection */
bool MReportViewer::renderReport( const int initRow, const int initCol ) {
  if ( !rptEngine )
    return false;

  // Check if a previous report exists and
  // if so de-allocated it
  if ( report != 0 )
    delete report;

  // Render the report
  report = rptEngine->renderReport( initRow, initCol );

  printToPos_ = report->printToPos();

  // Display the first page of the report
  if ( report != 0 && report->getFirstPage() != 0 ) {
    display->setPageDimensions( report->pageDimensions() );
    display->setPage( report->getFirstPage() );
    display->show();
    return true;
  }

  return false;
}

void MReportViewer::slotUpdateDisplay() {
  if ( report != 0 ) {
    display->setPageDimensions( report->pageDimensions() );
    if ( report->getCurrentPage() != 0 )
      display->setPage( report->getCurrentPage() );
    display->show();
  }
  display->repaint();
}

/** Clears the report's page collection */
void MReportViewer::clearReport() {
  // Hide the display
  if ( display )
    display->hide();

  // De-Allocate any report
  if ( report ) {
    delete report;
    report = 0;
  }
}

#if defined(Q_OS_WIN32)
bool MReportViewer::printGhostReport() {
  QString outPsFile = QString( FL_HOME ) + "/.flxcache/outprint.ps";
  QFile::remove( outPsFile );
  if ( !printReportToPS( outPsFile ) )
    return false;

  bool gsPrintOk = false;
  QProcess *procTemp = new QProcess();
  procTemp->addArgument( "gsprint" );
  procTemp->addArgument( "-help" );
  gsPrintOk = procTemp->start();
  delete procTemp;

  QProcess *proc = new QProcess();
  if ( gsPrintOk ) {
    proc->addArgument( "gsprint" );
    proc->addArgument( "-color" );
    proc->addArgument( "-query" );
    proc->addArgument( outPsFile );
  } else {
    QString setupPsFile = QString( FL_HOME ) + "/.flxcache/setup.ps";
    QFile::remove( setupPsFile );
    QFile fileSetup( setupPsFile );

    if ( fileSetup.open( IO_WriteOnly ) ) {
      QTextStream stream( &fileSetup );
      stream << "mark" << "\n";
      stream << "  /NoCancel      true" << "\n";
      stream << "  /BitsPerPixel  24" << "\n";
      stream << "  /UserSettings" << "\n";
      stream << "    <<" << "\n";
      stream << "      /DocumentName  (AbanQ document)" << "\n";
      stream << QString( "      /MaxResolution %1" ).arg( dpi_ ) << "\n";
      stream << "    >>" << "\n";
      stream << "  (mswinpr2) finddevice" << "\n";
      stream << "  putdeviceprops" << "\n";
      stream << "setdevice" << "\n";
      fileSetup.close();
    }

    proc->addArgument( "gswin32c" );
    proc->addArgument( "-q" );
    proc->addArgument( "-dBATCH" );
    proc->addArgument( "-dNOPAUSE" );
    proc->addArgument( "-dNODISPLAY" );
    proc->addArgument( QString( "-r%1" ).arg( dpi_ ) );
    proc->addArgument( setupPsFile );
    proc->addArgument( outPsFile );
  }

  if ( !proc->start() ) {
    delete proc;
    return false;
  }

  while ( proc->isRunning() )
    qApp->processEvents();

  delete proc;

  qApp->processEvents();

  return true;
}

bool MReportViewer::printGhostReportToPS( const QString & outPsFile ) {
  if ( report == 0 )
    return false;

  int cnt = report->pageCount();

  if ( cnt == 0 )
    return false;

  psprinter = new PSPrinter( PSPrinter::ScreenResolution );
  psprinter->setPageSize( ( PSPrinter::PageSize ) report->pageSize() );
  psprinter->setOrientation( ( PSPrinter::Orientation ) report->pageOrientation() );
  psprinter->setMinMax( 1, cnt );
  psprinter->setFromTo( 1, cnt );
  psprinter->setFullPage( true );
  psprinter->setColorMode( PSPrinter::Color );
  psprinter->setNumCopies( numCopies_ );
  psprinter->setResolution( dpi_ );

  QPicture * page;
  QPainter painter;
  bool printRev = false;

  int viewIdx = report->getCurrentIndex();

  if ( psprinter->pageOrder() == QPrinter::LastPageFirst )
    printRev = true;

  int printFrom = psprinter->fromPage() - 1;
  int printTo = psprinter->toPage();
  int printCnt = ( printTo - printFrom );
  int printCopies = psprinter->numCopies();
  int totalSteps = printCnt * printCopies;
  int currentStep = 1;

  psprinter->setNumCopies( 1 );
  psprinter->setOutputToFile( true );
  psprinter->setOutputFileName( outPsFile );

  QProgressDialog progress( tr( "Imprimiendo Informe..." ), tr( "Cancelar" ), totalSteps, this, tr( "progreso" ), true );
  progress.setMinimumDuration( M_PROGRESS_DELAY );
  QObject::connect( &progress, SIGNAL( cancelled() ), this, SLOT( slotCancelPrinting() ) );
  progress.setProgress( 0 );
  qApp->processEvents();

  painter.begin( psprinter );
  QPaintDeviceMetrics pdm( psprinter );
  painter.setWindow( 0, 0, pdm.widthMM() / 25.4 * 78., pdm.heightMM() / 25.4 * 78. );
  painter.setViewport( 0, 0, pdm.width(), pdm.height() );

  for ( int j = 0; j < printCopies; j++ ) {
    for ( int i = printFrom; i < printTo; i++, currentStep++ ) {
      if ( !psprinter->aborted() ) {
        progress.setProgress( currentStep );
        qApp->processEvents();
        if ( printRev )
          report->setCurrentPage( ( printCnt == 1 ) ? i : ( printCnt - 1 ) - i );
        else
          report->setCurrentPage( i );
        page = report->getCurrentPage();
        page->play( &painter );
        if ( ( i - printFrom ) < printCnt - 1 )
          psprinter->newPage();
      } else {
        j = printCopies;
        break;
      }
    }
    if ( j < printCopies - 1 )
      psprinter->newPage();
  }

  painter.end();
  report->setCurrentPage( viewIdx );

  delete psprinter;
  return true;
}
#else
bool MReportViewer::printGhostReport() {}
bool MReportViewer::printGhostReportToPS( const QString & ) {}
#endif

bool MReportViewer::printPosReport() {
  if ( report == 0 )
    return false;

  posprinter = new FLPosPrinter();
  posprinter->setPaperWidth( ( FLPosPrinter::PaperWidth ) report->pageSize() );
  posprinter->setPrinterName( printerName_ );

  QPicture * page;
  QPainter painter;
  bool printRev = false;

  int viewIdx = report->getCurrentIndex();

  int printCopies = numCopies_;

  painter.begin( posprinter );
  QPaintDeviceMetrics pdm( posprinter );
  painter.setWindow( 0, 0, pdm.widthMM() / 25.4 * 78., pdm.heightMM() / 25.4 * 78. );
  painter.setViewport( 0, 0, pdm.width(), pdm.height() );

  for ( int j = 0; j < printCopies; j++ ) {
    report->setCurrentPage( 1 );
    page = report->getCurrentPage();
    page->play( &painter );
  }

  painter.end();
  report->setCurrentPage( viewIdx );

  delete posprinter;

  return true;
}

/** Imprime directamente sobre formato PDF */
bool MReportViewer::printReportToPDF( const QString & outPdfFile ) {
  if ( report == 0 )
    return false;

#if defined(Q_OS_WIN32)
  QString gs = "gswin32c";
#else
  QString gs = "gs";
#endif

  bool gsOk = false;
  QProcess *procTemp = new QProcess();
  procTemp->addArgument( gs );
  procTemp->addArgument( "--version" );
  gsOk = procTemp->start();
  delete procTemp;
  if ( !gsOk ) {
    QMessageBox *m = new QMessageBox( tr( "Falta Ghostscript" ),
                                      tr( "Para poder exportar a PDF debe instalar Ghostscript (http://www.ghostscript.com) y añadir\n"
                                          "el directorio de instalación a la ruta de búsqueda de programas\ndel sistema (PATH).\n\n" ),
                                      QMessageBox::Critical, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this, 0, false );
    m->show();
    return false;
  }

  QString outPsFile = QString( FL_HOME ) + "/.flxcache/outprintpdf.ps";
  QString outPsPdfFile = QString( FL_HOME ) + "/.flxcache/outprintps.pdf";
  QFile::remove( outPsFile );
  QFile::remove( outPsPdfFile );
  if ( !printReportToPS( outPsFile ) )
    return false;

  QProcess *proc = new QProcess();
  proc->addArgument( gs );
  proc->addArgument( "-q" );
  proc->addArgument( "-dBATCH" );
  proc->addArgument( "-dNOPAUSE" );
  proc->addArgument( "-dSAFER" );
  proc->addArgument( "-dCompatibilityLevel=1.4" );
  proc->addArgument( "-dPDFSETTINGS=/printer" );
  proc->addArgument( "-dAutoFilterColorImages=false" );
  proc->addArgument( "-sColorImageFilter=FlateEncode" );
  proc->addArgument( "-dAutoFilterGrayImages=false" );
  proc->addArgument( "-sGrayImageFilter=FlateEncode" );
  proc->addArgument( QString( "-r%1" ).arg( dpi_ ) );
  proc->addArgument( "-sPAPERSIZE=a4" );
  proc->addArgument( "-dAutoRotatePages=/PageByPage" );
  proc->addArgument( "-sOutputFile=" + outPsPdfFile );
  proc->addArgument( "-sDEVICE=pdfwrite" );
  proc->addArgument( outPsFile );

  if ( !proc->start() ) {
    delete proc;
    return false;
  }

  while ( proc->isRunning() )
    qApp->processEvents();

  delete proc;

  qApp->processEvents();

  proc = new QProcess();
  proc->addArgument( gs );
  proc->addArgument( "-q" );
  proc->addArgument( "-dBATCH" );
  proc->addArgument( "-dNOPAUSE" );
  proc->addArgument( "-dSAFER" );
  proc->addArgument( "-dNODISPLAY" );
  proc->addArgument( "-dDELAYSAFER" );
  proc->addArgument( "--" );
  proc->addArgument( "pdfopt.ps" );
  proc->addArgument( outPsPdfFile );
  proc->addArgument( outPdfFile );

  if ( !proc->start() ) {
    delete proc;
    return false;
  }

  while ( proc->isRunning() )
    qApp->processEvents();

  delete proc;

  qApp->processEvents();

  return true;
}

/** Imprime directamente sobre formato PS */
bool MReportViewer::printReportToPS( const QString & outPsFile ) {
  // Check for a report
  if ( report == 0 )
    return false;

#if defined(Q_OS_WIN32)
  return printGhostReportToPS( outPsFile );
#endif

  // Get the page count
  int cnt = report->pageCount();

  // Check if there is a report or any pages to print
  if ( cnt == 0 ) {
    QMessageBox::critical( this, "Kugar",
                           tr( "No hay páginas en el\ninforme para." ),
                           QMessageBox::Ok, QMessageBox::NoButton,
                           QMessageBox::NoButton );
    return false;
  }

  // Set the printer dialog
  printer = new QPrinter( QPrinter::ScreenResolution );
  printer->setPageSize( ( QPrinter::PageSize ) report->pageSize() );
  printer->setOrientation( ( QPrinter::Orientation ) report->pageOrientation() );
  printer->setMinMax( 1, cnt );
  printer->setFromTo( 1, cnt );
  printer->setFullPage( true );
  printer->setColorMode( QPrinter::Color );
  printer->setNumCopies( numCopies_ );
  printer->setOutputToFile( true );
  printer->setOutputFileName( outPsFile );

  QPicture * page;
  QPainter painter;
  bool printRev = false;

  // Save the viewer's page index
  int viewIdx = report->getCurrentIndex();

  // Check the order we are printing the pages
  if ( printer->pageOrder() == QPrinter::LastPageFirst )
    printRev = true;

  // Get the count of pages and copies to print
  int printFrom = printer->fromPage() - 1;
  int printTo = printer->toPage();
  int printCnt = ( printTo - printFrom );
  int printCopies = printer->numCopies();
  int totalSteps = printCnt * printCopies;
  int currentStep = 1;

  // Set copies to 1, QPrinter copies does not appear to work ...
  printer->setNumCopies( numCopies_ );
  printer->setResolution( dpi_ );

  // Setup the progress dialog
  QProgressDialog progress( tr( "Imprimiendo Informe..." ), tr( "Cancelar" ),
                            totalSteps, this, tr( "progreso" ), true );
  progress.setMinimumDuration( M_PROGRESS_DELAY );
  QObject::connect( &progress, SIGNAL( cancelled() ), this,
                    SLOT( slotCancelPrinting() ) );
  progress.setProgress( 0 );
  qApp->processEvents();

  // Start the printer
  painter.begin( printer );
  QPaintDeviceMetrics pdm( printer );
  painter.setWindow( 0, 0, pdm.widthMM() / 25.4 * 78., pdm.heightMM() / 25.4 * 78. );
  painter.setViewport( 0, 0, pdm.width(), pdm.height() );

  // Print each page in the collection
  for ( int i = printFrom; i < printTo; i++, currentStep++ ) {
    if ( !printer->aborted() ) {
      progress.setProgress( currentStep );
      qApp->processEvents();
      if ( printRev )
        report->setCurrentPage( ( printCnt == 1 ) ? i : ( printCnt - 1 ) - i );
      else
        report->setCurrentPage( i );

      page = report->getCurrentPage();
      page->play( &painter );
      if ( ( i - printFrom ) < printCnt - 1 )
        printer->newPage();
    } else {
      break;
    }
  }

  // Cleanup printing
  painter.end();
  report->setCurrentPage( viewIdx );
  delete printer;
  return true;
}

/** Prints the rendered report to the selected printer - displays Qt print dialog */
bool MReportViewer::printReport() {
  // Check for a report
  if ( report == 0 )
    return false;

  report->setPrintToPos( printToPos_ );

  if ( report->printToPos() )
    return printPosReport();

#if defined(Q_OS_WIN32)
  bool gsOk = false;
  QProcess *procTemp = new QProcess();
  procTemp->addArgument( "gswin32c" );
  procTemp->addArgument( "--version" );
  gsOk = procTemp->start();
  delete procTemp;
  if ( gsOk ) {
    if ( printGhostReport() )
      return true;
  }

  QMessageBox *m = new QMessageBox( tr( "Sugerencia" ),
                                    tr( "Si instala Ghostscript (http://www.ghostscript.com) y añade\n"
                                        "el directorio de instalación a la ruta de búsqueda de programas\n"
                                        "del sistema (PATH), AbanQ podrá utilizarlo para optimizar\n"
                                        "sustancialmente la calidad de impresión y para poder generar códigos\nde barras.\n\n" ),
                                    QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this, 0, false );
  m->show();
#endif

  // Get the page count
  int cnt = report->pageCount();

  // Check if there is a report or any pages to print
  if ( cnt == 0 ) {
    QMessageBox::critical( this, "Kugar",
                           tr( "No hay páginas en el\ninforme para." ),
                           QMessageBox::Ok, QMessageBox::NoButton,
                           QMessageBox::NoButton );
    return false;
  }

  // Set the printer dialog
  printer = new QPrinter( QPrinter::ScreenResolution );
  printer->setPageSize( ( QPrinter::PageSize ) report->pageSize() );
  printer->setOrientation( ( QPrinter::Orientation ) report->pageOrientation() );
  printer->setMinMax( 1, cnt );
  printer->setFromTo( 1, cnt );
  printer->setFullPage( true );
  printer->setColorMode( QPrinter::Color );
  printer->setNumCopies( numCopies_ );
  printer->setResolution( dpi_ );
  if ( !printerName_.isEmpty() )
    printer->setPrinterName( printerName_ );
  QString printProg( ( ( FLApplication * ) qApp )->printProgram() );
  if ( !printProg.isEmpty() )
    printer->setPrintProgram( ( ( FLApplication * ) qApp )->printProgram() );

  bool printNow = true;
  if ( !printerName_.isNull() )
    printNow = true;
  else
    printNow = printer->setup( qApp->focusWidget() );

  if ( printNow ) {
    QPicture * page;
    QPainter painter;
    bool printRev = false;

    // Save the viewer's page index
    int viewIdx = report->getCurrentIndex();

    // Check the order we are printing the pages
    if ( printer->pageOrder() == QPrinter::LastPageFirst )
      printRev = true;

    // Get the count of pages and copies to print
    int printFrom = printer->fromPage() - 1;
    int printTo = printer->toPage();
    int printCnt = ( printTo - printFrom );
    int printCopies = printer->numCopies();
    int totalSteps = printCnt * printCopies;
    int currentStep = 1;

    // Set copies to 1, QPrinter copies does not appear to work ...
    printer->setNumCopies( 1 );

    // Setup the progress dialog
    QProgressDialog progress( tr( "Imprimiendo Informe..." ), tr( "Cancelar" ),
                              totalSteps, this, tr( "progreso" ), true );
    progress.setMinimumDuration( M_PROGRESS_DELAY );
    QObject::connect( &progress, SIGNAL( cancelled() ), this,
                      SLOT( slotCancelPrinting() ) );
    progress.setProgress( 0 );
    qApp->processEvents();

    // Start the printer
    painter.begin( printer );
    QPaintDeviceMetrics pdm( printer );

    painter.setWindow( 0, 0, pdm.widthMM() / 25.4 * 78., pdm.heightMM() / 25.4 * 78. );
    painter.setViewport( 0, 0, pdm.width(), pdm.height() );

    // Print each copy
    for ( int j = 0; j < printCopies; j++ ) {
      // Print each page in the collection
      for ( int i = printFrom; i < printTo; i++, currentStep++ ) {
        if ( !printer->aborted() ) {
          progress.setProgress( currentStep );
          qApp->processEvents();
          if ( printRev )
            report->setCurrentPage( ( printCnt == 1 ) ? i : ( printCnt - 1 ) - i );
          else
            report->setCurrentPage( i );

          page = report->getCurrentPage();
          page->play( &painter );
          if ( ( i - printFrom ) < printCnt - 1 )
            printer->newPage();
        } else {
          j = printCopies;
          break;
        }
      }
      if ( j < printCopies - 1 )
        printer->newPage();
    }

    // Cleanup printing
    painter.end();
    report->setCurrentPage( viewIdx );
    delete printer;
    return true;
  }
  delete printer;
  return false;
}

/** Shows the first page in the report */
void MReportViewer::slotFirstPage() {
  QPicture * page;

  if ( report == 0 )
    return ;

  if ( ( page = report->getFirstPage() ) != 0 ) {
    display->setPage( page );
    display->repaint();
  }
}

/** Shows the next page in the report */
void MReportViewer::slotNextPage() {
  QPicture * page;

  if ( report == 0 )
    return ;

  int index = report->getCurrentIndex();

  if ( ( page = report->getNextPage() ) != 0 ) {
    display->setPage( page );
    display->repaint();
  } else
    report->setCurrentPage( index );
}

/** Shows the prevoius page in the report */
void MReportViewer::slotPrevPage() {
  QPicture * page;

  if ( report == 0 )
    return ;

  int index = report->getCurrentIndex();

  if ( ( page = report->getPreviousPage() ) != 0 ) {
    display->setPage( page );
    display->repaint();
  } else
    report->setCurrentPage( index );
}

/** Shows the last page in the report */
void MReportViewer::slotLastPage() {
  QPicture * page;

  if ( report == 0 )
    return ;

  if ( ( page = report->getLastPage() ) != 0 ) {
    display->setPage( page );
    display->repaint();
  }
}

void MReportViewer::slotZoomUp() {
  if ( report == 0 )
    return ;

  display->zoomUp();
  display->repaint();
}

void MReportViewer::slotZoomDown() {
  if ( report == 0 )
    return ;

  display->zoomDown();
  display->repaint();
}

/** Cancel printing of the report */
void MReportViewer::slotCancelPrinting() {
#if defined(Q_OS_WIN32)
  if ( psprinter )
    psprinter->abort();
  else
    if ( printer )
#endif

      printer->abort();
}

/** Updates rendering progress */
void MReportViewer::slotRenderProgress( int p ) {
  if ( !rptEngine )
    return ;

  if ( !progress ) {
    totalSteps = rptEngine->getRenderSteps();
    if ( totalSteps <= 0 )
      totalSteps = 1;
    progress = new QProgressDialog( tr( "Creando informe..." ), tr( "Cancelar" ), totalSteps, 0, tr( "progreso" ), true );
    progress->setMinimumDuration( M_PROGRESS_DELAY );
    connect( progress, SIGNAL( canceled() ), rptEngine, SLOT( slotCancelRendering() ) );
  }

  if ( !progress )
    return ;

  progress->setProgress( p );
  qApp->processEvents();
}


// Return the preferred size.
QSize MReportViewer::sizeHint() const {
  return scroller->sizeHint();
}

//copyright : (C) 2002-2003 InfoSiAL S.L.
//email     : mail@infosial.com
void MReportViewer::setReportEngine( MReportEngine * r ) {
  rptEngine = r;

  // Connect the rendering update signal and slot
  connect( rptEngine, SIGNAL( signalRenderStatus( int ) ), SLOT( slotRenderProgress( int ) ) );

  connect( rptEngine, SIGNAL( preferedTemplate( const QString & ) ), SIGNAL( preferedTemplate( const QString & ) ) );
}

void MReportViewer::setNumCopies( const int numCopies ) {
  numCopies_ = numCopies;
}

void MReportViewer::setPrintToPos( bool ptp ) {
  printToPos_ = ptp;
}

void MReportViewer::setPrinterName( const QString & pName ) {
  printerName_ = pName;
}

void MReportViewer::setResolution( int dpi ) {
  dpi_ = dpi;
}

MPageCollection * MReportViewer::reportPages() {
  return report;
}
