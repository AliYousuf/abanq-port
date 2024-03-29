/***************************************************************************
             mreportviewer.h  -  Kugar QT report viewer widget
             -------------------
   begin     : Fri Aug 13 1999
   copyright : (C) 1999 by Mutiny Bay Software
   email     : info@mutinybaysoftware.com
   copyright : (C) 2002 Alexander Dymo
   email     : cloudtemple@mksat.net
   copyright : (C) 2002-2004 InfoSiAL S.L.
   email     : mail@infosial.com
***************************************************************************/

#ifndef MREPORTVIEWER_H
#define MREPORTVIEWER_H

#include <qapplication.h>
#include <qwidget.h>
#include <qscrollview.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qprogressdialog.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qprocess.h>
#include <qtimer.h>

#include "mreportengine.h"
#include "mpagecollection.h"
#include "mpagedisplay.h"

#define M_PROGRESS_DELAY 1000  // Number of ms to delay progress dialog display

#if defined(Q_OS_WIN32)
class PSPrinter;
#endif

class FLPosPrinter;

/**
Kugar QT report viewer widget

@author Mutiny Bay Software
*/
class FL_EXPORT MReportViewer: public QWidget {

  Q_OBJECT

public:

  MReportViewer( QWidget * parent = 0, const char *name = 0 );
  virtual ~ MReportViewer();

  bool setReportData( const QString & );
  bool setReportData( QIODevice * );
  bool setReportTemplate( const QString & );
  bool setReportTemplate( QIODevice * );
  bool renderReport( const int initRow = 0, const int initCol = 0 );
  void clearReport();
  bool printReport();
  bool printReportToPS( const QString & );
  bool printReportToPDF( const QString & );
  void setReportEngine( MReportEngine * );

  QSize sizeHint() const;

  void setNumCopies( const int numCopies );
  void setPrintToPos( bool ptp );
  void setPrinterName( const QString & pName );

  void setResolution( int dpi );
  int resolution() const {
    return dpi_;
  }

  MPageCollection * reportPages();

public slots:

  void slotFirstPage();
  void slotNextPage();
  void slotPrevPage();
  void slotLastPage();
  void slotZoomUp();
  void slotZoomDown();
  void slotUpdateDisplay();

signals:

  void preferedTemplate( const QString & );

private slots:

  void slotCancelPrinting();
  void slotRenderProgress( int );

protected:

  void paintEvent( QPaintEvent * event );
  void resizeEvent( QResizeEvent * event );

  bool printGhostReport();
  bool printGhostReportToPS( const QString & );
  bool printPosReport();

  QScrollView *scroller;
  MPageDisplay *display;
  MReportEngine *rptEngine;
  MPageCollection *report;
  QPrinter *printer;
  FLPosPrinter *posprinter;
#if defined(Q_OS_WIN32)
  PSPrinter *psprinter;
#endif
  QProgressDialog *progress;
  int totalSteps;
  int numCopies_;
  bool printToPos_;
  QString printerName_;
  int dpi_;
};

#endif
