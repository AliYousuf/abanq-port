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

#include "completion.h"
#include "paragdata.h"
#include "editor.h"
#include <q3listbox.h>
#include <q3vbox.h>
#include <qmap.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <Q3ValueList>
#include <Q3Frame>
#include <QEvent>
#include <private/q3richtext_p.h>
#include <qapplication.h>
#include <qregexp.h>
#include "arghintwidget.h"
#include <qsizegrip.h>
#include <qtimer.h>

static QColor getColor( const QString &type )
{
    if ( type == QString::fromLatin1("function") || type == QString::fromLatin1("slot") || type == QString::fromLatin1("package") )
	return Qt::blue;
    else if ( type == QString::fromLatin1("variable") || type == QString::fromLatin1("widget") || type == QString::fromLatin1("dir") )
	return Qt::darkRed;
    else if ( type == QString::fromLatin1("object") || type == QString::fromLatin1("class") )
	return Qt::darkBlue;
    else if ( type == QString::fromLatin1("property") )
	return Qt::darkGreen;
    else if ( type == QString::fromLatin1("enum") )
	return Qt::darkYellow;
    return Qt::black;
}

class CompletionItem : public Q3ListBoxItem
{
public:
    CompletionItem( Q3ListBox *lb, const QString &txt, const QString &t, const QString &p,
		    const QString &pre, const QString &p2 )
	: Q3ListBoxItem( lb ), type( t ), postfix( p ), prefix( pre ), postfix2( p2 ),
	  parag( 0 ), lastState( FALSE ) { setText( txt ); }
    ~CompletionItem() { delete parag; }
    void paint( QPainter *painter ) {
	if ( lastState != isSelected() ) {
	    delete parag;
	    parag = 0;
	}
	lastState = isSelected();
	if ( !parag )
	    setupParagraph();
	parag->paint( *painter, listBox()->colorGroup() );
    }

    int height( const Q3ListBox * ) const {
	if ( !parag )
	    ( (CompletionItem*)this )->setupParagraph();
	return parag->rect().height();
    }
    int width( const Q3ListBox * ) const {
	if ( !parag )
	    ( (CompletionItem*)this )->setupParagraph();
	return parag->rect().width() - 2;
    }
    QString text() const { return Q3ListBoxItem::text() + postfix; }

private:
    void setupParagraph();
    QString type, postfix, prefix, postfix2;
    Q3TextParagraph *parag;
    bool lastState;

};

void CompletionItem::setupParagraph() {
    if ( !parag ) {
  Q3TextFormatter *formatter;
	formatter = new QTextFormatterBreakWords;
	formatter->setWrapEnabled( FALSE );
  parag = new Q3TextParagraph( 0 );
	parag->setTabStops( listBox()->fontMetrics().width( QString::fromLatin1("propertyXXXX") ) );
	parag->pseudoDocument()->pFormatter = formatter;
	parag->insert( 0, " " + type + ( type.isEmpty() ? " " : "\t" ) + prefix +
		       Q3ListBoxItem::text() + postfix + postfix2 );
	bool selCol = isSelected() && listBox()->colorGroup().highlightedText() != listBox()->colorGroup().text();
	QColor sc = selCol ? listBox()->colorGroup().highlightedText() : getColor( type );
  Q3TextFormat *f1 = parag->formatCollection()->format( listBox()->font(), sc );
  Q3TextFormat *f3 = parag->formatCollection()->format( listBox()->font(), isSelected() ?
							     listBox()->colorGroup().highlightedText() :
							     listBox()->colorGroup().text() );
	QFont f( listBox()->font() );
	f.setBold( TRUE );
  Q3TextFormat *f2 =
	    parag->formatCollection()->format( f, isSelected() ? listBox()->colorGroup().highlightedText() :
					       listBox()->colorGroup().text() );
	parag->setFormat( 1, type.length() + 1, f1 );
	parag->setFormat( type.length() + 2, prefix.length() + Q3ListBoxItem::text().length(), f2 );
	if ( !postfix.isEmpty() )
	    parag->setFormat( type.length() + 2 + prefix.length() + Q3ListBoxItem::text().length(),
			      postfix.length(), f3 );
	parag->setFormat( type.length() + 2 + prefix.length() + Q3ListBoxItem::text().length() + postfix.length(),
			  postfix2.length(), f3 );
	f1->removeRef();
	f2->removeRef();
	f3->removeRef();
	parag->format();
    }
}


