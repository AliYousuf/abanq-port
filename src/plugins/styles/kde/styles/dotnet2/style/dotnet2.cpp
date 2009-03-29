/*
 * KDE3 dotNET2 Style (version 1.5)
 * Copyright (C) 2001-2002, Chris Lee <clee@kde.org>
 *                          Carsten Pfeiffer <pfeiffer@kde.org>
 *                           Karol Szwed <gallium@kde.org>
 *
 * Drawing routines completely reimplemented from KDE3 HighColor, which was
 * originally based on some stuff from the KDE2 HighColor.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <qstyleplugin.h>
#include <qstylefactory.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qtabbar.h>
#include <qprogressbar.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qscrollbar.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qdrawutil.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qpixmapcache.h>
#include <qslider.h>
#include <qsettings.h>
#include <kpixmap.h>
#include <qlabel.h>

#include "dotnet.h"
//#include "moc_dotnet.cpp"

/* This code registers the style with QStyleFactory, which makes it possible
 * for any code that uses QStyle to find out about it. */
// -- Style Plugin Interface -------------------------
class dotNET2stylePlugin : public QStylePlugin
{
public:
    dotNET2stylePlugin() {}
    ~dotNET2stylePlugin() {}

    QStringList keys() const {
        return QStringList() << "dotNET2";
    }

    QStyle* create( const QString& key ) {
        if (key == "dotnet2")
            return new dotNET2style;
        return 0;
    }
};

Q_EXPORT_PLUGIN( dotNET2stylePlugin )

// ---------------------------------------------------

dotNET2style::dotNET2style() : KStyle( AllowMenuTransparency ), kickerMode(false)
{
    int hue,sat,val;
    hoverWidget = 0;

    QColorGroup cg = QApplication::palette().active();

    c1 = new QColor(MYSELRED(cg)+cg.highlight().red(),MYSELGREEN(cg)+cg.highlight().green(),MYSELBLUE(cg)+cg.highlight().blue());
    c2 = new QColor(MYRED(cg)+cg.background().red(),MYGREEN(cg)+cg.background().green(),MYBLUE(cg)+cg.background().blue());
    c3 = new QColor(cg.base().red()-MYRED(cg),cg.base().green()-MYGREEN(cg),cg.base().blue()-MYBLUE(cg));
    csel = new QColor();

    c1->getHsv(&hue,&sat,&val);
    sat = sat - 25;
    val = val + 8;
    sat = (sat<0)?0:sat;
    val = (val>255)?255:val;
    csel->setHsv(hue,sat,val);

    if (qApp->inherits("KApplication")) {
        connect( qApp, SIGNAL( kdisplayPaletteChanged() ), SLOT( paletteChanged() ));
    }

    QSettings settings;
    useTextShadows = settings.readBoolEntry("/KStyle/Settings/UseTextShadows", false);
    reverseLayout = QApplication::reverseLayout();
}


dotNET2style::~dotNET2style()
{}

void dotNET2style::paletteChanged()
{

    int hue,sat,val;
    hoverWidget = 0;

    QColorGroup cg = QApplication::palette().active();

    c1 = new QColor(MYSELRED(cg)+cg.highlight().red(),MYSELGREEN(cg)+cg.highlight().green(),MYSELBLUE(cg)+cg.highlight().blue());
    c2 = new QColor(MYRED(cg)+cg.background().red(),MYGREEN(cg)+cg.background().green(),MYBLUE(cg)+cg.background().blue());
    c3 = new QColor(cg.base().red()-MYRED(cg),cg.base().green()-MYGREEN(cg),cg.base().blue()-MYBLUE(cg));
    csel = new QColor();

    c1->getHsv(&hue,&sat,&val);
    sat = sat - 25;
    val = val + 8;
    sat = (sat<0)?0:sat;
    val = (val>255)?255:val;
    csel->setHsv(hue,sat,val);
}

bool dotNET2style::inheritsKHTML(const QWidget* w) const
{
    if (w->parentWidget(true) &&
            w->parentWidget(true)->parentWidget(true) &&
            w->parentWidget(true)->parentWidget(true)->parentWidget(true) &&
            w->parentWidget(true)->parentWidget(true)->parentWidget(true)->inherits("KHTMLView")) {
        return true;
    } else {
        return false;
    }
}

void dotNET2style::polish(QWidget* widget)
{
    if (!qstrcmp(qApp->argv()[0], "kicker") || widget->inherits("Kicker"))
        kickerMode = true;

    if (widget->isTopLevel())
        return;

    // we can't simply set a palette -- upon color-theme changes, we have
    // to update the palette again.
    bool extraPalette = false;

    if (widget->inherits("QPushButton") || widget->inherits("QComboBox") ||
            widget->inherits("QSpinWidget") || widget->inherits("QSlider") || widget->inherits("QScrollBar") ||
            widget->inherits("QCheckBox")   || widget->inherits("QRadioButton") || widget->inherits("QToolButton")) {
        widget->installEventFilter(this);
        //updatePalette( (QComboBox*) widget );
        extraPalette = true;
    } else {
        KStyle::polish(widget);
    }
}

void dotNET2style::unPolish(QWidget* widget)
{
    KStyle::unPolish(widget);

    if (widget->inherits("QPushButton") || widget->inherits("QComboBox") ||
            widget->inherits("QSpinWidget") || widget->inherits("QSlider") || widget->inherits("QScrollBar") ||
            widget->inherits("QCheckBox") || widget->inherits("QRadioButton") || widget->inherits("QToolButton")) {
        widget->removeEventFilter(this);
    }
}

void dotNET2style::renderMenuBlendPixmap(KPixmap &pix, const QColorGroup &cg, const QPopupMenu *popup) const
{
    QPainter p( &pix );
    if (reverseLayout) {
        p.fillRect( popup->frameRect().width()-22, 0, 22, pix.height(), cg.mid() );
        p.fillRect( 0, 0, popup->frameRect().width()-22, pix.height(), cg.background().light() );
    } else {
        p.fillRect( 0, 0, pix.width(), pix.height(), cg.background().light() );
        p.fillRect( popup->frameRect().left() + 1, 0, 22, pix.height(), cg.mid() );
    }
}

/* This is the code that renders 90+% of the buttons in dotNET2.
 * Changing the code here will most likely affect EVERYTHING.
 * If you want to modify the style, this is a good place to start.
 * Also, take a look at the sizeMetric function and play with the
 * widths that things return for different Frame elements.
 */
void dotNET2style::renderButton(QPainter *p, const QRect &r, const QColorGroup &cg,
                                bool sunken, bool mouseover) const
{
    if (sunken) {
        p->setPen(cg.highlight());
        p->setBrush(*csel);
        p->drawRect(r);
    } else if (mouseover) {
        p->setPen(cg.highlight().dark(150));
        p->setBrush(*c1);
        p->drawRect(r);
    } else {
        p->setPen(c2->dark());
        p->setBrush(*c2);
        p->drawRect(r);
    }
}

