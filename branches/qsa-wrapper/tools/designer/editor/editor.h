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

#ifndef EDITOR_H
#define EDITOR_H

#include <q3textedit.h>
//Added by qt3to4:
#include <QEvent>
#include <QKeyEvent>
#include <Q3PopupMenu>

struct Config;
class ParenMatcher;
class EditorCompletion;
class EditorBrowser;
class Q3Accel;

class Editor : public Q3TextEdit
{
    Q_OBJECT

public:
    enum Selection {
	Error = 3,
	Step = 4
    };

    Editor( const QString &fn, QWidget *parent, const char *name );
    ~Editor();
    virtual void load( const QString &fn );
    virtual void save( const QString &fn );
    Q3TextDocument *document() const { return Q3TextEdit::document(); }
    void placeCursor( const QPoint &p, Q3TextCursor *c ) { Q3TextEdit::placeCursor( p, c ); }
    void setDocument( Q3TextDocument *doc ) { Q3TextEdit::setDocument( doc ); }
    Q3TextCursor *textCursor() const { return Q3TextEdit::textCursor(); }
    void repaintChanged() { Q3TextEdit::repaintChanged(); }

    virtual EditorCompletion *completionManager() { return 0; }
    virtual EditorBrowser *browserManager() { return 0; }
    virtual void configChanged();

    Config *config() { return cfg; }

    void setErrorSelection( int line );
    void setStepSelection( int line );
    void clearStepSelection();
    void clearSelections();

    virtual bool supportsErrors() const { return TRUE; }
    virtual bool supportsBreakPoints() const { return TRUE; }
    virtual void makeFunctionVisible( Q3TextParagraph * ) {}

    void drawCursor( bool b ) { Q3TextEdit::drawCursor( b ); }

    Q3PopupMenu *createPopupMenu( const QPoint &p );
    bool eventFilter( QObject *o, QEvent *e );

    void setEditable( bool b ) { editable = b; }

protected:
    void doKeyboardAction( KeyboardAction action );
    void keyPressEvent( QKeyEvent *e );

signals:
    void clearErrorMarker();
    void intervalChanged();

private slots:
    void cursorPosChanged( Q3TextCursor *c );
    void doChangeInterval();
    void commentSelection();
    void uncommentSelection();

protected:
    ParenMatcher *parenMatcher;
    QString filename;
    Config *cfg;
    bool hasError;
    Q3Accel *accelComment, *accelUncomment;
    bool editable;

};

#endif