EditorCompletion::EditorCompletion( Editor *e )
{
    enabled = TRUE;
    lastDoc = 0;
    completionPopup = new Q3VBox( e->topLevelWidget(), 0, Qt::WType_Popup );
    completionPopup->setFrameStyle( Q3Frame::Box | Q3Frame::Plain );
    completionPopup->setLineWidth( 1 );
    functionLabel = new ArgHintWidget( e->topLevelWidget(), "editor_function_lbl" );
    functionLabel->hide();
    completionListBox = new Q3ListBox( completionPopup, "editor_completion_lb" );
    completionListBox->setFrameStyle( Q3Frame::NoFrame );
    completionListBox->installEventFilter( this );
    completionListBox->setHScrollBarMode( Q3ScrollView::AlwaysOn );
    completionListBox->setVScrollBarMode( Q3ScrollView::AlwaysOn );
    completionListBox->setCornerWidget( new QSizeGrip( completionListBox, "editor_cornerwidget" ) );
    completionPopup->installEventFilter( this );
    functionLabel->installEventFilter( this );
    completionPopup->setFocusProxy( completionListBox );
    completionOffset = 0;
    curEditor = e;
    curEditor->installEventFilter( this );
}

EditorCompletion::~EditorCompletion()
{
    delete completionPopup;
    delete functionLabel;
}

void EditorCompletion::addCompletionEntry( const QString &s, Q3TextDocument *, bool strict )
{
    QChar key( s[ 0 ] );
    QMap<QChar, QStringList>::Iterator it = completionMap.find( key );
    if ( it == completionMap.end() ) {
	completionMap.insert( key, QStringList( s ) );
    } else {
	if ( strict ) {
	    QStringList::Iterator sit;
	    for ( sit = (*it).begin(); sit != (*it).end(); ) {
		QStringList::Iterator it2 = sit;
		++sit;
		if ( (*it2).length() > s.length() && (*it2).left( s.length() ) == s ) {
		    if ( (*it2)[ (int)s.length() ].isLetter() && (*it2)[ (int)s.length() ].upper() != (*it2)[ (int)s.length() ] )
			return;
		} else if ( s.length() > (*it2).length() && s.left( (*it2).length() ) == *it2 ) {
		    if ( s[ (int)(*it2).length() ].isLetter() && s[ (int)(*it2).length() ].upper() != s[ (int)(*it2).length() ] )
			(*it).remove( it2 );
		}
	    }
	}
	(*it).append( s );
    }
}

Q3ValueList<CompletionEntry> EditorCompletion::completionList( const QString &s, Q3TextDocument *doc ) const
{
    if ( doc )
	( (EditorCompletion*)this )->updateCompletionMap( doc );

    QChar key( s[ 0 ] );
    QMap<QChar, QStringList>::ConstIterator it = completionMap.find( key );
    if ( it == completionMap.end() )
	return Q3ValueList<CompletionEntry>();
    QStringList::ConstIterator it2 = (*it).begin();
    Q3ValueList<CompletionEntry> lst;
    int len = s.length();
    for ( ; it2 != (*it).end(); ++it2 ) {
	CompletionEntry c;
	c.type = "";
	c.text = *it2;
	c.postfix = "";
	c.prefix = "";
	c.postfix2 = "";
	if ( (int)(*it2).length() > len && (*it2).left( len ) == s && lst.find( c ) == lst.end() )
	    lst << c;
    }

    return lst;
}

void EditorCompletion::updateCompletionMap( Q3TextDocument *doc )
{
    bool strict = TRUE;
    if ( doc != lastDoc )
	strict = FALSE;
    lastDoc = doc;
    Q3TextParagraph *s = doc->firstParagraph();
    if ( !s->extraData() )
	s->setExtraData( new ParagData );
    while ( s ) {
	if ( s->length() == ( (ParagData*)s->extraData() )->lastLengthForCompletion ) {
	    s = s->next();
	    continue;
	}

	QChar c;
	QString buffer;
	for ( int i = 0; i < s->length(); ++i ) {
	    c = s->at( i )->c;
	    if ( c.isLetter() || c.isNumber() || c == '_' || c == '#' ) {
		buffer += c;
	    } else {
		addCompletionEntry( buffer, doc, strict );
		buffer = QString::null;
	    }
	}
	if ( !buffer.isEmpty() )
	    addCompletionEntry( buffer, doc, strict );

	( (ParagData*)s->extraData() )->lastLengthForCompletion = s->length();
	s = s->next();
    }
}

bool EditorCompletion::doCompletion()
{
    searchString = "";
    if ( !curEditor )
	return FALSE;

    Q3TextCursor *cursor = curEditor->textCursor();
    Q3TextDocument *doc = curEditor->document();

    if ( cursor->index() > 0 && cursor->paragraph()->at( cursor->index() - 1 )->c == '.' &&
	 ( cursor->index() == 1 || cursor->paragraph()->at( cursor->index() - 2 )->c != '.' ) )
	return doObjectCompletion();

    int idx = cursor->index();
    if ( idx == 0 )
	return FALSE;
    QChar c = cursor->paragraph()->at( idx - 1 )->c;
    if ( !c.isLetter() && !c.isNumber() && c != '_' && c != '#' )
	return FALSE;

    QString s;
    idx--;
    completionOffset = 1;
    for (;;) {
	s.prepend( QString( cursor->paragraph()->at( idx )->c ) );
	idx--;
	if ( idx < 0 )
	    break;
	if ( !cursor->paragraph()->at( idx )->c.isLetter() &&
	     !cursor->paragraph()->at( idx )->c.isNumber() &&
	     cursor->paragraph()->at( idx )->c != '_' &&
	     cursor->paragraph()->at( idx )->c != '#' )
	    break;
	completionOffset++;
    }

    searchString = s;

    Q3ValueList<CompletionEntry> lst( completionList( s, doc ) );
    if ( lst.count() > 1 ) {
  Q3TextStringChar *chr = cursor->paragraph()->at( cursor->index() );
	int h = cursor->paragraph()->lineHeightOfChar( cursor->index() );
	int x = cursor->paragraph()->rect().x() + chr->x;
	int y, dummy;
	cursor->paragraph()->lineHeightOfChar( cursor->index(), &dummy, &y );
	y += cursor->paragraph()->rect().y();
	completionListBox->clear();
	for ( Q3ValueList<CompletionEntry>::ConstIterator it = lst.begin(); it != lst.end(); ++it )
	    (void)new CompletionItem( completionListBox, (*it).text, (*it).type, (*it).postfix,
				      (*it).prefix, (*it).postfix2 );
	cList = lst;
	completionPopup->resize( completionListBox->sizeHint() +
				 QSize( completionListBox->verticalScrollBar()->width() + 4,
					completionListBox->horizontalScrollBar()->height() + 4 ) );
	completionListBox->setCurrentItem( 0 );
	completionListBox->setFocus();
	if ( curEditor->mapToGlobal( QPoint( 0, y ) ).y() + h + completionPopup->height() < QApplication::desktop()->height() )
	    completionPopup->move( curEditor->mapToGlobal( curEditor->
							   contentsToViewport( QPoint( x, y + h ) ) ) );
	else
	    completionPopup->move( curEditor->mapToGlobal( curEditor->
							   contentsToViewport( QPoint( x, y - completionPopup->height() ) ) ) );
	completionPopup->show();
    } else if ( lst.count() == 1 ) {
	curEditor->insert( lst.first().text.mid( completionOffset, 0xFFFFFF ),
 			   (uint) ( Q3TextEdit::RedoIndentation |
				    Q3TextEdit::CheckNewLines |
				    Q3TextEdit::RemoveSelected ) );
    } else {
	return FALSE;
    }

    return TRUE;
}

