/****************************************************************************
** $Id: qstyleplugin.h,v 1.1.1.8 2005/09/22 12:55:45 chehrlic Exp $
**
** Definition of QStylePlugin class
**
** Created : 010920
**
** Copyright (C) 2005 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QSTYLEPLUGIN_H
#define QSTYLEPLUGIN_H

#ifndef QT_H
#include "qgplugin.h"
#include "qstringlist.h"
#endif // QT_H

#ifndef QT_NO_STYLE
#ifndef QT_NO_COMPONENT

class QStyle;
class QStylePluginPrivate;

class Q_EXPORT QStylePlugin : public QGPlugin
{
    Q_OBJECT
public:
    QStylePlugin();
    ~QStylePlugin();

    virtual QStringList keys() const = 0;
    virtual QStyle *create( const QString &key ) = 0;

private:
    QStylePluginPrivate *d;
};

#endif // QT_NO_COMPONENT
#endif // QT_NO_STYLE

#endif // QSTYLEPLUGIN_H
