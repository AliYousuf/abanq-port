/****************************************************************************
** $Id: qt/quuid.h   3.3.6   edited Aug 31 2005 $
**
** Definition of QUuid class
**
** Created : 010523
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
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

#ifndef QUUID_H
#define QUUID_H

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

#include <string.h>

#if defined(Q_OS_WIN32)
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
    ulong   Data1;
    ushort  Data2;
    ushort  Data3;
    uchar   Data4[ 8 ];
} GUID, *REFGUID, *LPGUID;
#endif
#endif


struct Q_EXPORT QUuid
{
    enum Variant {
	VarUnknown	=-1,
	NCS		= 0, // 0 - -
	DCE		= 2, // 1 0 -
	Microsoft	= 6, // 1 1 0
	Reserved	= 7  // 1 1 1
    };

    enum Version {
	VerUnknown	=-1,
	Time		= 1, // 0 0 0 1
	EmbeddedPOSIX	= 2, // 0 0 1 0
	Name		= 3, // 0 0 1 1
	Random		= 4  // 0 1 0 0
    };

    QUuid()
    {
	memset( this, 0, sizeof(QUuid) );
    }
    QUuid( uint l, ushort w1, ushort w2, uchar b1, uchar b2, uchar b3, uchar b4, uchar b5, uchar b6, uchar b7, uchar b8 )
    {
	data1 = l;
	data2 = w1;
	data3 = w2;
	data4[0] = b1;
	data4[1] = b2;
	data4[2] = b3;
	data4[3] = b4;
	data4[4] = b5;
	data4[5] = b6;
	data4[6] = b7;
	data4[7] = b8;
    }
    QUuid( const QUuid &uuid )
    {
	memcpy( this, &uuid, sizeof(QUuid) );
    }
#ifndef QT_NO_QUUID_STRING
    QUuid( const QString & );
    QUuid( const char * );
    QString toString() const;
    operator QString() const { return toString(); }
#endif
    bool isNull() const;

    QUuid &operator=(const QUuid &orig )
    {
	memcpy( this, &orig, sizeof(QUuid) );
	return *this;
    }

    bool operator==(const QUuid &orig ) const
    {
	uint i;
	if ( data1 != orig.data1 || data2 != orig.data2 || 
	     data3 != orig.data3 )
	    return FALSE;

	for( i = 0; i < 8; i++ )
	    if ( data4[i] != orig.data4[i] )
		return FALSE;
	
	return TRUE;
    }

    bool operator!=(const QUuid &orig ) const
    {
	return !( *this == orig );
    }

    bool operator<(const QUuid &other ) const;
    bool operator>(const QUuid &other ) const;

#if defined(Q_OS_WIN32)
    // On Windows we have a type GUID that is used by the platform API, so we
    // provide convenience operators to cast from and to this type.
    QUuid( const GUID &guid )
    {
	memcpy( this, &guid, sizeof(GUID) );
    }

    QUuid &operator=(const GUID &orig )
    {
	memcpy( this, &orig, sizeof(QUuid) );
	return *this;
    }

    operator GUID() const
    {
	GUID guid = { data1, data2, data3, { data4[0], data4[1], data4[2], data4[3], data4[4], data4[5], data4[6], data4[7] } };
	return guid;
    }

    bool operator==( const GUID &guid ) const
    {
	uint i;
	if ( data1 != guid.Data1 || data2 != guid.Data2 || 
	     data3 != guid.Data3 )
	    return FALSE;

	for( i = 0; i < 8; i++ )
	    if ( data4[i] != guid.Data4[i] )
		return FALSE;
	
	return TRUE;
    }

    bool operator!=( const GUID &guid ) const
    {
	return !( *this == guid );
    }
#endif
    static QUuid createUuid();
    QUuid::Variant variant() const;
    QUuid::Version version() const;

    uint    data1;
    ushort  data2;
    ushort  data3;
    uchar   data4[ 8 ];
};

#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream &, const QUuid & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QUuid & );
#endif

#endif //QUUID_H