/* This is the code that renders the slider handles on scrollbars. It draws
 * them just like normal buttons, by calling renderButton, but it also adds
 * very subtle grips to the slider handle.
 */
void dotNET2style::renderSlider(QPainter *p,
                                const QRect &r,
                                const QColorGroup &g) const
{
    int x, y, w, h;
    int llx, lly, urx, ury;

    r.rect(&x,&y,&w,&h);

    llx = (x + (w/2) - 4);
    lly = (y + (h/2) + 3);
    urx = (x + (w/2) + 2);
    ury = (y + (h/2) - 3);

    renderButton(p, r, g, false);
}

/* This function does the rendering for most of the panels in dotNET2.
 * The QCOORD stuff is used to provide the slightly-rounded corners.
 * Changing this function will have pretty widespread effects. Also a good
 * place to start if you're looking to make your own style.
 */
void dotNET2style::renderPanel(QPainter *p, const QRect &r, const QColorGroup &cg) const
{
    p->save();

    p->setPen(cg.background().dark());
    p->setBrush(cg.background());
    p->drawRect(r);

    p->restore();

}


void dotNET2style::drawKStylePrimitive(KStylePrimitive kpe,
                                       QPainter *p,
                                       const QWidget* widget,
                                       const QRect &r,
                                       const QColorGroup &cg,
                                       SFlags flags,
                                       const QStyleOption& opt) const
{
    //  SLIDER
    //  ------
    switch ( kpe ) {
    case KPE_SliderGroove: {
        const QSlider* slider = (const QSlider*)widget;
        int x, y, w, h;
        r.rect(&x, &y, &w, &h);
        bool horizontal = slider->orientation() == Horizontal;
        int gcenter = (horizontal ? h : w) / 2;

        if (horizontal) {
            gcenter += y;
            p->setPen (cg.background().dark());
            p->drawLine(x, gcenter, x+w, gcenter);
            gcenter++;
            p->setPen (cg.background().light());
            p->drawLine(x, gcenter, x + w, gcenter);
        } else {
            gcenter += x;
            p->setPen (cg.background().dark());
            p->drawLine(gcenter, y, gcenter, y + h);
            gcenter++;
            p->setPen (cg.background().light());
            p->drawLine(gcenter, y, gcenter, y + h);
        }
        break;
    }

    case KPE_SliderHandle: {
        renderButton(p, r, cg,false,(hoverWidget == widget));
        break;
    }

    default:
        KStyle::drawKStylePrimitive(kpe, p, widget, r, cg, flags, opt);
    }
}


