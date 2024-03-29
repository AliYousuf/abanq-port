/**********************************************************************
** $Id: qt/qgpluginmanager_p.h   3.3.6   edited Aug 31 2005 $
**
** Definition of QGPluginManager class
**
** Copyright (C) 2000-2005 Trolltech AS.  All rights reserved.
**
** This file is part of the tools module of the Qt GUI Toolkit.
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

#ifndef QGPLUGINMANAGER_P_H
#define QGPLUGINMANAGER_P_H

#ifndef QT_H
#include "qdict.h"
#include "qlibrary.h"
#include "quuid.h"
#include "qstringlist.h"
#include "qcom_p.h"
#endif // QT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_NO_COMPONENT

#if defined(Q_TEMPLATEDLL)
// MOC_SKIP_BEGIN
//Q_TEMPLATE_EXTERN template class Q_EXPORT QDict<QLibrary>;
// MOC_SKIP_END
#endif

class Q_EXPORT QGPluginManager
{
public:
    QGPluginManager( const QUuid& id, const QStringList& paths = QString::null, const QString &suffix = QString::null, bool cs = TRUE );
    ~QGPluginManager();

    void addLibraryPath( const QString& path );
    const QLibrary* library( const QString& feature ) const;
    QStringList featureList() const;

    bool autoUnload() const;
    void setAutoUnload( bool );

protected:
    bool enabled() const;
    bool addLibrary( QLibrary* plugin );

    QRESULT queryUnknownInterface(const QString& feature, QUnknownInterface** iface) const;

    QUuid interfaceId;
    QDict<QLibrary> plugDict;	    // Dict to match feature with library
    QDict<QLibrary> libDict;	    // Dict to match library file with library
    QStringList libList;

    uint casesens : 1;
    uint autounload : 1;
};

inline void QGPluginManager::setAutoUnload( bool unload )
{
    autounload = unload;
}

inline bool QGPluginManager::autoUnload() const
{
    return autounload;
}

#endif

#endif //QGPLUGINMANAGER_P_H
