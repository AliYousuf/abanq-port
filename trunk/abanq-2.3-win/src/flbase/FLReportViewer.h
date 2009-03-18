/***************************************************************************
                       FLReportViewer.h  -  description
                          -------------------
 begin                : vie jun 28 2002
 copyright            : (C) 2002-2004 by InfoSiAL S.L.
 email                : mail@infosial.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FLREPORTVIEWER_H
#define FLREPORTVIEWER_H

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qdom.h>

#include "FLWidgetReportViewer.h"

class MReportViewer;

class MPageCollection;

class FLSqlQuery;

class FLSqlCursor;

class FLReportEngine;

class FLSmtpClient;

class FLPicture;

/**
Visor para previsualizar informes de Kugar

@author InfoSiAL S.L.
*/

class FL_EXPORT FLReportViewer: public FLWidgetReportViewer {

public:

  /**
  constructor

  @param  embedInParent Si es TRUE y se ha indicado un widget padre, el visor intenta
                        incrustarse en la capa principal del padre. Si es FALSE el visor
                        ser� una ventana separada emergente.
  */
  FLReportViewer( QWidget * parent = 0, const char * name = 0, bool embedInParent = false );

  /**
  destructor
  */
  ~FLReportViewer();

  /**
  Muestra el formulario y entra en un nuevo bucle de eventos.
  */
  void exec();

  /**
  Obtiene una versi�n csv de los datos del informe (una vez ejecutado)
  */
  QString csvData();

  /**
  Carga el informe en el visor.

  @return TRUE si todo ha ido bien
  */
  bool renderReport( const int initRow = 0, const int initCol = 0 );

  /**
  Establece los datos del informe a partir de una consulta

  @param q Objeto FLSqlQuery con la consulta de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setReportData( FLSqlQuery * q );

  /**
  Establece los datos del informe a partir de una tabla

  @param t Objeto FLSqlCursor con  la tabla de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setReportData( FLSqlCursor * t );

  /**
  Establece los datos del informe a partir de un documento xml

  @param n Objeto FLDomDocument con  la tabla de la que se toman los datos
  @return TRUE si todo ha ido bien
  */
  bool setReportData( QDomNode n );

  /**
  Establece la plantilla para el informe.

  El nombre de la plantilla corresponde con el nombre del fichero con extesi�n ".kut".

  @param  t     Nombre de la plantilla
  @param  style Nombre de la hoja de estilo a aplicar
  @return TRUE si todo ha ido bien
  */
  bool setReportTemplate( const QString & t, const QString & style = QString::null );

  /**
  Reimplementaci�n de QWidget::sizeHint()
  */
  QSize sizeHint() const;

  /**
  Imprime directamente el informe sin mostrarlo
  */
  void slotPrintReport();

  /**
  Imprime el informe en un fichero PS
  */
  void slotPrintReportToPS( const QString & outPsFile );

  /**
  Imprime el informe en un fichero PDF
  */
  void slotPrintReportToPDF( const QString & outPdfFile );

  /**
  Establece el n�mero de copias por defecto a imprimir
  */
  void setNumCopies( const int numCopies );

  /**
  Establece si el informe se debe imprimir en una impresora ESC/POS
  */
  void setPrintToPos( bool ptp );

  /**
  Establece el nombre de la impresora a la que imprimir.

  Si se establece el nombre de la impresora no se mostrar� el cuadro de dialogo de impresi�n, y se
  usar� esa impresora para imprimir directamente. Para que el cuadro de di�logo de impresi�n se muestre bastar�
  con establecer un nombre vac�o; setPrinterName( QString::null ).
  */
  void setPrinterName( const QString & pName );

  /**
  Devuelve si el �ltimo informe ha sido imprimido en impresora o archivo.
  */
  bool reportPrinted();

protected:

  /**
  Captura evento cerrar
  */
  void closeEvent( QCloseEvent * e );

private slots:

  /**
  Muestra la primera p�gina del informe
  */
  void slotFirstPage();

  /**
  Muestra la �tlima p�gina del informe
  */
  void slotLastPage();

  /**
  Muestra la siguiente p�gina del informe
  */
  void slotNextPage();

  /**
  Muestra la anterior p�gina del informe
  */
  void slotPrevPage();

  /**
  Cierra el visor
  */
  void slotExit();

  /**
  Aumenta zoom de la p�gina actual
  */
  void slotZoomUp();

  /**
  Disminuye zoom de la p�gina actual
  */
  void slotZoomDown();

  /**
  Exporta a un fichero de disco la version CSV del informe
  */
  void exportFileCSVData();

  /**
  Exporta el informe a un fichero en formato PDF
  */
  void exportToPDF();

  /**
  Exporta el informe a un fichero en formato PDF y lo envia por correo el�ctronico
  */
  void sendEMailPDF();

  /**
  Muestra u oculta la ventana principal inicial

  @param show TRUE la muestra, FALSE la oculta
  */
  void showInitCentralWidget( bool show );

  /**
  Guarda como plantilla de estilo SVG
  */
  void saveSVGStyle();

  /**
  Guarda la p�gina actual como plantilla de estilo SVG simplificada ( s�lo los campos de datos )
  */
  void saveSimpleSVGStyle();

  /**
  Carga y aplica una plantilla de estilo SVG
  */
  void loadSVGStyle();

public slots:

  /**
  Establece si el visor debe cerrarse autom�ticamente tras imprimir el informe

  @param b TRUE para cierre autom�tico, FALSE para desactivar cierre autom�tico
  */
  void setAutoClose( bool b );

  /**
  Establece la resolucion de la impresora

  @param dpi Resolucion en puntos por pulgada
  */
  void setResolution( int dpi );
  void setPixel( int relDpi );
  void setDefaults();
  void updateReport();

  /**
  Establece el nombre del estilo
  */
  void setStyleName( const QString & style );

private:

  /**
  Almacena si se ha abierto el formulario con el m�todo FLReportViewer::exec()
  */
  bool loop;

  /**
  Indica si el �ltimo informe fue impreso, es decir, enviado a impresora o archivo
  */
  bool reportPrinted_;

  /**
  Visor b�sico de Kugar
  */
  MReportViewer * rptViewer;

  /**
  Motor de informes de FacturaLUX
  */
  FLReportEngine * rptEngine;

  /**
  Ventana principal inicial
  */
  QWidget * initCentralWidget_;

  /**
  Cliente SMTP para enviar el informe por correo electr�nico
  */
  FLSmtpClient * smtpClient_;

  /**
  Indica si el visor debe cerrarse autom�ticamente despues de imprimir
  */
  bool autoClose_;

  /**
  Colecci�n de paginas del informe ( lista de QPictures )
  */
  MPageCollection * report;

  /**
  Indica si el visor es un objeto incrustado en el padre o una venta emergente
  */
  bool embedInParent_;

  /**
  Nombre de la plantilla del informe
  */
  QString template_;

  /**
  Guarda la consulta origen
  */
  FLSqlQuery * qry_;

  /**
  Nombre del estilo del informe
  */
  QString styleName_;

public:

  FLPicture * getCurrentPage();
  FLPicture * getFirstPage();
  FLPicture * getPreviousPage();
  FLPicture * getNextPage();
  FLPicture * getLastPage();
  FLPicture * getPageAt( uint i );

  void  updateDisplay();
  void  clearPages();
  void  appendPage();
  int   getCurrentIndex();
  void  setCurrentPage( int idx );
  void  setPageSize( int s );
  void  setPageOrientation( int o );
  void  setPageDimensions( QSize dim );
  int   pageSize();
  int   pageOrientation();
  QSize pageDimensions();
  int   pageCount();
};

#endif