// This function draws primitive elements as well as their masks.
void dotNET2style::drawPrimitive(PrimitiveElement pe,
                                 QPainter *p,
                                 const QRect &r,
                                 const QColorGroup &cg,
                                 SFlags flags,
                                 const QStyleOption &opt ) const
{
    bool down   = flags & Style_Down;
    bool on     = flags & Style_On;
    bool horiz  = flags & Style_Horizontal;
    bool mouseover = flags & Style_MouseOver;

    int x, x2, y, y2, w, h;
    r.rect(&x, &y, &w, &h);
    r.coords(&x, &y, &x2, &y2);


    switch (pe) {
        // BUTTONS
        // -------
    case PE_ButtonBevel:
    case PE_ButtonTool:
    case PE_ButtonDropDown:
    case PE_HeaderSection:
    case PE_ButtonCommand: {
        if (on || down) {
            renderButton(p, r, cg, true,mouseover);
            p->setPen(cg.foreground());

        } else {
            renderButton(p, r, cg, false, mouseover);
        }

        break;
    }

    case PE_ButtonDefault: {
        QRect sr = r;
        p->setPen(QPen::NoPen);
        p->setBrush(cg.background().dark(105));
        p->drawRoundRect(sr, 25, 25);
        p->setBrush(cg.background().dark(110));
        sr.setCoords(sr.x() + 1, sr.y() + 1, sr.width() - 2, sr.height() - 2);
        p->drawRoundRect(sr, 25, 25);
        p->setBrush(cg.background().dark(115));
        sr.setCoords(sr.x() + 1, sr.y() + 1, sr.width() - 2, sr.height() - 2);
        p->drawRoundRect(sr, 25, 25);
        break;
    }

    case PE_ScrollBarSlider: {
        renderSlider(p, r, cg);
        break;
    }

    case PE_ScrollBarAddPage:
    case PE_ScrollBarSubPage: {
        if (on || down) {
            p->fillRect(r, QBrush(cg.mid().dark()));
        } else {
            p->fillRect(r, QBrush(cg.background()));
        }
        p->fillRect(r, QBrush(cg.background().light(), Dense4Pattern));
        break;
    }

    // SCROLLBAR BUTTONS
    // -----------------
    case PE_ScrollBarSubLine: {
        drawPrimitive(PE_ButtonBevel, p, r, cg, flags);
        p->setPen(down ? cg.highlightedText() : cg.foreground());
        drawPrimitive((horiz ? PE_ArrowLeft : PE_ArrowUp), p, r, cg, flags);
        break;
    }

    case PE_ScrollBarAddLine: {
        drawPrimitive(PE_ButtonBevel, p, r, cg, flags);
        p->setPen(down ? cg.highlightedText() : cg.foreground());
        drawPrimitive((horiz ? PE_ArrowRight : PE_ArrowDown), p, r, cg, flags);
        break;
    }

    // CHECKBOXES
    // ----------
    case PE_Indicator: {
        p->setPen(flags & Style_Enabled ? cg.highlight(): cg.background().dark());
        p->setBrush(flags & Style_Enabled ? *csel /*cg.light()*/ : cg.background());
        p->drawRect(r);
        //p->setPen(cg.background().dark(115));
        //p->drawLine(x + 1, y + 1, w - 2, y + 1);
        //p->drawLine(x + 1, y + 1, x + 1, h - 2);

        break;
    }

    case PE_IndicatorMask: {
        p->fillRect (r, color1);
        break;
    }

    // RADIOBUTTONS
    // ------------
    case PE_ExclusiveIndicator: {
        const QCOORD outline[] = {
            0, 7, 0, 5, 1, 4, 1, 3, 2, 2, 3, 1, 4, 1, 5, 0, 7, 0, 8, 1, 9, 1, 10, 2, 11, 3, 11, 4, 12, 5, 12, 7, 11, 8, 11, 9, 10, 10, 9, 11, 8, 11, 7, 12, 5, 12, 4, 11, 3, 11, 2, 10, 1, 9, 1, 8
        };
        const QCOORD indark[] = {
            1, 7, 1, 6, 1, 5, 2, 4, 2, 3, 3, 2, 4, 2, 5, 1, 6, 1, 7, 1
        };

        p->fillRect(r, cg.background());
        p->setPen(cg.background().dark());
        p->setBrush(flags & Style_Enabled ? cg.light() : cg.background());
        p->drawPolygon(QPointArray(28, outline));
        p->setPen(cg.background().dark(115));
        p->drawPoints(QPointArray(10, indark));
        break;
    }

    case PE_ExclusiveIndicatorMask: {
        const QCOORD outline[] = {
            0, 7, 0, 5, 1, 4, 1, 3, 2, 2, 3, 1, 4, 1, 5, 0, 7, 0, 8, 1, 9, 1, 10, 2, 11, 3, 11, 4, 12, 5, 12, 7, 11, 8, 11, 9, 10, 10, 9, 11, 8, 11, 7, 12, 5, 12, 4, 11, 3, 11, 2, 10, 1, 9, 1, 8
        };
        p->fillRect(r, color0);
        p->setPen(color1);
        p->setBrush(color1);
        p->drawPolygon(QPointArray(28, outline));
        break;
    }

    // GENERAL PANELS
    // --------------
    case PE_Splitter: {
        int k;
        p->fillRect(r, cg.background());
        p->setBrush(cg.background().dark());

        if (horiz) {
            for (k = r.center().y() - 9; k < r.center().y() + 9; k += 3) {
                p->drawLine(x + 2, k, w - 2, k);
            }
        } else {
            for (k = r.center().x() - 9; k < r.center().x() + 9; k += 3) {
                p->drawLine(k, y + 2, k, h - 2);
            }
        }

        break;
    }

    case PE_PanelGroupBox:
    case PE_GroupBoxFrame: {
        renderPanel(p, r, cg);
    }
    case PE_WindowFrame:
    case PE_Panel: {
        renderPanel(p, r, cg);
        break;
    }
    case PE_PanelLineEdit: {
        renderPanel(p, r, cg);
        break;
    }
    case PE_StatusBarSection: {
        renderPanel(p, r, cg);
        break;
    }
    case PE_TabBarBase: // Still not sure what this one does
    case PE_PanelTabWidget: {
        p->fillRect(r, cg.background());

        p->setPen(cg.background().light());
        p->drawLine(x,y,x,y+h-1);
        p->drawLine(x,y,x+w-1,y);
        p->setPen(cg.background().dark(175));
        p->drawLine(x+w-1,y,x+w-1,y+h-1);
        p->drawLine(x,y+h-1,x+w-1,y+h-1);


        break;
    }
    case PE_PanelPopup: {
        //QColor c2 = QColor(MYRED(cg)+cg.background().red(),MYGREEN(cg)+cg.background().green(),MYBLUE(cg)+cg.background().blue());
        p->setPen(c2->dark());
        p->drawRect(r);
        break;
    }

    // MENU / TOOLBAR PANEL
    // --------------------
    case PE_PanelMenuBar:    // Menu
    case PE_PanelDockWindow: { // Toolbar
        QWidget* w = dynamic_cast<QWidget*>(p->device());
        if (w && w->backgroundMode() == PaletteButton)
            w->setBackgroundMode(PaletteBackground);
        p->fillRect(r, cg.brush(QColorGroup::Background));
        /*   p->setPen(cg.background());
           p->setBrush(cg.background());
                    p->drawRect(r);*/
        break;
    }

    // TOOLBAR/DOCK WINDOW HANDLE
    // --------------------------
    case PE_DockWindowResizeHandle: {
        renderButton(p, r, cg,false, mouseover);
        break;
    }

    case PE_DockWindowHandle: {
        int k;

        p->fillRect(r, cg.background());
        p->setPen(cg.background().dark());

        if (horiz) {
            for (k = r.center().y() - 9; k < r.center().y() + 9; k += 3) {
                p->drawLine(x + 2, k, w - 2, k);
            }
        } else {
            for (k = r.center().x() - 9; k < r.center().x() + 9; k += 3) {
                p->drawLine(k, y + 2, k, h - 2);
            }
        }

        break;
    }

    // TOOLBAR SEPARATOR
    // -----------------
    case PE_DockWindowSeparator: {
        break;
    }

    case PE_CheckMark: {
        int x = r.center().x() - 3, y = r.center().y() - 3;
        const QCOORD check[] = {
            x, y + 2, x, y + 4, x + 2, y + 6, x + 6, y + 2, x + 6, y, x + 2, y + 4
        };
        const QPointArray a(6, check);

        p->setPen(flags & Style_Down ? /*cg.highlight().light() :*/ cg.highlight() :p->pen());
        p->setBrush(flags & Style_Down ? /*cg.highlight().light() :*/ cg.highlight() :p->pen().color());

        p->drawPolygon(a);

        break;
    }

    case PE_SpinWidgetUp:
    case PE_SpinWidgetDown:
    case PE_HeaderArrow:
    case PE_ArrowUp:
    case PE_ArrowDown:
    case PE_ArrowLeft:
    case PE_ArrowRight: {
        QPointArray a;

        switch (pe) {
        case PE_SpinWidgetUp:
        case PE_ArrowUp: {
            a.setPoints(7, u_arrow);
            break;
        }
        case PE_SpinWidgetDown:
        case PE_ArrowDown: {
            a.setPoints(7, d_arrow);
            break;
        }
        case PE_ArrowLeft: {
            a.setPoints(7, l_arrow);
            break;
        }
        case PE_ArrowRight: {
            a.setPoints(7, r_arrow);
            break;
        }
        default: {
            if (flags & Style_Up) {
                a.setPoints(7, u_arrow);
            } else {
                a.setPoints(7, d_arrow);
            }
        }
        }

        p->save();

        if (flags & Style_Down) {
            p->translate(pixelMetric(PM_ButtonShiftHorizontal),
                         pixelMetric(PM_ButtonShiftVertical));
        }

        if (flags & Style_Enabled) {
            a.translate((r.x() + r.width()/2), (r.y() + r.height()/2));
            if (p->pen() == QPen::NoPen) {
                p->setPen(cg.buttonText());
            }
            p->drawLineSegments(a, 0, 3);
            p->drawPoint(a[6]);
        } else {
            a.translate((r.x() + r.width()/2)+1, (r.y() + r.height()/2)+1);
            if (p->pen() == QPen::NoPen) {
                p->setPen(cg.highlightedText());
            }
            p->drawLineSegments(a, 0, 3);
            p->drawPoint(a[6]);

            a.translate(-1,-1);
            if (p->pen() == QPen::NoPen) {
                p->setPen(cg.mid());
            }
            p->drawLineSegments(a, 0, 3);
            p->drawPoint(a[6]);
        }

        p->restore();
        break;
    }

    default: {
        return KStyle::drawPrimitive(pe, p, r, cg, flags, opt);
    }
    }
}