bool EditorCompletion::eventFilter( QObject *o, QEvent *e )
{
    if ( !enabled )
	return FALSE;
    if ( e->type() == QEvent::KeyPress &&
#if (QT_VERSION) < 0x030200
	 o->inherits("Editor")
#else
   static_cast<Editor*>(o)
#endif
	 ) {
	curEditor = (Editor*)o;
	QKeyEvent *ke = (QKeyEvent*)e;
	if ( ke->key() == Qt::Key_Tab ) {
	    QString s = curEditor->textCursor()->paragraph()->string()->toString().
			left( curEditor->textCursor()->index() );
      if ( curEditor->document()->hasSelection( Q3TextDocument::Standard ) ||
		 s.simplifyWhiteSpace().isEmpty() ) {
		if ( curEditor->document()->indent() ) {
		    curEditor->indent();
		    int i = 0;
		    for ( ; i < curEditor->textCursor()->paragraph()->length() - 1; ++i ) {
			if ( curEditor->textCursor()->paragraph()->at( i )->c != ' ' &&
			     curEditor->textCursor()->paragraph()->at( i )->c != '\t' )
			    break;
		    }
		    curEditor->drawCursor( FALSE );
		    curEditor->textCursor()->setIndex( i );
		    curEditor->drawCursor( TRUE );
		} else {
		    curEditor->insert( QString::fromLatin1("\t") );
		}
		return TRUE;
	    }
	}

	if ( functionLabel->isVisible() ) {
	    if ( ke->key() == Qt::Key_Up && ( ke->state() & Qt::ControlModifier ) == Qt::ControlModifier ) {
		functionLabel->gotoPrev();
		return TRUE;
	    } else if ( ke->key() == Qt::Key_Down && ( ke->state() & Qt::ControlModifier ) == Qt::ControlModifier ) {
		functionLabel->gotoNext();
		return TRUE;
	    }
	}

	if ( ke->text().length() && !( ke->state() & Qt::AltModifier ) &&
	     ( !ke->ascii() || ke->ascii() >= 32 ) ||
	     ( ke->text() == QString::fromLatin1("\t") && !( ke->state() & Qt::ControlModifier ) ) ) {
	    if ( ke->key() == Qt::Key_Tab ) {
		if ( curEditor->textCursor()->index() == 0 &&
		     curEditor->textCursor()->paragraph()->isListItem() )
		    return FALSE;
		if ( doCompletion() )
			return TRUE;
	    } else if ( ke->key() == Qt::Key_Period &&
			( curEditor->textCursor()->index() == 0 ||
			  curEditor->textCursor()->paragraph()->at( curEditor->textCursor()->index() - 1 )->c != '.' )
			||
			ke->key() == Qt::Key_Greater &&
			curEditor->textCursor()->index() > 0 &&
			curEditor->textCursor()->paragraph()->at( curEditor->textCursor()->index() - 1 )->c == '-' ) {
		doObjectCompletion();
	    } else {
		if ( !doArgumentHint( ke->text() == QString::fromLatin1("(") ) )
		    functionLabel->hide();
	    }
	}
    } else if ( o == completionPopup || o == completionListBox ||
	 o == completionListBox->viewport() ) {
	if ( e->type() == QEvent::KeyPress ) {
	    QKeyEvent *ke = (QKeyEvent*)e;
	    if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Tab ) {
		if ( ke->key() == Qt::Key_Tab && completionListBox->count() > 1 &&
		     completionListBox->currentItem() < (int)completionListBox->count() - 1 ) {
		    completionListBox->setCurrentItem( completionListBox->currentItem() + 1 );
		    return TRUE;
		}
		completeCompletion();
		return TRUE;
	    } else if ( ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right ||
			ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down ||
			ke->key() == Qt::Key_Home || ke->key() == Qt::Key_End ||
			ke->key() == Qt::Key_PageUp || ke->key() == Qt::Key_PageDown ) {
		return FALSE;
	    } else if ( ke->key() != Qt::Key_Shift && ke->key() != Qt::Key_Control &&
			ke->key() != Qt::Key_Alt ) {
		int l = searchString.length();
		if ( ke->key() == Qt::Key_Backspace ) {
		    searchString.remove( searchString.length() - 1, 1 );
		} else {
		    searchString += ke->text();
		    l = 1;
		}
		if ( !l || !continueComplete() ) {
		    completionPopup->close();
		    curEditor->setFocus();
		}
		QApplication::sendEvent( curEditor, e );
		return TRUE;
	    }
	} else if ( e->type() == QEvent::MouseButtonDblClick ) {
	    completeCompletion();
	    return TRUE;
	}
    }
    if ( ( o == functionLabel ||
#if (QT_VERSION) < 0x030200
	   o->inherits("Editor")
#else
     static_cast<Editor*>(o)
#endif
	   ) && functionLabel->isVisible() ) {
	if ( e->type() == QEvent::KeyPress ) {
	    QKeyEvent *ke = (QKeyEvent*)e;
	    if ( ke->key() == Qt::Key_Escape ) {
		functionLabel->hide();
	    } else {
		if ( !doArgumentHint( ke->text() == QString::fromLatin1("(") ) )
		    functionLabel->hide();
		if ( o == functionLabel ) {
		    QApplication::sendEvent( curEditor, e );
		    return TRUE;
		}
	    }
	}
    }
    return FALSE;
}

