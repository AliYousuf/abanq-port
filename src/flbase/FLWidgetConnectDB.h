/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetConnectDB.ui'
**
** Created: mar 31. mar 23:52:19 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETCONNECTDB_H
#define FLWIDGETCONNECTDB_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QComboBox;
class QLineEdit;
class QToolButton;
class QPushButton;

class FLWidgetConnectDB : public QDialog
{
    Q_OBJECT

public:
    FLWidgetConnectDB( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FLWidgetConnectDB();

    QFrame* frameConnect;
    QLabel* textLabelDataBase;
    QComboBox* comboBoxNameDB;
    QLabel* TextLabelUser;
    QLineEdit* lineEditUser;
    QLabel* TextLabelPassword;
    QLineEdit* lineEditPassword;
    QToolButton* pbMarks;
    QPushButton* pushButtonConnect;
    QPushButton* pushButtonCancel;
    QPushButton* pbnMore;
    QFrame* frMore;
    QLabel* lblCredito;
    QComboBox* comboBoxMarks;
    QToolButton* pbAddMark;
    QToolButton* pbRemoveMark;
    QLabel* textLabelDB;
    QComboBox* comboBoxDB;
    QLabel* TextLabelHost;
    QLineEdit* lineEditHost;
    QLabel* TextLabelPort;
    QLineEdit* lineEditPort;

protected:
    QVBoxLayout* FLWidgetConnectDBLayout;
    QSpacerItem* spacer3;
    QSpacerItem* spacer6;
    QHBoxLayout* layout12;
    QHBoxLayout* frameConnectLayout;
    QVBoxLayout* layout12_2;
    QSpacerItem* spacer4;
    QHBoxLayout* layout3;
    QHBoxLayout* layout5;
    QHBoxLayout* layout6;
    QHBoxLayout* layout25;
    QVBoxLayout* frMoreLayout;
    QHBoxLayout* layout25_2;
    QHBoxLayout* layout3_2;
    QHBoxLayout* layout4_2;
    QHBoxLayout* layout6_2;

protected slots:
    virtual void languageChange();

    virtual void init();
    virtual void destroy();
    virtual void cancel();
    virtual void tryConnect();
    virtual void driverChanged(const QString&);
    virtual void addMark();
    virtual void removeMark();
    virtual void selectMark(const QString &);
    virtual void popupMarks();


private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;

};

#endif // FLWIDGETCONNECTDB_H