void dotNET2style::drawControl(ControlElement element,
                               QPainter *p,
                               const QWidget *widget,
                               const QRect &r,
                               const QColorGroup &cg,
                               SFlags flags,
                               const QStyleOption& opt) const
{
    if ( widget == hoverWidget) {
        flags |= Style_MouseOver;
    }

    switch (element) {
        // CHECKBOXES
        // ----------
    case CE_CheckBox: {
        drawPrimitive(PE_Indicator, p, r, cg, flags);
        if (flags & Style_On || flags & Style_Down) {
            p->setPen(cg.foreground());
            drawPrimitive(PE_CheckMark, p, r, cg, flags);
        }
        break;
    }

    // PROGRESSBAR
    // -----------
    case CE_ProgressBarLabel: {
        QProgressBar *pb = (QProgressBar *)widget;

        if (pb->totalSteps()) {
            const int text_flags = AlignVCenter | AlignHCenter | ShowPrefix |
                                   DontClip | SingleLine;
            QFont qf = p->font();
            QString qs = QString();
            qf.setBold(true);
            p->setFont(qf);

            p->save();
            qs.setNum((int) (((double)pb->progress() / (double)pb->totalSteps())*100));
            qs.append("%");

            if (useTextShadows) {
                p->setPen(cg.background().dark(115));
                p->drawText(r.x()+1, r.y()+1, r.width(), r.height(), text_flags, qs);
            }

            if (flags & Style_Enabled) {
                p->setPen(flags & Style_Selected ? cg.foreground() : cg.foreground().light());
            } else {
                p->setPen(cg.mid().dark());
            }

            p->drawText(r, text_flags, qs);

            p->restore();
        }
        break;
    }
    case CE_ProgressBarGroove: {
        p->save();

        p->setPen(cg.highlight());
        p->setBrush(cg.base());
        p->drawRect(r);

        p->restore();
        break;
    }

    case CE_ProgressBarContents: {
        QProgressBar *pb = (QProgressBar *)widget;

        if (pb->totalSteps()) {
            int x, y, w, h;
            double percent = (double)pb->progress() / (double)pb->totalSteps();

            r.rect(&x, &y, &w, &h);

            p->fillRect(x, y, w, h, cg.base());
            if (reverseLayout) {
                x = w - (int)(w * percent);
                w = w - x;
            } else {
                w = (int)(w * percent);
            }

            p->fillRect(x-1, y-1, w+2, h+2, *c1);

            if (pb->progress() < pb->totalSteps()) {
                p->setPen(cg.highlight());
                p->drawLine(x + w, y-1, x + w, h+2);
            }
            p->setPen(cg.foreground());

        }
        break;
    }

    // RADIOBUTTONS
    // ------------
    case CE_RadioButton: {
        drawPrimitive(PE_ExclusiveIndicator, p, r, cg, flags);

        if (flags & Style_On || flags & Style_Down) {
            QCOORD center[] = { 4, 5, 4, 7, 5, 8, 7, 8, 8, 7, 8, 5, 7, 4, 5, 4 };
            QPointArray c(8, center);
            p->setPen(flags & Style_Down ? cg.highlight() : cg.text());
            p->setBrush(flags & Style_Down ? cg.highlight() : cg.text());
            p->drawPolygon(c);
        }
        break;
    }

    // TABS
    // ----
    case CE_TabBarTab: {
        const QTabBar * tb = (const QTabBar *) widget;
        QTabBar::Shape tbs = tb->shape();
        bool selected = flags & Style_Selected;
        int x, x2, y, y2, w, h;
        r.rect(&x, &y, &w, &h);
        r.coords(&x, &y, &x2, &y2);

        switch (tbs) {
        case QTabBar::RoundedAbove: {
            if (selected) {
                QRect r2(x , y, w, h);
                p->fillRect(r2, cg.background().light(115));

                QRect r3(x, y+3, w - 1, h);
                p->fillRect(r3, cg.background());

                p->setPen(cg.background().light());
                p->drawLine(x, y+h-1, x, y+3);
                p->drawLine(x, y+3, x+w-1, y+3);

                p->setPen(QColor(0,0,0));
                p->drawLine(x+w-1, y+4, x+w-1, y+h-1);
            } else {
                if ( tb->indexOf( opt.tab()->identifier() ) == 0 ) {
                    QRect r2(x , y, w, h);
                    p->fillRect(r2, cg.background().light(115));
                } else {
                    QRect r2(x+1 , y, w-1, h);
                    p->fillRect(r2, cg.background().light(115));
                }

                p->setPen(cg.background().light());
                p->drawLine(x, y+h-1, x+w, y+h-1);

                p->setPen(cg.background().dark(175));
                p->drawLine(x+w-1, y+5, x+w-1, y+h-5);
            }
            break;
        }

        case QTabBar::RoundedBelow: {
            if (selected) {
                QRect r2(x, y, w, h);
                p->fillRect(r2, cg.background().light(115));

                QRect r3(x, y+3, w-1, h);
                p->fillRect(r3, cg.background());

                p->setPen(cg.background().light());
                p->drawLine(x, y+h-1, x, y+3);
                p->drawLine(x, y+3, x+w-1, y+3);
                p->setPen(QColor(0,0,0));
                p->drawLine(x+w-1, y+4, x+w-1, y+h-1);
            } else {
                QRect r2(x, y, w, h);
                p->fillRect(r2, cg.background().light(115));

                p->setPen(cg.background().light());
                p->drawLine(x, y+h-1, x+w, y+h-1);

                p->setPen(cg.background().dark(175));
                p->drawLine(x+w-1, y+5, x+w-1, y+h-5);
            }
            break;
        }

        default:
            KStyle::drawControl(element, p, widget, r, cg, flags, opt);
        }

        break;
    }

    case CE_TabBarLabel: {
        const int text_flags = AlignVCenter | AlignHCenter | ShowPrefix |
                               DontClip | SingleLine;

        //p->save();
        QFont qf = p->font();

        if (useTextShadows) {
            p->setPen(cg.background().dark(115));
            p->drawText(r.x()+1, r.y()+1, r.width(), r.height(), text_flags, opt.tab()->text());
        }

        if (flags & Style_Enabled) {
            p->setPen(flags & Style_Selected ? cg.foreground() : cg.foreground().light());

            if ( flags & Style_Selected) {
                qf.setBold(true);
                p->setFont(qf);
            }
        } else {
            p->setPen(cg.mid().dark());
        }

        if (!flags & Style_Selected) {
            p->translate(pixelMetric(PM_ButtonShiftHorizontal),
                         pixelMetric(PM_ButtonShiftVertical));
        }

        p->drawText(r, text_flags, opt.tab()->text());

        //p->restore();
        break;
    }

    case CE_PushButton: {
        QPushButton *button = (QPushButton *)widget;
        QRect br = r;
        bool btnDefault = button->isDefault();

        if (btnDefault || button->autoDefault()) {
            static int di = pixelMetric(PM_ButtonDefaultIndicator);
            br.addCoords(di, di, -di, -di);
        }

        if (btnDefault) {
            drawPrimitive(PE_ButtonDefault, p, r, cg, flags);
        }

        drawPrimitive(PE_ButtonBevel, p, br, cg, flags);
        //      drawPrimitive(PE_ButtonBevel, p, r, cg, flags);
        break;
    }

    case CE_PushButtonLabel: {
        int x, y, w, h;
        r.rect( &x, &y, &w, &h );

        const QPushButton* button = static_cast<const QPushButton *>( widget );
        bool active = button->isOn() || button->isDown();
        bool cornArrow = false;

        if ( active ) {
            flags |= Style_Sunken;
        }

        // Does the button have a popup menu?
        if ( button->isMenuButton() ) {
            int dx = pixelMetric( PM_MenuButtonIndicator, widget );
            if ( button->iconSet() && !button->iconSet()->isNull()  &&
                    (dx + button->iconSet()->pixmap (QIconSet::Small, QIconSet::Normal, QIconSet::Off ).width()) >= w ) {
                cornArrow = true; //To little room. Draw the arrow in the corner, don't adjust the widget
            } else {
                drawPrimitive( PE_ArrowDown, p, visualRect( QRect(x + w - dx - 8, y + 2, dx, h - 4), r ),
                               cg, flags, opt );
                w -= dx;
            }
        }

        // Draw the icon if there is one
        if ( button->iconSet() && !button->iconSet()->isNull() ) {
            QIconSet::Mode  mode  = QIconSet::Disabled;
            QIconSet::State state = QIconSet::Off;

            if (button->isEnabled())
                mode = button->hasFocus() ? QIconSet::Active : QIconSet::Normal;
            if (button->isToggleButton() && button->isOn())
                state = QIconSet::On;

            QPixmap pixmap = button->iconSet()->pixmap( QIconSet::Small, mode, state );

            if (button->text().isEmpty() && !button->pixmap())
                p->drawPixmap( x + w/2 - pixmap.width()/2, y + h / 2 - pixmap.height() / 2,
                               pixmap );
            else
                p->drawPixmap( x + 4, y + h / 2 - pixmap.height() / 2, pixmap );

            if (cornArrow) //Draw over the icon
                drawPrimitive( PE_ArrowDown, p, visualRect( QRect(x + w - 6, x + h - 6, 7, 7), r ),
                               cg, flags, opt );


            int  pw = pixmap.width();
            x += pw + 4;
            w -= pw + 4;
        }

        // Make the label indicate if the button is a default button or not
        drawItem( p, QRect(x, y, w, h), AlignCenter|ShowPrefix, button->colorGroup(),
                  button->isEnabled(), button->pixmap(), button->text(), -1,
                  &button->colorGroup().buttonText() );
        break;
    }
    case CE_MenuBarItem: {
        QMenuItem *mi = opt.menuItem();
        bool active  = flags & Style_Active;
        bool focused = flags & Style_HasFocus;
        bool down = flags & Style_Down;
        const int text_flags =
            AlignVCenter | AlignHCenter | ShowPrefix | DontClip | SingleLine;

        p->fillRect(r, cg.background());

        if (active && focused) {
            if (!down) {
                p->setBrush(*c1);
                p->setPen(cg.highlight());
                p->drawRect(r);
            } else {
                int x, x2, y, y2;
                r.coords(&x, &y, &x2, &y2);
                p->setBrush(*c2);
                p->setPen(c2->dark());
                p->drawRect(r);
                p->setPen(c2->light());
                p->drawLine(x+1, y2, x2-1, y2);
            }
        }

        if (useTextShadows) {
            p->setPen((active && focused && !down ? cg.foreground().dark(135) : cg.background().dark(115)));
            p->drawText(r.x() + 1, r.y() + 1, r.width(), r.height(), text_flags, mi->text());
        }
        p->setPen( cg.foreground());
        p->drawText(r, text_flags, mi->text());
        break;
    }

    // POPUPMENU ITEM (highlighted on mouseover)
    // ------------------------------------------
    case CE_PopupMenuItem: {
        QMenuItem *mi = opt.menuItem();

        if (!mi) {
            return;
        }

        const QPopupMenu *pum = (const QPopupMenu *) widget;
        static const int itemFrame = 2;
        static const int itemHMargin = 3;
        static const int itemVMargin = 3;
        static const int arrowHMargin = 6;
        static const int rightBorder = 20;
        const int tab = opt.tabWidth();

        int checkcol = QMAX(opt.maxIconWidth(), 26);
        int x, x2, y, y2, w, h;
        r.rect(&x, &y, &w, &h);
        r.coords(&x, &y, &x2, &y2);

        bool active = flags & Style_Active;
        bool disabled = !mi->isEnabled();
        bool checkable = pum->isCheckable();

        int xpos = x;
        int xm = itemFrame + checkcol + itemHMargin;

        if (pum->erasePixmap() && !pum->erasePixmap()->isNull()) {
            p->drawPixmap( x, y, *pum->erasePixmap(), x, y, w, h );
        } else {
            if (!reverseLayout) {
                p->fillRect( x, y, 24, h, *c2 );
                p->fillRect( x + 24, y, w - 24, h, *c3 );
            } else { // i wonder why exactly +1 (diego)
                p->fillRect( w-24, y, 24+1, h, *c3 );
                p->fillRect( x, y, w - 24, h, *c2 );
            }
        }

        if (mi->isSeparator()) {
            p->setPen(c2->dark(180));
            if (!reverseLayout)
                p->drawLine(x + opt.maxIconWidth() + 6, y, x + w, y);
            else
                p->drawLine(w - (x + opt.maxIconWidth() + 6), y, x, y);
            return;
        }

        if (active && !disabled) {
            p->setBrush(*c1);
            p->fillRect(x + 2, y + 2, w - 4, h - 4, *c1);
            p->setPen(cg.highlight());
            const QCOORD corners[] = {
                x+1, y2-1, x+1, y+1, x2-1, y+1, x2-1, y2-1
            };
            p->drawConvexPolygon(QPointArray(4, corners));

            p->setPen(cg.foreground());
        }

        if (mi->iconSet()) {
            p->save();
            QIconSet::Mode mode = disabled ? QIconSet::Disabled : QIconSet::Normal;
            QPixmap pixmap = mi->iconSet()->pixmap(QIconSet::Small, mode);
            int pixw = pixmap.width();
            int pixh = pixmap.height();

            QRect cr(xpos, y, opt.maxIconWidth(), h);
            QRect pmr(0, 0, pixw, pixh);
            pmr.moveCenter(cr.center());

            if (reverseLayout)
                pmr = visualRect( pmr, r );

            p->setPen(cg.foreground());
            if (active && !disabled)
                p->drawPixmap(QPoint(pmr.topLeft().x()+1,pmr.topLeft().y()), pixmap);
            else
                p->drawPixmap(QPoint(pmr.topLeft().x()+2,pmr.topLeft().y()+1), pixmap);
            p->restore();
        }

        p->setPen(active ? cg.foreground() : cg.buttonText());

        xpos += xm;

        if (mi->custom()) {
            int m = itemVMargin;
            p->setPen(cg.foreground());
            mi->custom()->paint(p, cg, active, !disabled, x+xm, y+m, w-xm-tab+1, h-2*m);
            return;
        } else {
            QString s = mi->text();
            if (!s.isNull()) {
                int t = s.find('\t');
                int m = itemVMargin;

                int text_flags = AlignVCenter | ShowPrefix | DontClip | SingleLine;

                if (active && !disabled) {
                    p->setPen(cg.foreground());
                } else if (disabled) {
                    p->setPen(c2->dark(190));
                } else {
                    p->setPen(cg.text());
                }

                if (t >= 0) {
                    int xp;
                    xp = x + w - tab - rightBorder - itemHMargin - itemFrame + 1;

                    QRect rr = QRect(xp, y+m, tab, h-(2*m));
                    if (reverseLayout)
                        rr = visualRect(rr, r);

                    if (useTextShadows) {
                        QPen op = p->pen();
                        p->setPen(active && !disabled ? cg.foreground().dark(130) : cg.background().dark(115));
                        p->drawText(rr.x() + 1, rr.y() + 1, rr.width(), rr.height(), text_flags, s.mid(t+1));
                        p->setPen(op);
                    }

                    p->drawText(rr, text_flags, s.mid(t+1));
                    s = s.left(t);
                }

                QRect rr = QRect(xpos, y+m, w-xm-tab+1, h-(2*m));
                if (reverseLayout) {
                    rr = visualRect(rr, r);
                    text_flags |= AlignRight;
                }

                if (useTextShadows) {
                    QPen op = p->pen();
                    p->setPen(active && !disabled ? cg.foreground().dark(130) : cg.background().dark(115));
                    p->drawText(rr.x() + 1, rr.y() + 1, rr.width(), rr.height(), text_flags, s);
                    p->setPen(op);
                }

                p->drawText(rr, text_flags, s);
            } else if (mi->pixmap()) {
                QPixmap *pixmap = mi->pixmap();
                if (pixmap->depth() == 1) {
                    p->setBackgroundMode(OpaqueMode);
                }
                p->drawPixmap(xpos, y, *pixmap);
                if (pixmap->depth() == 1) {
                    p->setBackgroundMode(TransparentMode);
                }
            }
        }

        if (mi->popup()) {
            int dim = pixelMetric(PM_MenuButtonIndicator);

            xpos = x+w - arrowHMargin - 2*itemFrame - dim;

            if (active && !disabled)
                p->setPen(cg.foreground());
            else
                p->setPen(cg.text());


            QRect rr = QRect(xpos, y + h/2 - dim/2, dim, dim);
            if (reverseLayout)
                rr = visualRect( rr, r );
            drawPrimitive((reverseLayout ? PE_ArrowLeft : PE_ArrowRight), p, rr, cg, Style_Enabled);
        }

        if (checkable) {
            if (mi->isChecked()) {
                SFlags cflags = Style_Default;

                if (!disabled)
                    cflags |= Style_Enabled;
                else
                    cflags |= Style_On;

                QRect rr = QRect( x + 2, y + 2, 20, h-4 );

                p->fillRect(rr, cg.background());

                p->save();
                p->setPen(cg.highlight());
                p->setBrush(*csel);
                p->drawRect(rr);
                p->restore();

                drawPrimitive(PE_CheckMark, p, rr, cg, cflags);
            }
        }

        break;
    }

    // Menu and dockwindow empty space
    //
    case CE_DockWindowEmptyArea:
    case CE_MenuBarEmptyArea:
        p->fillRect(r, cg.background());
        break;
    default:
        KStyle::drawControl(element, p, widget, r, cg, flags, opt);
    }
}

