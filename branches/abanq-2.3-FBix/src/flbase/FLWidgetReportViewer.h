/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetReportViewer.ui'
**
** Created: mar 31. mar 23:52:19 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETREPORTVIEWER_H
#define FLWIDGETREPORTVIEWER_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QCheckBox;
class QSpinBox;

class FL_EXPORT FLWidgetReportViewer : public QMainWindow
{
    Q_OBJECT

public:
    FLWidgetReportViewer( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~FLWidgetReportViewer();

    QFrame* frEMail;
    QLabel* textLabel1_3_3;
    QLineEdit* leMailServer;
    QLabel* textLabel1;
    QLineEdit* leDocumento;
    QPushButton* pbEnviar;
    QLabel* textLabel1_2;
    QLineEdit* leDe;
    QLabel* textLabel1_3;
    QLineEdit* lePara;
    QLabel* textLabel1_3_2;
    QLineEdit* leAsunto;
    QTextEdit* leCuerpo;
    QLabel* lblEstado;
    QCheckBox* chkAutoClose;
    QLabel* textLabel1_5;
    QSpinBox* spnPixel;
    QLabel* textLabel1_4;
    QSpinBox* spnResolution;
    QLabel* textLabel1_6;
    QLineEdit* ledStyle;
    QMenuBar *menubar;
    QPopupMenu *fileMenu;
    QToolBar *toolBar;
    QToolBar *toolBar2;
    QAction* imprimir;
    QAction* anterior;
    QAction* siguiente;
    QAction* ultima;
    QAction* salir;
    QAction* primera;
    QAction* zoomup;
    QAction* zoomdown;
    QAction* export_DSV;
    QAction* export_PDF;
    QAction* enviar_email;
    QAction* actualizar;
    QAction* reset;
    QAction* save_page_SVG;
    QAction* save_page_tpl_SVG;
    QAction* mennew_itemAction;
    QAction* load_tpl_SVG;

public slots:
    virtual void slotPrevPage();
    virtual void slotPrintReport();
    virtual void slotFirstPage();
    virtual void exportFileCSVData();
    virtual void slotNextPage();
    virtual void slotLastPage();
    virtual void slotZoomUp();
    virtual void slotZoomDown();
    virtual void exportToPDF();
    virtual void sendEMailPDF();
    virtual void showInitCentralWidget(bool);
    virtual void setAutoClose(bool);
    virtual void setResolution(int);
    virtual void setPixel(int);
    virtual void updateReport();
    virtual void setDefaults();
    virtual void saveSVGStyle();
    virtual void setStyleName(const QString &);
    virtual void saveSimpleSVGStyle();
    virtual void loadSVGStyle();

protected:
    QVBoxLayout* FLWidgetReportViewerLayout;
    QVBoxLayout* frEMailLayout;
    QHBoxLayout* layout7_2;
    QHBoxLayout* layout5;
    QHBoxLayout* layout6;
    QHBoxLayout* layout7;
    QHBoxLayout* layout8;

protected slots:
    virtual void languageChange();

private:
    QPixmap image32518;
    QPixmap image21499;
    QPixmap image49114;
    QPixmap image11018;
    QPixmap image39438;
    QPixmap image23476;
    QPixmap image56470;
    QPixmap image20796;
    QPixmap image14705;
    QPixmap image24355;
    QPixmap image65464;
    QPixmap image19647;
    QPixmap image35899;
    QPixmap image57321;
    QPixmap image32588;
    QPixmap image59073;
    QPixmap image50033;
    QPixmap image44212;

};

#endif // FLWIDGETREPORTVIEWER_H
