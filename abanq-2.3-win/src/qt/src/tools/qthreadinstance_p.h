/****************************************************************************
** $Id: qt/qthreadinstance_p.h   3.3.6   edited Aug 31 2005 $
**
** ...
**
** Copyright (C) 2005 Trolltech AS.  All rights reserved.
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

#ifndef QTHREAD_P_H
#define QTHREAD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of QThread and QThreadStorage. This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifdef QT_THREAD_SUPPORT

#ifndef QT_H
#include "qmutex.h"
#include "qwindowdefs.h"
#endif // QT_H

//#ifdef Q_OS_UNIX
#include <pthread.h>
//#endif

class QThreadInstance {
public:
    static QThreadInstance *current();

    void init(unsigned int stackSize);
    void deinit();

    QMutex *mutex() const;
    void terminate();

    unsigned int stacksize;
    void *args[2];
    void **thread_storage;
    bool finished : 1;
    bool running  : 1;
    bool orphan   : 1;
/*
//#ifdef Q_OS_UNIX
    pthread_cond_t thread_done;
    pthread_t thread_id;

    static void *start( void * );
    static void finish( void * );
//#endif // Q_OS_UNIX

#if 0
//#ifdef Q_OS_WIN32
*/
    Qt::HANDLE handle;
    unsigned int thread_id;
    int waiters;

    static unsigned int __stdcall start( void * );
    static void finish( QThreadInstance * );
//#endif // Q_OS_WIN32
};

#endif // QT_THREAD_SUPPORT
#endif // QTHREAD_P_H