void dotNET2style::drawControlMask(ControlElement element,
                                   QPainter *p,
                                   const QWidget *w,
                                   const QRect &r,
                                   const QStyleOption &opt) const
{
    switch (element) {
    case CE_PushButton: {
        // Just fill the entire thing
        p->fillRect (r, color1);
        break;
    }

    default: {
        KStyle::drawControlMask (element, p, w, r, opt);
    }
    }
}

void dotNET2style::drawComplexControlMask(ComplexControl c,
        QPainter *p,
        const QWidget *w,
        const QRect &r,
        const QStyleOption &o) const
{
    switch (c) {
    case CC_ComboBox: {
        // Just fill the entire thing
        p->fillRect (r, color1);
        break;
    }
    default: {
        KStyle::drawComplexControlMask (c, p, w, r, o);
    }
    }
}

void dotNET2style::drawComplexControl(ComplexControl control,
                                      QPainter *p,
                                      const QWidget *widget,
                                      const QRect &r,
                                      const QColorGroup &cg,
                                      SFlags flags,
                                      SCFlags controls,
                                      SCFlags active,
                                      const QStyleOption& opt) const
{
    if ( widget == hoverWidget) {
        flags |= Style_MouseOver;
    }

    switch (control) {
        // COMBOBOX
        // --------
    case CC_ComboBox: {
        //QColor c1 = QColor(MYSELRED(cg)+cg.highlight().red(),MYSELGREEN(cg)+cg.highlight().green(),MYSELBLUE(cg)+cg.highlight().blue());
        int x, y, w, h;
        const QComboBox *cb = (const QComboBox *)widget;
        r.rect(&x, &y, &w, &h);

        if (active & Style_Sunken)
            flags |= Style_Sunken;

        static const unsigned char downarrow_bits[] = {
            0x7f, 0xbe, 0x9c, 0x08, 0x00, 0x00, 0x00, 0x28,
            0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xb8
        };

        static const unsigned int handle_width = 15;
        static const unsigned int handle_offset = handle_width + 1;

        QBitmap downArrow = QBitmap(7, 4, downarrow_bits, true);
        downArrow.setMask(downArrow);

        if ( flags & Style_MouseOver || flags & Style_HasFocus) {
            QRect rec = QRect(x + 1, y + 1, w - 1, h - 1);
            p->save();
            p->setPen(cg.highlight());
            p->setBrush(cg.base());
            p->drawRect(rec);
            p->restore();
        } else {
            QRect rec = QRect(x + 1, y + 1, w - 1, h - 1);
            p->save();
            p->setPen(cg.base());
            p->setBrush(cg.base());
            p->drawRect(rec);
            p->restore();
        }


        p->save();
        // Draw the box on the right.
        if (cb->listBox() && cb->listBox()->isVisible() || flags & Style_HasFocus || flags & Style_MouseOver) {
            p->setPen(cg.highlight());
            p->setBrush(*c1);
        } else {
            p->setPen(cg.base());
            p->setBrush(cg.background());
        }

        QRect hr = QRect(w - handle_offset - 1, y + 1, handle_width + 2, h - 1);
        if (reverseLayout) {
            hr = visualRect(hr, r);
        }

        p->drawRect(hr);
        p->setBrush(NoBrush);

        p->setPen(cg.foreground());

        QRect cr(w - handle_offset, y, handle_width, h - 2);
        QRect pmr(0, 0, 7, 4);
        pmr.moveCenter(cr.center());
        if (reverseLayout) {
            pmr = visualRect( pmr, r );
        }

        p->drawPixmap(pmr.topLeft(), downArrow);

        p->restore();
        break;
    }

    // TOOLBUTTON
    // ----------
    case CC_ToolButton: {
        const QToolButton *tb = (const QToolButton *) widget;

        QRect button, menuarea;
        button   = querySubControlMetrics(control, widget, SC_ToolButton, opt);
        menuarea = querySubControlMetrics(control, widget, SC_ToolButtonMenu, opt);

        SFlags bflags = flags, mflags = flags;

        if (active & SC_ToolButton)
            bflags |= Style_Down;

        if (active & SC_ToolButtonMenu)
            mflags |= Style_Down;

        if (controls & SC_ToolButton) {
            // If we're pressed, on, or raised...
            if (bflags & (Style_Down | Style_On | Style_Raised)) {
                drawPrimitive(PE_ButtonTool, p, button, cg, bflags, opt);
            } else if (tb->parentWidget() &&
                       tb->parentWidget()->backgroundPixmap() &&
                       !tb->parentWidget()->backgroundPixmap()->isNull()) {
                QPixmap pixmap = *(tb->parentWidget()->backgroundPixmap());
                p->drawTiledPixmap( r, pixmap, tb->pos() );
            }
        }

        // Draw a toolbutton menu indicator if required
        if (controls & SC_ToolButtonMenu) {
            if (mflags & (Style_Down | Style_On | Style_Raised)) {
                drawPrimitive(PE_ButtonDropDown, p, menuarea, cg, mflags, opt);
            }
            drawPrimitive(PE_ArrowDown, p, menuarea, cg, mflags, opt);
        }

        if (tb->hasFocus() && !tb->focusProxy()) {
            QRect fr = tb->rect();
            fr.addCoords(2, 2, -2, -2);
            drawPrimitive(PE_FocusRect, p, fr, cg);
        }

        break;
    }

    // SPINWIDGETS
    // -----------
    case CC_SpinWidget: {
        const QSpinWidget *sw = (const QSpinWidget *) widget;
        SFlags flags;
        PrimitiveElement pe;

        QRect swf = querySubControlMetrics(control, sw, SC_SpinWidgetFrame);

        if (controls & SC_SpinWidgetFrame) {
            // Don't draw the WindowsStyle thick frame.
            renderPanel(p, swf, cg);
        }

        if (controls & SC_SpinWidgetUp) {
            flags = Style_Default | Style_Enabled;
            if (active == SC_SpinWidgetUp) {
                flags |= Style_On;
                flags |= Style_Sunken;
            } else
                flags |= Style_Raised;

            if (sw->buttonSymbols() == QSpinWidget::PlusMinus)
                pe = PE_SpinWidgetPlus;
            else
                pe = PE_SpinWidgetUp;

            QRect re = sw->upRect();
            QColorGroup ucg = sw->isUpEnabled() ? cg : sw->palette().disabled();
            p->fillRect(re.x() + 1, re.y() + 1, re.width() - 2, re.height() - 1, flags & Style_Raised ? ucg.background() : ucg.highlight());

            re.setX(re.x() + 1);
            p->setPen(flags & Style_Raised ? ucg.foreground() : ucg.highlightedText());
            drawPrimitive(pe, p, re, ucg, flags);
        }

        if (controls & SC_SpinWidgetDown) {
            flags = Style_Default | Style_Enabled;
            if (active == SC_SpinWidgetDown) {
                flags |= Style_On;
                flags |= Style_Sunken;
            } else
                flags |= Style_Raised;

            if (sw->buttonSymbols() == QSpinWidget::PlusMinus)
                pe = PE_SpinWidgetMinus;
            else
                pe = PE_SpinWidgetDown;

            QRect re = sw->downRect();
            QColorGroup dcg = sw->isDownEnabled() ? cg : sw->palette().disabled();
            p->fillRect(re.x() + 1, re.y(), re.width() - 2, re.height() - 1, flags & Style_Raised ? dcg.background() : dcg.highlight());

            p->setPen(flags & Style_Raised ? dcg.foreground() : dcg.highlightedText());
            re.setX(re.x() + 1);
            drawPrimitive(pe, p, re, dcg, flags);
        }

        break;
    }
    default:
        KStyle::drawComplexControl(control, p, widget,
                                   r, cg, flags, controls,
                                   active, opt);
        break;
    }
}