void EditorCompletion::completeCompletion()
{
    int idx = curEditor->textCursor()->index();
    QString s = completionListBox->currentText().mid( searchString.length() );
    curEditor->insert( s, (uint) ( Q3TextEdit::RedoIndentation |
				   Q3TextEdit::CheckNewLines |
				   Q3TextEdit::RemoveSelected ) );
    int i = s.find( '(' );
    completionPopup->close();
    curEditor->setFocus();
    if ( i != -1 && i < (int)s.length() ) {
	curEditor->setCursorPosition( curEditor->textCursor()->paragraph()->paragId(), idx + i + 1 );
	doArgumentHint( FALSE );
    }
}

void EditorCompletion::setCurrentEdior( Editor *e )
{
    curEditor = e;
    curEditor->installEventFilter( this );
}

void EditorCompletion::addEditor( Editor *e )
{
    e->installEventFilter( this );
}

bool EditorCompletion::doObjectCompletion()
{
    searchString = "";
    QString object;
    int i = curEditor->textCursor()->index();
    i--;
    Q3TextParagraph *p = curEditor->textCursor()->paragraph();
    for (;;) {
	if ( i < 0 )
	    break;
	if ( p->at( i )->c == ' ' || p->at( i )->c == '\t' )
	    break;
	object.prepend( p->at( i )->c );
	i--;
    }

    if (object.isEmpty())
	return false;

    if ( object[ (int)object.length() - 1 ] == '-' )
	object.remove( object.length() - 1, 1 );

    return doObjectCompletion( object );
}

bool EditorCompletion::doObjectCompletion( const QString & )
{
    return FALSE;
}

static void strip( QString &txt )
{
    int i = txt.find( QString::fromLatin1("(") );
    if ( i == -1 )
	return;
    txt = txt.left( i );
}

