/**********************************************************************
** Copyright (C) 2000 Trolltech AS.  All rights reserved.
**
** This file is part of Qt Designer.
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
** See http://www.trolltech.com/gpl/ for GPL licensing information.
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef BROWSER_H
#define BROWSER_H

#include <qobject.h>
//Added by qt3to4:
#include <QEvent>

class Editor;
class Q3TextCursor;
class Q3TextParagraph;
class Q3TextFormat;

class EditorBrowser : public QObject
{
    Q_OBJECT

public:
    EditorBrowser( Editor *e );
    ~EditorBrowser();

    bool eventFilter( QObject *o, QEvent *e );
    virtual void setCurrentEdior( Editor *e );
    virtual void addEditor( Editor *e );
    virtual bool findCursor( const Q3TextCursor &c, Q3TextCursor &from, Q3TextCursor &to );
    virtual void showHelp( const QString & ) {}

protected:
    Editor *curEditor;
    Q3TextParagraph *oldHighlightedParag;
    QString lastWord;
    Q3TextFormat *highlightedFormat;

};

#endif