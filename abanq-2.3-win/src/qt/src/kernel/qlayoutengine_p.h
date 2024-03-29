/****************************************************************************
** $Id: qt/qlayoutengine_p.h   3.3.6   edited May 27 2003 $
**
** Internal header file.
**
** Created : 981027
**
** Copyright (C) 1998-99 by Trolltech AS.  All rights reserved.
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

#ifndef QLAYOUTENGINE_P_H
#define QLAYOUTENGINE_P_H

#ifndef QLAYOUT_H
    #error "Need to include qlayout.h before including qlayoutengine_p.h"
#endif

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qlayout.cpp, qlayoutengine.cpp, qmainwindow.cpp and qsplitter.cpp.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//
//


#ifndef QT_H
#include "qabstractlayout.h"
#endif // QT_H

#ifndef QT_NO_LAYOUT

struct QLayoutStruct
{
    inline void init( int stretchFactor = 0, int spacing = 0 ) {
	stretch = stretchFactor;
	minimumSize = sizeHint = spacing;
	maximumSize = QLAYOUTSIZE_MAX;
	expansive = FALSE;
	empty = TRUE;
    }

    QCOORD smartSizeHint() {
	return ( stretch > 0 ) ? minimumSize : sizeHint;
    }

    // parameters
    int stretch;
    QCOORD sizeHint;
    QCOORD maximumSize;
    QCOORD minimumSize;
    bool expansive;
    bool empty;

    // temporary storage
    bool done;

    // result
    int pos;
    int size;
};


Q_EXPORT void qGeomCalc( QMemArray<QLayoutStruct> &chain, int start, int count,
			 int pos, int space, int spacer );
Q_EXPORT QSize qSmartMinSize( const QWidgetItem *i );
Q_EXPORT QSize qSmartMinSize( QWidget *w );
Q_EXPORT QSize qSmartMaxSize( const QWidgetItem *i, int align = 0 );
Q_EXPORT QSize qSmartMaxSize( QWidget *w, int align = 0 );


/*
  Modify total maximum (max) and total expansion (exp)
  when adding boxmax/boxexp.

  Expansive boxes win over non-expansive boxes.
*/
static inline void qMaxExpCalc( QCOORD & max, bool &exp,
				QCOORD boxmax, bool boxexp )
{
    if ( exp ) {
	if ( boxexp )
	    max = QMAX( max, boxmax );
    } else {
	if ( boxexp )
	    max = boxmax;
	else
	    max = QMIN( max, boxmax );
    }
    exp = exp || boxexp;
}

#endif //QT_NO_LAYOUT
#endif
