/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetFieldDB.ui'
**
** Created: mar 31. mar 23:52:20 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETFIELDDB_H
#define FLWIDGETFIELDDB_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class FL_EXPORT FLWidgetFieldDB : public QWidget
{
    Q_OBJECT

public:
    FLWidgetFieldDB( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FLWidgetFieldDB();

    QLabel* textLabelDB;
    QPushButton* pushButtonDB;

protected:
    QHBoxLayout* FLWidgetFieldDBLayout;
    QHBoxLayout* lytButtons;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void searchValue();


private:
    QPixmap image0;

};

#endif // FLWIDGETFIELDDB_H