bool EditorCompletion::continueComplete()
{
    if ( searchString.isEmpty() ) {
	completionListBox->clear();
	for ( Q3ValueList<CompletionEntry>::ConstIterator it = cList.begin(); it != cList.end(); ++it )
	    (void)new CompletionItem( completionListBox, (*it).text, (*it).type,
				      (*it).postfix, (*it).prefix, (*it).postfix2 );
	completionListBox->setCurrentItem( 0 );
	completionListBox->setSelected( completionListBox->currentItem(), TRUE );
	return TRUE;
    }

    Q3ListBoxItem *i = completionListBox->findItem( searchString );
    if ( !i )
	return FALSE;

    QString txt1 = i->text();
    QString txt2 = searchString;
    strip( txt1 );
    strip( txt2 );
    if ( txt1 == txt2 && !i->next() )
	return FALSE;

    Q3ValueList<CompletionEntry> res;
    for ( Q3ValueList<CompletionEntry>::ConstIterator it = cList.begin(); it != cList.end(); ++it ) {
	if ( (*it).text.left( searchString.length() ) == searchString )
	    res << *it;
    }
    if ( res.isEmpty() )
	return FALSE;
    completionListBox->clear();
    for ( Q3ValueList<CompletionEntry>::ConstIterator it2 = res.begin(); it2 != res.end(); ++it2 )
	(void)new CompletionItem( completionListBox, (*it2).text, (*it2).type,
				  (*it2).postfix, (*it2).prefix, (*it2).postfix2 );
    completionListBox->setCurrentItem( 0 );
    completionListBox->setSelected( completionListBox->currentItem(), TRUE );
    return TRUE;
}

bool EditorCompletion::doArgumentHint( bool useIndex )
{
    Q3TextCursor *cursor = curEditor->textCursor();
    int i = cursor->index() ;
    if ( !useIndex ) {
	bool foundParen = FALSE;
	int closeParens = 0;
	while ( i >= 0 ) {
	    if ( cursor->paragraph()->at( i )->c == ')' && i != cursor->index() )
		closeParens++;
	    if ( cursor->paragraph()->at( i )->c == '(' ) {
		closeParens--;
		if ( closeParens == -1 ) {
		    foundParen = TRUE;
		    break;
		}
	    }
	    --i;
	}

	if ( !foundParen )
	    return FALSE;
    }
    int j = i - 1;
    bool foundSpace = FALSE;
    bool foundNonSpace = FALSE;
    while ( j >= 0 ) {
	if ( foundNonSpace && ( cursor->paragraph()->at( j )->c == ' ' || cursor->paragraph()->at( j )->c == ',' ) ) {
	    foundSpace = TRUE;
	    break;
	}
	if ( !foundNonSpace && ( cursor->paragraph()->at( j )->c != ' ' || cursor->paragraph()->at( j )->c != ',' ) )
	    foundNonSpace = TRUE;
	--j;
    }
    if ( foundSpace )
	++j;
    j = QMAX( j, 0 );
    QString function( cursor->paragraph()->string()->toString().mid( j, i - j + 1 ) );
    QString part = cursor->paragraph()->string()->toString().mid( j, cursor->index() - j + 1 );
    function = function.simplifyWhiteSpace();
    for (;;) {
	if ( function[ (int)function.length() - 1 ] == '(' ) {
	    function.remove( function.length() - 1, 1 );
	    function = function.simplifyWhiteSpace();
	} else if ( function[ (int)function.length() - 1 ] == ')' ) {
	    function.remove( function.length() - 1, 1 );
	    function = function.simplifyWhiteSpace();
	} else {
	    break;
	}
    }

    QChar sep;
    QString pre, post;
    Q3ValueList<QStringList> argl = functionParameters( function, sep, pre, post );
    if ( argl.isEmpty() )
	return FALSE;

    QString label;
    int w = 0;
    int num = 0;
    if ( !functionLabel->isVisible() )
	functionLabel->setNumFunctions( argl.count() );
    for ( Q3ValueList<QStringList>::Iterator vit = argl.begin(); vit != argl.end(); ++vit, ++num ) {
	QStringList args = *vit;
	int argNum = 0;
	int inParen = 0;
	for ( int k = 0; k < (int)part.length(); ++k ) {
	    if ( part[ k ] == sep && inParen < 2 )
		argNum++;
	    if ( part[ k ] == '(' )
		inParen++;
	    if ( part[ k ] == ')' )
		inParen--;
	}

	QString func = function;
	int pnt = -1;
	pnt = func.findRev( '.' );
	if ( pnt == -1 )
	    func.findRev( '>' );
	if ( pnt != -1 )
	    func = func.mid( pnt + 1 );

	QString s = func + "( ";
	if ( s[ 0 ] == '\"' )
	    s.remove( (uint)0, 1 );
	i = 0;
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it, ++i ) {
	    if ( i == argNum )
		s += "<b>" + *it + "</b>";
	    else
		s += *it;
	    if ( i < (int)args.count() - 1 )
		s += QString::fromLatin1(", ");
	    else
		s += QString::fromLatin1(" ");
	}
	s += QString::fromLatin1(")");
	s.prepend( pre );
	s.append( post );
	label += "<p>" + s + "</p>";
	functionLabel->setFunctionText( num, s );
	w = QMAX( w, functionLabel->fontMetrics().width( s ) + 10 );
    }
    w += 16;
    if ( label.isEmpty() )
	return FALSE;
    if ( functionLabel->isVisible() ) {
	functionLabel->resize( w + 50, QMAX( functionLabel->fontMetrics().height(), 16 ) );
    } else {
  Q3TextStringChar *chr = cursor->paragraph()->at( cursor->index() );
	int h = cursor->paragraph()->lineHeightOfChar( cursor->index() );
	int x = cursor->paragraph()->rect().x() + chr->x;
	int y, dummy;
	cursor->paragraph()->lineHeightOfChar( cursor->index(), &dummy, &y );
	y += cursor->paragraph()->rect().y();
	functionLabel->resize( w + 50, QMAX( functionLabel->fontMetrics().height(), 16 ) );
	functionLabel->move( curEditor->mapToGlobal( curEditor->contentsToViewport( QPoint( x, y + h ) ) ) );
	if ( functionLabel->x() + functionLabel->width() > QApplication::desktop()->width() )
	    functionLabel->move( QMAX( 0, QApplication::desktop()->width() - functionLabel->width() ),
				 functionLabel->y() );
	functionLabel->show();
	curEditor->setFocus();
    }
    QTimer::singleShot( 0, functionLabel, SLOT( relayout() ) );

    return TRUE;
}

