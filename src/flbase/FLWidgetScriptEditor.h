/****************************************************************************
** Form interface generated from reading ui file 'FLWidgetScriptEditor.ui'
**
** Created: mar 31. mar 23:52:20 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FLWIDGETSCRIPTEDITOR_H
#define FLWIDGETSCRIPTEDITOR_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

class FLWidgetScriptEditor : public QDialog
{
    Q_OBJECT

public:
    FLWidgetScriptEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FLWidgetScriptEditor();

    QFrame* frMain;
    QFrame* frTools;
    QLabel* lblBuscar;
    QLineEdit* leFind;
    QPushButton* pbSearch;
    QLabel* lblIrA;
    QLineEdit* leGoto;
    QFrame* frTools2;
    QPushButton* pbReplace;
    QPushButton* pbPref;
    QPushButton* pbOk;

protected:
    QVBoxLayout* FLWidgetScriptEditorLayout;
    QVBoxLayout* frMainLayout;
    QHBoxLayout* layout2;
    QSpacerItem* spacer3;
    QHBoxLayout* frToolsLayout;
    QSpacerItem* spacer2;
    QHBoxLayout* frTools2Layout;

protected slots:
    virtual void languageChange();

    virtual bool find(const QString &);
    virtual bool reFind();
    virtual void gotoLine(const QString &);


private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;

};

#endif // FLWIDGETSCRIPTEDITOR_H
