/* Plastik widget style for KDE 3
   Copyright (C) 2003 Sandro Giessl <ceebx@users.sourceforge.net>

   based on the KDE style "dotNET":
   Copyright (C) 2001-2002, Chris Lee <clee@kde.org>
                            Carsten Pfeiffer <pfeiffer@kde.org>
                            Karol Szwed <gallium@kde.org>
   Drawing routines completely reimplemented from KDE3 HighColor, which was
   originally based on some stuff from the KDE2 HighColor.

   based on drawing routines of the style "Keramik":
   Copyright (c) 2002 Malte Starostik <malte@kde.org>
             (c) 2002,2003 Maksim Orlovich <mo002j@mail.rochester.edu>
   based on the KDE3 HighColor Style
   Copyright (C) 2001-2002 Karol Szwed      <gallium@kde.org>
             (C) 2001-2002 Fredrik H�glund  <fredrik@kde.org>
   Drawing routines adapted from the KDE2 HCStyle,
   Copyright (C) 2000 Daniel M. Duley       <mosfet@kde.org>
             (C) 2000 Dirk Mueller          <mueller@kde.org>
             (C) 2001 Martijn Klingens      <klingens@kde.org>
   Progressbar code based on KStyle,
   Copyright (C) 2001-2002 Karol Szwed <gallium@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

#ifndef __PLASTIK_H
#define __PLASTIK_H

#include <kstyle.h>
#include <qbitmap.h>
#include <qintcache.h>
#include <qmainwindow.h>

#define u_arrow -4,1, 2,1, -3,0, 1,0, -2,-1, 0,-1, -1,-2
#define d_arrow -4,-2, 2,-2, -3,-1, 1,-1, -2,0, 0,0, -1,1
#define l_arrow 0,-3, 0,3,-1,-2,-1,2,-2,-1,-2,1,-3,0
#define r_arrow -2,-3,-2,3,-1,-2, -1,2,0,-1,0,1,1,0

#define QCOORDARRLEN(x) sizeof(x)/(sizeof(QCOORD)*2)

class QSettings;
class QTab;
class QTimer;

class PlastikStyle : public KStyle
{
    Q_OBJECT

public:
    PlastikStyle();
    virtual ~PlastikStyle();

    void polish(QApplication* app );
    void polish(QWidget* widget );
    void unPolish(QWidget* widget );

    void drawKStylePrimitive(KStylePrimitive kpe,
                             QPainter* p,
                             const QWidget* widget,
                             const QRect &r,
                             const QColorGroup &cg,
                             SFlags flags = Style_Default,
                             const QStyleOption& = QStyleOption::Default ) const;

    void drawPrimitive(PrimitiveElement pe,
                       QPainter *p,
                       const QRect &r,
                       const QColorGroup &cg,
                       SFlags flags = Style_Default,
                       const QStyleOption &opt = QStyleOption::Default ) const;

    void drawControl(ControlElement element,
                     QPainter *p,
                     const QWidget *widget,
                     const QRect &r,
                     const QColorGroup &cg,
                     SFlags flags = Style_Default,
                     const QStyleOption& = QStyleOption::Default ) const;

    void drawControlMask( ControlElement, QPainter *, const QWidget *, const QRect &, const QStyleOption &) const;

    void drawComplexControl(ComplexControl control,
                            QPainter *p,
                            const QWidget *widget,
                            const QRect &r,
                            const QColorGroup &cg,
                            SFlags flags = Style_Default,
                            SCFlags controls = SC_All,
                            SCFlags active = SC_None,
                            const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric(PixelMetric m,
                    const QWidget *widget = 0 ) const;

    QRect subRect(SubRect r,
                  const QWidget *widget ) const;

    QRect querySubControlMetrics(ComplexControl control,
                                 const QWidget *widget,
                                 SubControl subcontrol,
                                 const QStyleOption &opt = QStyleOption::Default ) const;

    void drawComplexControlMask(QStyle::ComplexControl c,
                                QPainter *p,
                                const QWidget *w,
                                const QRect &r,
                                const QStyleOption &o=QStyleOption::Default) const;

    QSize sizeFromContents(QStyle::ContentsType t,
                           const QWidget *w,
                           const QSize &s,
                           const QStyleOption &o) const;

    int styleHint(StyleHint, const QWidget * = 0,
                  const QStyleOption & = QStyleOption::Default,
                  QStyleHintReturn * = 0 ) const;

protected:
    enum TabPosition {
        First = 0,
        Middle,
        Last,
        Single // only one tab!
    };

    enum ColorType {
        ButtonContour,
        DragButtonContour,
        DragButtonSurface,
        PanelContour,
        PanelLight,
        PanelLight2,
        PanelDark,
        PanelDark2,
        MouseOverHighlight,
        FocusHighlight,
        CheckMark
    };

    enum WidgetState {
        IsEnabled,
        IsPressed,     // implies IsEnabled
        IsHighlighted, // implies IsEnabled
        IsDisabled
    };

    // the only way i see to provide all these options
    // to renderContour/renderSurface...
    enum SurfaceFlags {
        Draw_Left =          0x00000001,
        Draw_Right =         0x00000002,
        Draw_Top =           0x00000004,
        Draw_Bottom =        0x00000008,
        Highlight_Left =     0x00000010, // surface
        Highlight_Right =    0x00000020, // surface
        Highlight_Top =      0x00000040, // surface
        Highlight_Bottom =   0x00000080, // surface
        Is_Sunken =          0x00000100, // surface
        Is_Horizontal =      0x00000200, // surface
        Is_Highlight =       0x00000400, // surface
        Is_Default =         0x00000800, // surface
        Is_Disabled =        0x00001000,
        Round_UpperLeft =    0x00002000,
        Round_UpperRight =   0x00004000,
        Round_BottomLeft =   0x00008000,
        Round_BottomRight =  0x00010000,
        Draw_AlphaBlend =    0x00020000
    };

    void renderContour(QPainter *p,
                       const QRect &r,
                       const QColor &backgroundColor,
                       const QColor &contourColor,
                       const uint flags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                                          Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight) const;

    void renderMask(QPainter *p,
                    const QRect &r,
                    const QColor &color,
                    const uint flags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                                       Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight) const;

    void renderSurface(QPainter *p,
                       const QRect &r,
                       const QColor &backgroundColor,
                       const QColor &buttonColor,
                       const QColor &highlightColor,
                       int intensity = 5,
                       const uint flags = Draw_Left|Draw_Right|Draw_Top|Draw_Bottom|
                                          Round_UpperLeft|Round_UpperRight|Round_BottomLeft|Round_BottomRight|
                                          Is_Horizontal) const;

    inline void renderPixel(QPainter *p,
                            const QPoint &pos,
                            const int alpha,
                            const QColor &color,
                            const QColor &background = QColor(),
                            bool fullAlphaBlend = true) const;

    void renderButton(QPainter *p,
                      const QRect &r,
                      const QColorGroup &g,
                      bool sunken = false,
                      bool mouseOver = false,
                      bool horizontal = true,
                      bool enabled = true,
                      bool khtmlMode = false) const;

    void renderHeader(QPainter *p,
                      const QRect &r,
                      const QColorGroup &g,
                      bool sunken = false,
                      bool mouseOver = false,
                      bool horizontal = true,
                      bool enabled = true) const;

    void renderPanel(QPainter *p,
                     const QRect &r,
                     const QColorGroup &g,
                     const bool pseudo3d = true,
                     const bool sunken = true) const;

    void renderDot(QPainter *p,
                   const QPoint &point,
                   const QColor &baseColor,
                   const bool thick = false,
                   const bool sunken = false) const;

    void renderGradient(QPainter *p,
                        const QRect &r,
                        const QColor &c1,
                        const QColor &c2,
                        bool horizontal = true) const;

    void renderTab(QPainter *p,
                   const QRect &r,
                   const QColorGroup &g,
                   bool mouseOver = false,
                   const bool selected = false,
                   const bool bottom = false,
                   const TabPosition pos = Middle,
                   const bool triangular = false,
                   const bool cornerWidget = false) const;

    virtual void renderMenuBlendPixmap( KPixmap& pix, const QColorGroup& cg,
                                        const QPopupMenu* popup ) const;

    bool eventFilter(QObject *, QEvent *);

    QWidget* hoverWidget;
protected slots:
    void khtmlWidgetDestroyed(QObject* w);

    //Animation slots.
    void updateProgressPos();
    void progressBarDestroyed(QObject* bar);

    inline QColor getColor(const QColorGroup &cg, const ColorType t, const bool enabled = true)const;
    inline QColor getColor(const QColorGroup &cg, const ColorType t, const WidgetState s)const;
private:
// Disable copy constructor and = operator
    PlastikStyle( const PlastikStyle & );
    PlastikStyle& operator=( const PlastikStyle & );

    bool kickerMode, kornMode;
    mutable bool flatMode;

    int _contrast;
    bool _scrollBarLines;
    bool _animateProgressBar;
    bool _drawToolBarSeparator;
    bool _drawToolBarItemSeparator;
    bool _drawFocusRect;
    bool _drawTriangularExpander;
    bool _inputFocusHighlight;
    bool _customOverHighlightColor;
    bool _customFocusHighlightColor;
    bool _customCheckMarkColor;
    QColor _overHighlightColor;
    QColor _focusHighlightColor;
    QColor _checkMarkColor;

    QTab *hoverTab;

    // track khtml widgets.
    QMap<const QWidget*,bool> khtmlWidgets;

    //Animation support.
    QMap<QWidget*, int> progAnimWidgets;

    // pixmap cache.
    enum CacheEntryType {
        cSurface,
        cGradientTile,
        cAlphaDot
    };
    struct CacheEntry {
        CacheEntryType type;
        int width;
        int height;
        QRgb c1Rgb;
        QRgb c2Rgb;
        bool horizontal;

        QPixmap* pixmap;

        CacheEntry(CacheEntryType t, int w, int h, QRgb c1, QRgb c2 = 0,
                   bool hor = false, QPixmap* p = 0 ):
                type(t), width(w), height(h), c1Rgb(c1), c2Rgb(c2), horizontal(hor), pixmap(p) {}

        ~CacheEntry() {
            delete pixmap;
        }

        int key() {
            // create an int key from the properties which is used to refer to entries in the QIntCache.
            // the result may not be 100% correct as we don't have so much space in one integer -- use
            // == operator after find to make sure we got the right one. :)
            return horizontal ^ (type<<1) ^ (width<<5) ^ (height<<10) ^ (c1Rgb<<19) ^ (c2Rgb<<22);
        }

        bool operator == (const CacheEntry& other) {
            bool match = (type == other.type) &&
                         (width   == other.width) &&
                         (height == other.height) &&
                         (c1Rgb == other.c1Rgb) &&
                         (c1Rgb == other.c1Rgb) &&
                         (horizontal = other.horizontal);
//             if(!match) {
//                 qDebug("operator ==: CacheEntries don't match!");
//                 qDebug("width: %d\t\tother width: %d", width, other.width);
//                 qDebug("height: %d\t\tother height: %d", height, other.height);
//                 qDebug("fgRgb: %d\t\tother fgRgb: %d", fgRgb, other.fgRgb);
//                 qDebug("bgRgb: %d\t\tother bgRgb: %d", bgRgb, other.bgRgb);
//                 qDebug("surfaceFlags: %d\t\tother surfaceFlags: %d", surfaceFlags, other.surfaceFlags);
//             }
            return match;
        }
    };
    QIntCache<CacheEntry> *pixmapCache;

    // For renderFocusRect
    mutable QBitmap *verticalDots;
    mutable QBitmap *horizontalDots;

    // For KPE_ListViewBranch
    mutable QBitmap *verticalLine;
    mutable QBitmap *horizontalLine;

    // For progress bar animation
    QTimer *animationTimer;
};

#endif // __PLASTIK_H