Q3ValueList<QStringList> EditorCompletion::functionParameters( const QString &, QChar &, QString &, QString & )
{
    return Q3ValueList<QStringList>();
}

void EditorCompletion::setContext( QObject * )
{
}

void EditorCompletion::showCompletion( const Q3ValueList<CompletionEntry> &lst )
{
    Q3TextCursor *cursor = curEditor->textCursor();
    QTextStringChar *chr = cursor->paragraph()->at( cursor->index() );
    int h = cursor->paragraph()->lineHeightOfChar( cursor->index() );
    int x = cursor->paragraph()->rect().x() + chr->x;
    int y, dummy;
    cursor->paragraph()->lineHeightOfChar( cursor->index(), &dummy, &y );
    y += cursor->paragraph()->rect().y();
    completionListBox->clear();
    for ( Q3ValueList<CompletionEntry>::ConstIterator it = lst.begin(); it != lst.end(); ++it )
	(void)new CompletionItem( completionListBox, (*it).text, (*it).type,
				  (*it).postfix, (*it).prefix, (*it).postfix2 );
    cList = lst;
    completionPopup->resize( completionListBox->sizeHint() +
			     QSize( completionListBox->verticalScrollBar()->width() + 4,
				    completionListBox->horizontalScrollBar()->height() + 4 ) );
    completionListBox->setCurrentItem( 0 );
    completionListBox->setFocus();
    if ( curEditor->mapToGlobal( QPoint( 0, y ) ).y() + h + completionPopup->height() < QApplication::desktop()->height() )
	completionPopup->move( curEditor->mapToGlobal( curEditor->
						       contentsToViewport( QPoint( x, y + h ) ) ) );
    else
	completionPopup->move( curEditor->mapToGlobal( curEditor->
						       contentsToViewport( QPoint( x, y - completionPopup->height() ) ) ) );

    completionPopup->show();
}
