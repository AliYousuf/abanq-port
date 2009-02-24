/***************************************************************************
                          FLReportViewer.cpp
                         -------------------
begin                : vie jun 28 2002
copyright            : (C) 2002-2005 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLReportViewer.h"
#include "FLReportEngine.h"
#include "mreportviewer.h"
#include "FLSmtpClient.h"
#include "FLUtil.h"
#include "FLPicture.h"
#include "FLStylePainter.h"

FLReportViewer::FLReportViewer( QWidget * parent, const char * name, bool embedInParent )
    : FLWidgetReportViewer( parent, name, (( parent && embedInParent ) ? 0 :
                                           ( Qt::WStyle_Customize | Qt::WStyle_Maximize | Qt::WStyle_Title |
                                             Qt::WType_Dialog | Qt::WShowModal | Qt::WStyle_SysMenu ) ) ),
    loop( false ), reportPrinted_( false ), report( 0 ), qry_( 0 ) {

  embedInParent_ = ( parent && embedInParent );

  if ( embedInParent_ ) {
    autoClose_ = false;
    menubar->hide();
    chkAutoClose->hide();
    spnResolution->hide();
    spnPixel->hide();
    salir->setVisible( false );

    if ( !parent->layout() ) {
      QVBoxLayout * lay = new QVBoxLayout( parent );
      lay->addWidget( this );
    } else
      parent->layout()->add( this );
  } else {
    autoClose_ = FLUtil::readSettingEntry( "rptViewer/autoClose", "false" ).toBool();
    chkAutoClose->setChecked( autoClose_ );
  }

  rptEngine = new FLReportEngine;

  rptViewer = new MReportViewer( this );

  rptViewer->setReportEngine( rptEngine );

  setFont( qApp->font() );
  setFocusPolicy( QWidget::StrongFocus );

  lePara->setText( FLUtil::readSettingEntry( "email/to" ).toString() );
  leDe->setText( FLUtil::readSettingEntry( "email/from" ).toString() );
  leMailServer->setText( FLUtil::readSettingEntry( "email/mailserver" ).toString() );

  initCentralWidget_ = centralWidget();

  smtpClient_ = new FLSmtpClient( this );
  connect( smtpClient_, SIGNAL( status( const QString& ) ), lblEstado, SLOT( setText( const QString& ) ) );

  setCentralWidget( rptViewer );
  frEMail->hide();
  initCentralWidget_->hide();

  if ( !embedInParent_ ) {
    spnResolution->setValue( FLUtil::readSettingEntry( "rptViewer/dpi",
                             QVariant( rptViewer->resolution() ).toString() ).toInt() );
    spnPixel->setValue( FLUtil::readSettingEntry( "rptViewer/pixel",
                        QString::number(( double ) rptEngine->relDpi(), 'f', 2 ) ).toDouble() * 10. );
  }

  report = rptViewer->reportPages();
}

FLReportViewer::~FLReportViewer() {
  if ( rptEngine )
    rptEngine->deleteLater();

  if ( !embedInParent_ )
    FLUtil::writeSettingEntry( "rptViewer/autoClose", QVariant( autoClose_ ).toString() );
}

void FLReportViewer::exec() {
  show();

  if ( embedInParent_ )
    return;

  loop = true;

  qApp->exec();

  deleteLater();
}

QString FLReportViewer::csvData() {
  return rptEngine->csvData();
}

void FLReportViewer::closeEvent( QCloseEvent * e ) {
  if ( loop && !embedInParent_ )
    qApp->exit();

  loop = false;

  QWidget::closeEvent( e );
}

bool FLReportViewer::renderReport( const int initRow, const int initCol ) {
  bool ret = rptViewer->renderReport( initRow, initCol );
  report = rptViewer->reportPages();
  return ret;
}

void FLReportViewer::slotFirstPage() {
  rptViewer->slotFirstPage();
}

void FLReportViewer::slotLastPage() {
  rptViewer->slotLastPage();
}

void FLReportViewer::slotNextPage() {
  rptViewer->slotNextPage();
}

void FLReportViewer::slotPrevPage() {
  rptViewer->slotPrevPage();
}

void FLReportViewer::slotZoomUp() {
  rptViewer->slotZoomUp();
}

void FLReportViewer::slotZoomDown() {
  rptViewer->slotZoomDown();
}

void FLReportViewer::exportFileCSVData() {
  QString fileName = QFileDialog::getSaveFileName( "", tr( "Fichero CSV (*.csv *.txt)" ),
                     this, tr( "Exportar a CSV" ), tr( "Exportar a CSV" ) );

  if ( fileName.isEmpty() )
    return;

  if ( !fileName.upper().contains( ".CSV" ) )
    fileName += ".csv";

  if ( QFile::exists( fileName ) && QMessageBox::question( this, tr( "Sobreescribir %1" ).arg( fileName ),
       tr( "Ya existe un fichero llamado %1. ¿ Desea sobreescribirlo ?" ).arg( fileName ),
       tr( "&Sí" ), tr( "&No" ), QString::null, 0, 1 ) )
    return;

  QFile file( fileName );

  if ( file.open( IO_WriteOnly ) ) {
    QTextStream stream( &file );
    stream << csvData() << "\n";
    file.close();
  } else {
    QMessageBox::critical( this, tr( "Error abriendo fichero" ),
                           tr( "No se pudo abrir el fichero %1 para escribir: %2" ).arg( fileName,
                               qApp->translate( "QFile", file.errorString() ) ) );
  }
}

void FLReportViewer::exportToPDF() {
  QString fileName = QFileDialog::getSaveFileName( "", tr( "Fichero PDF (*.pdf)" ),
                     this, tr( "Exportar a PDF" ), tr( "Exportar a PDF" ) );

  if ( fileName.isEmpty() )
    return;

  if ( !fileName.upper().contains( ".PDF" ) )
    fileName += ".pdf";

  if ( QFile::exists( fileName ) && QMessageBox::question( this, tr( "Sobreescribir %1" ).arg( fileName ),
       tr( "Ya existe un fichero llamado %1. ¿ Desea sobreescribirlo ?" ).arg( fileName ),
       tr( "&Sí" ), tr( "&No" ), QString::null, 0, 1 ) )
    return;

  slotPrintReportToPDF( fileName );
}

void FLReportViewer::sendEMailPDF() {
  QString name = leDocumento->text().isEmpty() ?  "informe.pdf" : leDocumento->text();
  QString fileName = QFileDialog::getSaveFileName( QString( FL_HOME ) + "/" + name +
                     ".pdf", tr( "Fichero PDF a enviar(*.pdf)" ), this, tr( "Exportar a PDF par enviar" ),
                     tr( "Exportar a PDF para enviar" ) );

  if ( fileName.isEmpty() )
    return;

  if ( !fileName.upper().contains( ".PDF" ) )
    fileName += ".pdf";

  if ( QFile::exists( fileName ) && QMessageBox::question( this, tr( "Sobreescribir %1" ).arg( fileName ),
       tr( "Ya existe un fichero llamado %1. ¿ Desea sobreescribirlo ?" ).arg( fileName ),
       tr( "&Sí" ), tr( "&No" ), QString::null, 0, 1 ) )
    return;

  bool autoCloseSave = autoClose_;

  autoClose_ = false;

  slotPrintReportToPDF( fileName );

  autoClose_ = autoCloseSave;

  FLUtil::writeSettingEntry( "email/to", lePara->text() );

  FLUtil::writeSettingEntry( "email/from", leDe->text() );

  FLUtil::writeSettingEntry( "email/mailserver", leMailServer->text() );

  QFileInfo fi( fileName );

  name = fi.fileName();

  smtpClient_->setMailServer( leMailServer->text() );

  smtpClient_->setTo( lePara->text() );

  smtpClient_->setFrom( leDe->text() );

  smtpClient_->setSubject( leAsunto->text().isEmpty() ? name : leAsunto->text() );

  smtpClient_->setBody( leCuerpo->text() + "\n\n" + name );

  smtpClient_->addAttachment( fileName );

  smtpClient_->startSend();
}

void FLReportViewer::showInitCentralWidget( bool show ) {
  if ( show ) {
    rptViewer->hide();
    setCentralWidget( initCentralWidget_ );
    leDocumento->setText( "doc-" + QDateTime::currentDateTime().toString().replace( ":", "" ).replace( " ", "" ) );
    frEMail->show();
    initCentralWidget_->show();
  } else {
    initCentralWidget_->hide();
    frEMail->hide();
    setCentralWidget( rptViewer );
    rptViewer->show();
  }
}

void FLReportViewer::saveSVGStyle() {
  if ( report ) {
    QString fileName = QFileDialog::getSaveFileName( "", tr( "Fichero SVG (*.svg)" ),
                       this, tr( "Guardar en SVG" ), tr( "Guardar en SVG" ) );
    if ( fileName.isEmpty() )
      return;

    if ( !fileName.upper().contains( ".SVG" ) )
      fileName += ".svg";
    if ( QFile::exists( fileName ) && QMessageBox::question( this, tr( "Sobreescribir %1" ).arg( fileName ),
         tr( "Ya existe un fichero llamado %1. ¿ Desea sobreescribirlo ?" ).arg( fileName ),
         tr( "&Sí" ), tr( "&No" ), QString::null, 0, 1 ) )
      return;

    FLStylePainter::setSVGMode( true );
    updateReport();
    FLStylePainter::setSVGMode( false );
    
    QStringList fileNames;
    QString fname;
    QPicture * page;
    QSize psize;
    for ( int i = 0; i < report->pageCount(); ++i ) {
      fname = fileName + QString::number( i );
      fileNames.append( fname );
      page = report->getPageAt( i );
      psize = report->pageDimensions();
      page->setBoundingRect( QRect( QPoint( 0, 0 ), psize ) );
      page->save( fname, "svg" );
    }
    
    FLStylePainter::normalizeSVGFile( fileName, fileNames );

    updateReport();
  }
}

void FLReportViewer::saveSimpleSVGStyle() {
  QString backStyleName = styleName_;
  styleName_ = "_simple";
  saveSVGStyle();
  styleName_ = backStyleName;
  updateReport();
}

void FLReportViewer::loadSVGStyle() {
  QString fileName = QFileDialog::getOpenFileName( "", tr( "Fichero SVG (*.svg)" ),
                     this, tr( "Cargar estilo SVG" ), tr( "Cargar estilo SVG" ) );
  if ( fileName.isEmpty() )
    return;
  ledStyle->setText( "file:" + fileName );
  updateReport();
}

void FLReportViewer::slotExit() {
  close();
}

void FLReportViewer::slotPrintReportToPS( const QString & outPsFile ) {
  reportPrinted_ = rptViewer->printReportToPS( outPsFile );

  if ( reportPrinted_ && autoClose_ )
    slotExit();
}

void FLReportViewer::slotPrintReportToPDF( const QString & outPdfFile ) {
  reportPrinted_ = rptViewer->printReportToPDF( outPdfFile );

  if ( reportPrinted_ && autoClose_ )
    slotExit();
}

void FLReportViewer::slotPrintReport() {
  reportPrinted_ = rptViewer->printReport();

  if ( reportPrinted_ && autoClose_ )
    slotExit();
}

bool FLReportViewer::setReportData( FLSqlQuery * q ) {
  qry_ = q;
  return rptEngine->setReportData( q );
}

bool FLReportViewer::setReportData( FLSqlCursor * t ) {
  return rptEngine->setReportData( t );
}

bool FLReportViewer::setReportData( QDomNode d ) {
  return rptEngine->setFLReportData( d );
}

bool FLReportViewer::setReportTemplate( const QString & t, const QString & style ) {
  if ( !t.isEmpty() )
    template_ = t;
  bool ret = rptEngine->setFLReportTemplate( t );
  if ( !style.isEmpty() ) {
    styleName_ = style;
    rptEngine->setStyleName( style );
  }
  return ret;
}

QSize FLReportViewer::sizeHint() const {
  return rptViewer->sizeHint();
}

void FLReportViewer::setNumCopies( const int numCopies ) {
  rptViewer->setNumCopies( numCopies );
}

void FLReportViewer::setPrintToPos( bool ptp ) {
  rptViewer->setPrintToPos( ptp );
}

void FLReportViewer::setPrinterName( const QString & pName ) {
  rptViewer->setPrinterName( pName );
}

bool FLReportViewer::reportPrinted() {
  return reportPrinted_;
}

void FLReportViewer::setAutoClose( bool b ) {
  if ( embedInParent_ )
    autoClose_ = false;
  else
    autoClose_ = b;
}

void FLReportViewer::setResolution( int dpi ) {
  QString valn;
  FLUtil::writeSettingEntry( "rptViewer/dpi", valn.setNum( dpi ) );
  rptViewer->setResolution( dpi );
}

void FLReportViewer::setPixel( int relDpi ) {
  QString valn;
  rptEngine->setRelDpi( relDpi / 10. );
  FLUtil::writeSettingEntry( "rptViewer/pixel", valn.setNum(( double ) rptEngine->relDpi(), 'f', 2 ) );
}

void FLReportViewer::setDefaults() {
  spnResolution->setValue( 300 );
#if defined (Q_OS_LINUX)
  spnPixel->setValue( 780 );
#else
#if defined(Q_OS_WIN32)
  spnPixel->setValue( 780 );
#else
  spnPixel->setValue( 100 );
#endif
#endif
}

void FLReportViewer::updateReport() {
  if ( qry_ ) {
    rptViewer->clearReport();

    if ( rptEngine )
      rptEngine->deleteLater();

    rptEngine = new FLReportEngine;

    rptViewer->setReportEngine( rptEngine );

    setResolution( spnResolution->value() );

    setPixel( spnPixel->value() );
    setReportTemplate( template_, styleName_ );

    setReportData( qry_ );

    renderReport();
  }

  updateDisplay();
}

FLPicture * FLReportViewer::getCurrentPage() {
  if ( report )
    return new FLPicture( report->getCurrentPage(), this );

  return 0;
}

FLPicture * FLReportViewer::getFirstPage() {
  if ( report )
    return new FLPicture( report->getFirstPage(), this );

  return 0;
}

FLPicture * FLReportViewer::getPreviousPage() {
  if ( report )
    return new FLPicture( report->getPreviousPage(), this );

  return 0;
}

FLPicture * FLReportViewer::getNextPage() {
  if ( report )
    return new FLPicture( report->getNextPage(), this );

  return 0;
}

FLPicture * FLReportViewer::getLastPage() {
  if ( report )
    return new FLPicture( report->getLastPage(), this );

  return 0;
}

FLPicture * FLReportViewer::getPageAt( uint i ) {
  if ( report )
    return new FLPicture( report->getPageAt( i ), this );

  return 0;
}

void FLReportViewer::updateDisplay() {
  rptViewer->slotUpdateDisplay();
}

void FLReportViewer::clearPages() {
  if ( report )
    report->clear();
}

void  FLReportViewer::appendPage() {
  if ( report )
    report->appendPage();
}

int FLReportViewer::getCurrentIndex() {
  if ( report )
    return report->getCurrentIndex();

  return -1;
}

void FLReportViewer::setCurrentPage( int idx ) {
  if ( report )
    report->setCurrentPage( idx );
}

void FLReportViewer::setPageSize( int s ) {
  if ( report )
    report->setPageSize( s );
}

void FLReportViewer::setPageOrientation( int o ) {
  if ( report )
    report->setPageOrientation( o );
}

void FLReportViewer::setPageDimensions( QSize dim ) {
  if ( report )
    report->setPageDimensions( dim );
}

int FLReportViewer::pageSize() {
  if ( report )
    return report->pageSize();

  return -1;
}

int FLReportViewer::pageOrientation() {
  if ( report )
    return report->pageOrientation();

  return -1;
}

QSize FLReportViewer::pageDimensions() {
  if ( report )
    return report->pageDimensions();

  return QSize();
}

int FLReportViewer::pageCount() {
  if ( report )
    return report->pageCount();

  return -1;
}

void FLReportViewer::setStyleName( const QString & style ) {
  styleName_ = style;
}
