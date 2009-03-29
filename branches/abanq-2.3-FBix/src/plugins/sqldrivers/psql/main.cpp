/***************************************************************************
                        main.cpp  -  description
                           -------------------
  begin                : mie Sep 15 2004
  copyright            : (C) 2004 by InfoSiAL S.L.
  email                : mail@infosial.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/****************************************************************************
**
** Implementation of PostgreSQL driver plugin
**
** Created : 001103
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the sql module of the Qt GUI Toolkit.
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
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Commercial License Agreement provided
** with the Software.
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

#include <qsqldriverplugin.h>
#include "qsql_psql.h"

class QPSQLDriverPlugin: public QSqlDriverPlugin
{

public:

    QPSQLDriverPlugin();

    QSqlDriver *create( const QString & );
    QStringList keys() const;
};

QPSQLDriverPlugin::QPSQLDriverPlugin() : QSqlDriverPlugin() {}

QSqlDriver *QPSQLDriverPlugin::create( const QString & name )
{
    if ( name == "FLQPSQL7" ) {
        QPSQLDriver * driver = new QPSQLDriver();
        return driver;
    }
    return 0;
}

QStringList QPSQLDriverPlugin::keys() const
{
    QStringList l;
    l.append( "FLQPSQL7" );
    return l;
}

Q_EXPORT_PLUGIN( QPSQLDriverPlugin )
