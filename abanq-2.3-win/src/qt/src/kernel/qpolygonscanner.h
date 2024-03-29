/****************************************************************************
** $Id: qt/qpolygonscanner.h   3.3.6   edited Aug 31 2005 $
**
** Definition of QPolygonScanner class
**
** Created : 000120
**
** Copyright (C) 1999-2005 Trolltech AS.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QPOLYGONSCANNER_H
#define QPOLYGONSCANNER_H

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H

class QPointArray;
class QPoint;

class Q_EXPORT QPolygonScanner {
public:
    // BIC: fix for 3.0
    void scan( const QPointArray& pa, bool winding, int index=0, int npoints=-1 );
    void scan( const QPointArray& pa, bool winding, int index, int npoints, bool stitchable );
    enum Edge { Left=1, Right=2, Top=4, Bottom=8 };
    void scan( const QPointArray& pa, bool winding, int index, int npoints, Edge edges );
    virtual void processSpans( int n, QPoint* point, int* width )=0;
};

#endif // QPOLYGONSCANNER_H
