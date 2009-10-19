/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetAbout.ui'
**
** Created: mar 31. mar 23:52:19 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETABOUT_H
#define FLWIDGETABOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QFrame;
class QLabel;
class QTextEdit;
class QTextBrowser;
class QPushButton;

class FL_EXPORT FLWidgetAbout : public QDialog
{
    Q_OBJECT

public:
    FLWidgetAbout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FLWidgetAbout();

    QTabWidget* TabWidget2;
    QWidget* tab;
    QFrame* Frame3;
    QLabel* logo;
    QLabel* TextLabel7;
    QLabel* labelVersion;
    QLabel* lblCreditos;
    QWidget* tab_2;
    QTextEdit* TextEdit1;
    QWidget* tab_3;
    QLabel* textLabel2;
    QLabel* textLabel1;
    QWidget* tab_4;
    QTextBrowser* TextBrowser4;
    QPushButton* cerrar;

protected:
    QVBoxLayout* FLWidgetAboutLayout;
    QVBoxLayout* tabLayout;
    QHBoxLayout* layout7;
    QSpacerItem* spacer6;
    QSpacerItem* spacer4;
    QVBoxLayout* Frame3Layout;
    QHBoxLayout* layout9;
    QHBoxLayout* layout12;
    QHBoxLayout* layout8;
    QSpacerItem* spacer5;
    QSpacerItem* spacer4_2;
    QVBoxLayout* tabLayout_2;
    QVBoxLayout* tabLayout_3;
    QVBoxLayout* layout5;
    QSpacerItem* spacer3;
    QVBoxLayout* tabLayout_4;
    QHBoxLayout* Layout1;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;

};

#endif // FLWIDGETABOUT_H
