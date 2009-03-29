/*
 * $Id: dotnet.h,v 1.23 2004/01/04 14:44:41 mlaurent Exp $
 *
 * Copyright 2001, Chris Lee <lee@azsites.com>
 * Originally copied from the KDE3 HighColor style, modified to fit mine.
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

#ifndef __DOTNET_H
#define __DOTNET_H

#include <kstyle.h>
#include <qbitmap.h>

#define u_arrow -4,1, 2,1, -3,0, 1,0, -2,-1, 0,-1, -1,-2
#define d_arrow -4,-2, 2,-2, -3,-1, 1,-1, -2,0, 0,0, -1,1
#define l_arrow 0,-3, 0,3,-1,-2,-1,2,-2,-1,-2,1,-3,0
#define r_arrow -2,-3,-2,3,-1,-2, -1,2,0,-1,0,1,1,0

#define QCOORDARRLEN(x) sizeof(x)/(sizeof(QCOORD)*2)

#define MYRED(cg) ((int) (cg.base().red() - cg.background().red())/6)
#define MYGREEN(cg) ((int) (cg.base().green() - cg.background().green())/6)
#define MYBLUE(cg) ((int) (cg.base().blue() - cg.background().blue())/6)

#define MYSELRED(cg) ((int) ((cg.base().red() - cg.highlight().red())*0.7))
#define MYSELGREEN(cg) ((int) ((cg.base().green() - cg.highlight().green())*0.7))
#define MYSELBLUE(cg) ((int) ((cg.base().blue() - cg.highlight().blue())*0.7))


class dotNET2style : public KStyle
{
    Q_OBJECT

public:
    dotNET2style();
    virtual ~dotNET2style();

    bool inheritsKHTML( const QWidget* widget ) const;
    void polish( QWidget* widget );
    void unPolish( QWidget* widget );

    void renderMenuBlendPixmap( KPixmap&, const QColorGroup&, const QPopupMenu * ) const;

    void drawKStylePrimitive( KStylePrimitive kpe,
                              QPainter* p,
                              const QWidget* widget,
                              const QRect &r,
                              const QColorGroup &cg,
                              SFlags flags = Style_Default,
                              const QStyleOption& = QStyleOption::Default ) const;

    void drawPrimitive( PrimitiveElement pe,
                        QPainter *p,
                        const QRect &r,
                        const QColorGroup &cg,
                        SFlags flags = Style_Default,
                        const QStyleOption &opt = QStyleOption::Default ) const;

    void drawControl( ControlElement element,
                      QPainter *p,
                      const QWidget *widget,
                      const QRect &r,
                      const QColorGroup &cg,
                      SFlags flags = Style_Default,
                      const QStyleOption& = QStyleOption::Default ) const;

    void drawControlMask( ControlElement, QPainter *, const QWidget *, const QRect &, const QStyleOption &) const;

    void drawComplexControl( ComplexControl control,
                             QPainter *p,
                             const QWidget *widget,
                             const QRect &r,
                             const QColorGroup &cg,
                             SFlags flags = Style_Default,
                             SCFlags controls = SC_All,
                             SCFlags active = SC_None,
                             const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric( PixelMetric m,
                     const QWidget *widget = 0 ) const;

    QRect subRect( SubRect r,
                   const QWidget *widget ) const;

    QRect querySubControlMetrics( ComplexControl control,
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
    QWidget* hoverWidget;
    QColor *c1,*c2,*c3, *csel;
protected:
    void renderButton(QPainter *p,
                      const QRect &r,
                      const QColorGroup &g,
                      bool sunken = false,
                      bool mouseover = false) const;

    void renderPanel(QPainter *p,
                     const QRect &r,
                     const QColorGroup &g ) const;

    void renderSlider(QPainter *p,
                      const QRect &r,
                      const QColorGroup &g) const;

    bool eventFilter(QObject *, QEvent *);
//  void updatePalette( QComboBox * );
//  void updatePalette( QToolBar * );
//  void updatePalette( QMenuBar * );

protected slots:
    void slotDestroyed();
    void paletteChanged();


private:
// Disable copy constructor and = operator
    dotNET2style( const dotNET2style & );
    dotNET2style& operator=( const dotNET2style & );
    QStyle *winstyle;

    bool useTextShadows, reverseLayout, kickerMode;

    QValueList<QWidget*> m_widgets;
    QRect MyRect;
};

#endif

// vim: set noet ts=4 sw=4:
