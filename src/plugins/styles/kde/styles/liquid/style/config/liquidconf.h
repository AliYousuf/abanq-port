#ifndef LIQUID_CONF_H
#define LIQUID_CONF_H

//#include <kstddirs.h>
#include <kcolorbutton.h>
#include <qpushbutton.h>

class QSlider;
class KColorButton;
class QGroupBox;
class QVButtonGroup;
class QRadioButton;
class QCheckBox;
class QLabel;

#define LIQUID_MENU_CHANGE 667

class LiquidStyleConfig : public QWidget
{
    Q_OBJECT
public:

    LiquidStyleConfig(QWidget* parent,const char * name = 0);
    ~LiquidStyleConfig();
    enum Buttons {None=0, StippledBg, StippledBtn,Custom};
public slots:

    void save();
    void load();
    void defaults();

signals:

    void changed(bool);
protected slots:

    void slotBtnGroupClicked(int id);
    void slotColorOptionChanged();
    void slotUsePanelCustomClicked(bool on);
    void slotUseCustomColorClicked(bool on);
    void slotPanelColorChanged(const QColor &c);
    void slotWidgetColorChanged(const QColor &c);
    void slotSliderChanged(int val);
protected:
    void adjustHSV(QImage &img, QImage &dest, const QColor &c,const QColor &bgColor);
    QSlider *opacitySlider;
    KColorButton *colorBtn;
    KColorButton *fgBtn;
    QGroupBox *editGroup;
    QVButtonGroup *btnGroup;
    QCheckBox *shadowBtn;

    QCheckBox *stippleBgBtn, *stipplePanelBtn;
    QSlider *stippleContrastSlider;
    QCheckBox *usePanelCustomBtn, *reverseColorBtn, *animProgressBar;
    QCheckBox *useTbFrameBtn;
    KColorButton *panelColorBtn;

    QCheckBox *useCustomColorBtn;
    KColorButton *radioOnColorBtn, *radioOffColorBtn;
    KColorButton *cbOnColorBtn, *cbOffColorBtn;
    KColorButton *sbSliderColorBtn, *sbGrooveColorBtn;
    KColorButton *tabOnColorBtn, *tabOffColorBtn;
    QLabel *radioOnLbl, *radioOffLbl;
    QLabel *cbOnLbl, *cbOffLbl;
    QLabel *sbSliderLbl, *sbGrooveLbl;
    QLabel *tabOnLbl, *tabOffLbl;

};
#endif