QRect dotNET2style::subRect(SubRect r, const QWidget *widget) const
{
    // Don't use KStyles progressbar subrect
    switch (r) {
    case SR_ComboBoxFocusRect: {
        return querySubControlMetrics( CC_ComboBox, widget, SC_ComboBoxEditField );
    }

    case SR_ProgressBarGroove:
    case SR_ProgressBarContents:
    case SR_ProgressBarLabel: {
        return KStyle::subRect(r, widget);
    }
    default: {
        return KStyle::subRect(r, widget);
    }
    }
}

QRect dotNET2style::querySubControlMetrics(ComplexControl control,
        const QWidget *widget,
        SubControl subcontrol,
        const QStyleOption &opt) const
{
    if (!widget) {
        return QRect();
    }

    QRect r(widget->rect());
    switch (control) {
    case CC_ComboBox: {
        switch (subcontrol) {
        case SC_ComboBoxEditField: {
            return QRect(r.x() + 2, r.y() + 2, r.width() - 19, r.height() - 4);
        }
        default: {
            return KStyle::querySubControlMetrics(control, widget, subcontrol, opt);
        }
        }
        break;
    }

    case CC_SpinWidget: {
        int fw = pixelMetric(PM_SpinBoxFrameWidth, widget);
        QSize bs;
        bs.setHeight(QMAX(8, widget->height()/2));
        bs.setWidth(QMIN(bs.height() * 8 / 5, widget->width() / 4));

        int y = fw;
        int x, lx, rx;
        x = widget->width() - y - bs.width();
        lx = fw;
        rx = x - fw;

        switch (subcontrol) {
        case SC_SpinWidgetUp: {
            return QRect(x, y-1, bs.width(), bs.height());
        }
        case SC_SpinWidgetDown: {
            return QRect(x, y + bs.height()-1, bs.width(), bs.height());
        }
        case SC_SpinWidgetFrame: {
            return QRect(0, 0, widget->width() - (bs.width() + 2), widget->height());
        }
        case SC_SpinWidgetEditField: {
            return QRect(lx, fw, widget->width() - (bs.width() + 4), widget->height() - 2 * fw);
        }
        case SC_SpinWidgetButtonField: {
            return QRect(x, y, bs.width(), widget->height() - 2 * fw);
        }
        default:
            ;
        }
        break;
    }
    default: {}
    }
    return KStyle::querySubControlMetrics(control, widget, subcontrol, opt);
}

int dotNET2style::pixelMetric(PixelMetric m, const QWidget *widget) const
{
    switch (m) {
        // TABS
        // ----
    case PM_TabBarTabVSpace: {
        const QTabBar * tb = (const QTabBar *) widget;
        if (tb->shape() == QTabBar::RoundedAbove || tb->shape() == QTabBar::RoundedBelow)
            return 12;
        else
            return 0;
    }

    case PM_TabBarTabOverlap: {
        const QTabBar* tb = (const QTabBar*)widget;
        if (tb->shape() != QTabBar::RoundedAbove && tb->shape() != QTabBar::RoundedBelow)
            return 3;                   // Leave standard size alone
        else
            return 1;                   // Change size for our tabs only
    }

    // SPLITTERS
    // ---------
    case PM_SplitterWidth: {
        return 6;
    }

    // PROGRESSBARS
    // ------------
    case PM_ProgressBarChunkWidth:
        return 9;

        // SLIDER
        // ------
    case PM_SliderLength:
        return 11;

        // MENU INDICATOR
        // --------------
    case PM_MenuButtonIndicator:
        return 8;

        // HEADER INDICATOR
        // ----------------
    case PM_HeaderMarkSize:
        return 1;

        // CHECKBOXES / RADIO BUTTONS
        // --------------------------
    case PM_ExclusiveIndicatorWidth:    // Radiobutton size
    case PM_ExclusiveIndicatorHeight:   // 13x13
    case PM_IndicatorWidth:             // Checkbox size
    case PM_IndicatorHeight:            // 13x13
        return 13;

        // FRAMES
        // ------
    case PM_SpinBoxFrameWidth:
        return 1;

    case PM_MenuBarFrameWidth:
        return 0;

    case PM_DefaultFrameWidth:
        return 1;

    case PM_ButtonDefaultIndicator:
        return 3;

    case PM_ButtonShiftVertical:
    case PM_ButtonShiftHorizontal:
        return 1;

    default:
        return KStyle::pixelMetric(m, widget);
    }
}


