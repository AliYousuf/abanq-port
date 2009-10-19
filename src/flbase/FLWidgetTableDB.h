/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetTableDB.ui'
**
** Created: mar 31. mar 23:52:20 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETTABLEDB_H
#define FLWIDGETTABLEDB_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QLineEdit;
class QComboBox;
class QWidgetStack;
class QPushButton;
class QTable;
class QButtonGroup;

class FL_EXPORT FLWidgetTableDB : public QWidget
{
    Q_OBJECT

public:
    FLWidgetTableDB( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FLWidgetTableDB();

    QFrame* FrameFind;
    QLabel* TextLabelSearch;
    QLineEdit* lineEditSearch;
    QLabel* TextLabelIn;
    QComboBox* comboBoxFieldToSearch;
    QWidgetStack* tabTable;
    QWidget* tabData;
    QWidget* tabFilter;
    QPushButton* pbClear;
    QTable* tdbFilter;
    QButtonGroup* bgrDataFilter;
    QPushButton* pbData;
    QPushButton* pbFilter;

public slots:
    virtual void putFirstCol( const QString & );

protected:
    QVBoxLayout* FLWidgetTableDBLayout;
    QHBoxLayout* FrameFindLayout;
    QHBoxLayout* layout6;
    QVBoxLayout* tabDataLayout;
    QVBoxLayout* tabFilterLayout;
    QHBoxLayout* layout1;
    QVBoxLayout* bgrDataFilterLayout;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void filterRecords( const QString & );
    virtual void tdbFilterClear();
    virtual void activeTabData( bool );
    virtual void activeTabFilter( bool );


private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;

};

#endif // FLWIDGETTABLEDB_H