QSize dotNET2style::sizeFromContents(ContentsType t,
                                     const QWidget *widget,
                                     const QSize &s,
                                     const QStyleOption &opt) const
{
    switch (t) {

    case CT_PopupMenuItem: {
        if (!widget || opt.isDefault())
            break;

        const QPopupMenu *popup = (const QPopupMenu *)widget;
        QMenuItem *mi = opt.menuItem();
        int maxpmw = opt.maxIconWidth();
        int w = s.width(), h = s.height();
        bool checkable = popup->isCheckable();

        if (mi->custom()) {
            w = mi->custom()->sizeHint().width();
            h = mi->custom()->sizeHint().height();
        } else if (mi->widget()) {
            // don't change the size in this case.
        } else if (mi->isSeparator()) {
            w = 10;
            h = 1;
        } else {
            if (mi->pixmap()) {
                h = QMAX(h, mi->pixmap()->height() + 8);
            }

            h = QMAX(h, widget->fontMetrics().height() + 10);

            if (mi->iconSet()) {
                h = QMAX(h, mi->iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).height() + 8);
            }
        }

        if (!mi->text().isNull()) {
            if ((mi->text().find('\t') >= 0) || mi->popup()) {
                w += 20;
            }
        }

        if (maxpmw) {
            w += maxpmw;
        }
        if (checkable && maxpmw < 20) {
            w += 20 - maxpmw;
        }
        if (checkable || maxpmw > 0) {
            w += 2;
        }

        w += 20;

        return QSize(w, h);
    }

    case CT_PushButton: {
        const QPushButton *button = (const QPushButton *)widget;
        QSize ret = QCommonStyle::sizeFromContents(t, widget, s, opt);
        int w = ret.width() + 6, h = ret.height() - 2, tp = 0;
        ;
        int dbi = (pixelMetric(PM_ButtonDefaultIndicator, widget) * 2) + 6;
        int mw = 80 - dbi, mh = 30 - dbi;
        bool btnDefault = button->isDefault() || button->autoDefault();
        bool hasText = (button->text() && !button->text().isNull());

        if (btnDefault) {
            mw += dbi;
            mh += dbi;
        }
        if (button->iconSet() && !button->iconSet()->isNull()) {
            w += button->iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).width();
            if (!hasText) {
                w = button->iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).width();
                h = button->iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).height();
            }
        } else if (button->pixmap() && !button->pixmap()->isNull()) {
            w += button->pixmap()->width();
            if (!hasText) {
                w = button->pixmap()->width();
                h = button->pixmap()->height();
            }
        }

        if (!hasText) {
            return QSize(w + 6, h + 6);
        }
        if (!inheritsKHTML(widget)) {
            QFontMetrics fm(QFont(button->font().family(), button->font().pointSize(), 75));
            fm.boundingRect(button->text()).rect(&tp, &tp, &w, &h);

            w = (w < mw ? mw : w + dbi);
            h = (h < mh ? mh : h + dbi);
        }

        return QSize(w, h);
    }

    case CT_ComboBox: {
        int arrow = 21;
        return QSize( s.width() + arrow, QMAX(s.height() + 4, 16));
    }
    default:
        return KStyle::sizeFromContents (t, widget, s, opt);
    }

    return KStyle::sizeFromContents (t, widget, s, opt);
}

void dotNET2style::slotDestroyed()
{
    m_widgets.remove( (QWidget*) sender() );
}

bool dotNET2style::eventFilter(QObject *obj , QEvent *ev)
{
    if (obj->inherits("QPushButton") || obj->inherits("QComboBox") ||
            obj->inherits("QSpinWidget") ||  obj->inherits("QSlider") || obj->inherits("QScrollBar") ||
            obj->inherits("QCheckBox") || obj->inherits("QRadioButton") || obj->inherits("QToolButton")) {
        if ( obj->inherits("QPushButton")) {
            QPushButton *mpb = (QPushButton *) obj;
            int x,y,w,h;
            (mpb->rect()).rect(&x,&y,&w,&h);
        }
    } else if (obj->inherits("QButton")) {
        QWidget *btn = (QWidget *)obj;
        QPalette pal = btn->palette();
        pal.setColor(QColorGroup::Button, pal.active().color(QColorGroup::Background));
        btn->setPalette(pal);
    }

    if (ev->type() == QEvent::Enter) {
        hoverWidget = (QWidget *)obj;
        if (hoverWidget->isEnabled()) {
            hoverWidget->repaint(false);
        }
    } else if (ev->type() == QEvent::Leave) {
        QWidget *btn = (QWidget *)obj;

        if ( hoverWidget == obj ) {
            hoverWidget = NULL;
        }

        btn->repaint(false);
    }

    return false;
}

// vim: noet ts=4 sw=4
