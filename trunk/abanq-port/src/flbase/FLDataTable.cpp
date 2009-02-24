/***************************************************************************
                           FLDataTable.cpp
                         -------------------
begin                : Sun Jul 1 2001
copyright            : (C) 2001-2005 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLDataTable.h"
#include "FLSqlCursor.h"
#include "FLTableMetaData.h"
#include "FLFieldMetaData.h"
#include "FLUtil.h"
#include "FLApplication.h"
#include "FLSqlQuery.h"
#include "FLSqlDatabase.h"
#include "FLManager.h"

void FLCheckBox::drawButton( QPainter * p ) {
  QRect rect, wrect( this->rect() );
  rect.setRect( ( wrect.width() - 13 ) / 2, ( wrect.height() - 13 ) / 2, 13, 13 );

  if ( state() == QButton::On ) {
    QBrush bu;
    bu.setColor( green );
    bu.setStyle( SolidPattern );
    p->fillRect( 1, 1, wrect.width() - 1, wrect.height() - 1, bu );
  }

  QRect irect = QStyle::visualRect( rect, this );
  p->fillRect( irect, Qt::white );
  p->drawRect( irect );

  if ( state() == QButton::On ) {
    QPointArray a( 7*2 );
    int i, xx, yy;
    xx = irect.x() + 3;
    yy = irect.y() + 5;

    for ( i = 0; i < 3; i++ ) {
      a.setPoint( 2*i,   xx, yy );
      a.setPoint( 2*i + 1, xx, yy + 2 );
      xx++;
      yy++;
    }

    yy -= 2;
    for ( i = 3; i < 7; i++ ) {
      a.setPoint( 2*i,   xx, yy );
      a.setPoint( 2*i + 1, xx, yy + 2 );
      xx++;
      yy--;
    }

    p->drawLineSegments( a );
  }

  drawButtonLabel( p );
}

bool FLCheckBox::hitButton( const QPoint & pos ) const {
  return this->rect().contains( pos );
}

FLDataTable::FLDataTable( QWidget * parent, const char * name, bool popup )
    : QDataTable( parent, name ),
    rowSelected( -1 ), colSelected( -1 ), cursor_( 0 ), readonly_( false ),
    editonly_( false ), insertonly_( false ), persistentFilter_( QString::null ),
    refreshing_( false ), popup_( popup ) {
  if ( !name )
    setName( "FLDataTable" );
  bu_.setColor( qApp->palette().color( QPalette::Active, QColorGroup::Midlight ) );
  bu_.setStyle( SolidPattern );
  ok = QPixmap::fromMimeSource( "unlock.png" );
  no = QPixmap::fromMimeSource( "lock.png" );
}

FLDataTable::~FLDataTable() {}

void FLDataTable::selectRow( int r, int c ) {
  if ( !cursor_ || !cursor_->metadata() )
    return ;

  if ( r < 0 ) {
    if ( cursor_->isValid() ) {
      rowSelected = cursor_->at();
      colSelected = currentColumn();
    } else {
      rowSelected = 0;
      colSelected = 0;
    }
  } else {
    rowSelected = r;
    colSelected = c;
  }

  QObject * snd = const_cast<QObject *>( sender() );
  if ( !snd || ( snd && !snd ->isA( "FLSqlCursor" ) ) ) {
    QWidget * sndw = ::qt_cast<QWidget *>( snd );
    if ( sndw ) {
      if ( !sndw->hasFocus() || !sndw->isVisible() ) {
        setCurrentCell( rowSelected, colSelected );
        return ;
      }
    }
    if ( numRows() != cursor_->size() )
      setNumRows( cursor_->size() );
    cursor_->seek( rowSelected );
  }
  setCurrentCell( rowSelected, colSelected );
}

void FLDataTable::setFLSqlCursor( FLSqlCursor * c ) {
  if ( c && c->metadata() ) {
    if ( !cursor_ ) {
      disconnect( this, SIGNAL( currentChanged( int, int ) ), this, SLOT( selectRow( int, int ) ) );
      disconnect( this, SIGNAL( clicked( int, int, int, const QPoint & ) ), this, SLOT( selectRow( int, int ) ) );
      connect( this, SIGNAL( currentChanged( int, int ) ), this, SLOT( selectRow( int, int ) ) );
      connect( this, SIGNAL( clicked( int, int, int, const QPoint & ) ), this, SLOT( selectRow( int, int ) ) );
    } else {
      disconnect( cursor_, SIGNAL( currentChanged( int ) ), this, SLOT( selectRow( int ) ) );
      if ( !popup_ )
        disconnect( cursor_, SIGNAL( cursorUpdated() ), this, SLOT( refresh() ) );
    }

    cursor_ = c;
    if ( cursor_ ) {
      disconnect( cursor_, SIGNAL( currentChanged( int ) ), this, SLOT( selectRow( int ) ) );
      if ( !popup_ )
        disconnect( cursor_, SIGNAL( cursorUpdated() ), this, SLOT( refresh() ) );
      connect( cursor_, SIGNAL( currentChanged( int ) ), this, SLOT( selectRow( int ) ) );
      if ( !popup_ )
        connect( cursor_, SIGNAL( cursorUpdated() ), this, SLOT( refresh() ) );
    }

    QDataTable::setSqlCursor( static_cast<QSqlCursor *>( c ), true, false );
  }
}

void FLDataTable::paintCell( QPainter * p, int row, int col, const QRect & cr,
                             bool selected, const QColorGroup & cg ) {
  QTable::paintCell( p, row, col, cr, selected, cg );

  FLTableMetaData * tMD;
  if ( !cursor_ || !( tMD = cursor_->metadata() ) )
    return ;

  int type = tMD->fieldType( cursor_->field( indexOf( col ) ) ->name() );
  if ( type == QVariant::Pixmap && row != rowSelected )
    return ;

  p->setPen( selected ? cg.highlightedText() : cg.text() );

  QSqlField * field = cursor_->field( indexOf( col ) );
  lastTextPainted = QString::null;
  if ( cursor_->QSqlCursor::seek( row ) ) {
    if ( !selected && ( row % 2 ) )
      p->fillRect( 0, 0, cr.width() - 2, cr.height() - 2, bu_ );
    paintField( p, field, cr, selected );
  } else
    return;

  cursor_->QSqlCursor::seek( rowSelected );

  int wC = columnWidth( col ), wH = fontMetrics().width( horizontalHeader() ->label( col ) + "W" );
  if ( wH < wC )
    wH = wC;
  wC = fontMetrics().width( lastTextPainted ) + fontMetrics().maxWidth();
  if ( wC > wH ) {
    QTable::setColumnWidth( col, wC );
#if defined(Q_OS_MACX) || defined(Q_OS_WIN32)
    QTable::repaintContents();
#endif
  }
}

void FLDataTable::paintField( QPainter * p, const QSqlField * field,
                              const QRect & cr, bool ) {
  if ( !field )
    return;

  FLTableMetaData * tMD = cursor_->metadata();
  FLFieldMetaData * fieldTMD = tMD->field( field->name() );
  if ( !fieldTMD )
    return;

  if ( !fieldTMD->visible() ) {
    QBrush bu;
    bu.setColor( gray );
    bu.setStyle( DiagCrossPattern );
    p->fillRect( 1, 1, cr.width() - 4, cr.height() - 4, bu );
    return;
  }

  if ( !field->isNull() ) {
    int type = fieldTMD->type();
    QString text;

    switch ( type ) {
      case QVariant::Double: {
        double fValue = field->value().toDouble();
        int partDecimal = fieldTMD->partDecimal();
        text.setNum( fValue, 'f', partDecimal );
        if ( fValue < 0.0 ) {
          QPen curPen = p->pen();
          QPen pen( curPen );
          pen.setColor( red );
          p->setPen( pen );
        }
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4, Qt::AlignRight,
                     FLUtil::formatoMiles( text ) );
      }
      break;

      case FLFieldMetaData::Unlock: {
        if ( field->value().toBool() )
          p->drawPixmap( ( cr.width() - ok.width() ) / 2, 2, ok, 0, 0, cr.width() - 4,
                         cr.height() - 4 );
        else
          p->drawPixmap( ( cr.width() - no.width() ) / 2, 2, no, 0, 0, cr.width() - 4,
                         cr.height() - 4 );
      }
      break;

      case QVariant::String: {
        text = field->value().toString();
        if ( fieldTMD->hasOptionsList() ) {
          QStringList ol( fieldTMD->optionsList() );
          if ( !ol.contains( text ) ) {
            QVariant defVal( fieldTMD->defaultValue() );
            if ( defVal.isValid() )
              text = defVal.toString();
            else
              text = ol.first();
          }
          text = FLUtil::translate( "MetaData", text );
        }
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4, fieldAlignment( field ), text );
      }
      break;

      case QVariant::Int:
        if ( field->value().toInt() < 0 ) {
          QPen curPen = p->pen();
          QPen pen( curPen );
          pen.setColor( red );
          p->setPen( pen );
        }
        text = field->value().toString();
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4, Qt::AlignRight,
                     FLUtil::formatoMiles( text ) );
        break;

      case FLFieldMetaData::Serial:
      case QVariant::UInt:
        text = field->value().toString();
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4, Qt::AlignRight,
                     FLUtil::formatoMiles( text ) );
        break;

      case QVariant::Pixmap: {
        QCString cs = cursor_->db()->manager()->fetchLargeValue( field->value().toString() ).toCString();
        if ( cs.isEmpty() )
          return ;

        QPixmap pix;

        if ( !QPixmapCache::find( cs.left( 100 ), pix ) ) {
          pix.loadFromData( cs );
          QPixmapCache::insert( cs.left( 100 ), pix );
        }
        if ( !pix.isNull() )
          p->drawPixmap( 2, 2, pix, 0, 0, cr.width() - 4,
                         cr.height() - 4 );
      }
      break;

      case QVariant::Date: {
        QDate d = field->value().toDate();

        text = d.toString( "dd-MM-yyyy" );
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4,
                     fieldAlignment( field ), text );
      }
      break;

      case QVariant::Time: {
        QTime t = field->value().toTime();

        text = t.toString( "hh:mm:ss" );
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4,
                     fieldAlignment( field ), text );
      }
      break;

      case QVariant::StringList:
        text = field->value().toString();
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4,
                     Qt::AlignAuto | Qt::AlignTop, text.left( 100 ) + "..." );
        break;

      case QVariant::Bool: {
        if ( fieldTMD->isCheck() ) {
          int row = rowAt( cr.y() ), col = columnAt( cr.x() );
          int curAt = cursor_->at();
          FLCheckBox * chk = ::qt_cast<FLCheckBox *>( cellWidget( row, col ) );
          if ( !chk ) {
            chk = new FLCheckBox( this );
            setCellWidget( row, col, chk );
          } else
            disconnect( chk, SIGNAL( toggled( bool ) ), this, SLOT( setChecked( bool ) ) );
          if ( cursor_->QSqlCursor::seek( row ) ) {
            chk->setChecked( primarysKeysChecked_.contains( cursor_->QSqlCursor::value( tMD->primaryKey() ) ) );
            cursor_->QSqlCursor::seek( curAt );
          }
          connect( chk, SIGNAL( toggled( bool ) ), this, SLOT( setChecked( bool ) ) );
        } else {
          if ( field->value().toBool() ) {
            QBrush bu;
            bu.setColor( green );
            bu.setStyle( SolidPattern );
            p->fillRect( 1, 1, cr.width() - 4, cr.height() - 4, bu );
          } else {
            QBrush bu;
            bu.setColor( red );
            bu.setStyle( SolidPattern );
            p->fillRect( 1, 1, cr.width() - 4, cr.height() - 4, bu );
          }
          QPen curPen = p->pen();
          QPen pen( curPen );
          pen.setColor( darkBlue );
          p->setPen( pen );
          text = field->value().toBool() ? tr( "Sí" ) : tr( "No" );
          p->drawText( 2, 2, cr.width() - 4, cr.height() - 4,
                       fieldAlignment( field ), text );
        }
      }
      break;
    }
    lastTextPainted = text;
  }
}

bool FLDataTable::eventFilter( QObject * o, QEvent * e ) {
  int r = currentRow(), c = currentColumn(), nr = numRows(), nc = numCols();

  switch ( e->type() ) {
    case QEvent::KeyPress: {
      QKeyEvent * ke = static_cast<QKeyEvent *>( e );

      if ( ke->key() == Key_Escape && popup_ && parentWidget() ) {
        parentWidget()->hide();
        return true;
      }

      if ( ke->key() == Key_Insert )
        return true;

      if ( ke->key() == Key_F2 )
        return true;

      if ( ke->key() == Key_Up && r == 0 )
        return true;

      if ( ke->key() == Key_Left && c == 0 )
        return true;

      if ( ke->key() == Key_Down && r == nr - 1 )
        return true;

      if ( ke->key() == Key_Right && c == nc - 1 )
        return true;

      if ( ( ke->key() == Key_Enter || ke->key() == Key_Return ) && r > -1 ) {
        emit recordChoosed();
        return true;
      }

      if ( ke->key() == Key_A && !popup_ )
        if ( cursor_ && !readonly_ && !editonly_ ) {
          cursor_->insertRecord();
          return true;
        } else
          return false;

      if ( ke->key() == Key_C && !popup_ )
        if ( cursor_ && !readonly_ && !editonly_ ) {
          cursor_->copyRecord();
          return true;
        } else
          return false;

      if ( ke->key() == Key_M && !popup_ )
        if ( insertonly_ )
          return false;
        else
          if ( cursor_ && !readonly_ ) {
            cursor_->editRecord();
            return true;
          } else
            return false;

      if ( ke->key() == Key_E || ke->key() == Key_Delete && !popup_ )
        if ( insertonly_ )
          return false;
        else
          if ( cursor_ && !readonly_ && !editonly_ ) {
            cursor_->deleteRecord();
            return true;
          } else
            return false;

      if ( ke->key() == Key_V && !popup_ )
        if ( cursor_ ) {
          cursor_->browseRecord();
          return true;
        }
      return false;
    }
    break;
  }
  return QDataTable::eventFilter( o, e );
}

void FLDataTable::contentsContextMenuEvent( QContextMenuEvent * e ) {
  QTable::contentsContextMenuEvent( e );
}

void FLDataTable::contentsMouseDoubleClickEvent( QMouseEvent * e ) {
  if ( e->button() != LeftButton )
    return ;

  int tmpRow = rowAt( e->pos().y() );
  int tmpCol = columnAt( e->pos().x() );
  QTableItem *itm = item( tmpRow, tmpCol );

  if ( itm && !itm->isEnabled() )
    return ;

  emit doubleClicked( tmpRow, tmpCol, e->button(), e->pos() );
  emit recordChoosed();
}

void FLDataTable::refresh() {
  if ( popup_ )
    QDataTable::refresh();
  if ( !refreshing_ && cursor_  && cursor_->metadata() ) {
    refreshing_ = true;
    cursor_->setFilter( persistentFilter_ );
    FLSqlCursor * sndCursor = ::qt_cast<FLSqlCursor *>( sender() );
    if ( sndCursor ) {
      setFilter( cursor_->curFilter() );
      QDataTable::refresh();
      cursor_->QSqlCursor::seek( cursor_->atFrom() );
      selectRow();
    } else {
      setFilter( cursor_->curFilter() );
      QDataTable::refresh();
      selectRow();
    }
  }
  refreshing_ = false;
}

void FLDataTable::setFocus() {
  if ( !cursor_ )
    return ;
  if ( !hasFocus() ) {
    setPaletteBackgroundColor( qApp->palette().color( QPalette::Active, QColorGroup::Base ) );
#ifndef FL_QUICK_CLIENT
    cursor_->QSqlCursor::select();
#endif
  } else
    if ( numRows() != cursor_->size() )
      setNumRows( cursor_->size() );
  QWidget::setFocus();
}

void FLDataTable::setQuickFocus() {
  setPaletteBackgroundColor( qApp->palette().color( QPalette::Active, QColorGroup::Base ) );
  QWidget::setFocus();
}

void FLDataTable::focusOutEvent( QFocusEvent * ) {
  setPaletteBackgroundColor( qApp->palette().color( QPalette::Active, QColorGroup::Background ) );
}

void FLDataTable::setFLReadOnly( const bool mode ) {
  if ( !cursor_ )
    return ;
  cursor_->setEdition( !mode );
  readonly_ = mode;
}

void FLDataTable::setEditOnly( const bool mode ) {
  editonly_ = mode;
}

void FLDataTable::setInsertOnly( const bool mode ) {
  if ( !cursor_ )
    return ;
  cursor_->setEdition( !mode );
  insertonly_ = mode;
}

void FLDataTable::ensureRowSelectedVisible() {
  if ( rowSelected > -1 ) {
    if ( !isUpdatesEnabled() || !viewport()->isUpdatesEnabled() )
      return;
    int cw = columnWidth( colSelected );
    int margin = visibleHeight() / 2;
    int y = rowPos( rowSelected ) + rowHeight( rowSelected ) / 2;
    if ( cw < visibleWidth() )
      ensureVisible( columnPos( colSelected ) + cw / 2, y, cw / 2, margin );
    else
      ensureVisible( columnPos( colSelected ), y, 0, margin );
  }
}

void FLDataTable::setChecked( bool on ) {
  FLCheckBox * chk = ::qt_cast<FLCheckBox *>( sender() );

  if ( !chk || !cursor_ || !cursor_->metadata() )
    return;

  int curAt = cursor_->at();
  int posAt = rowAt( viewportToContents( chk->pos() ).y() );

  if ( cursor_->QSqlCursor::seek( posAt ) ) {
    QVariant primaryKeyValue( cursor_->QSqlCursor::value( cursor_->metadata()->primaryKey() ) );
    setPrimaryKeyChecked( primaryKeyValue, on );
    cursor_->QSqlCursor::seek( curAt );
  }
}

const QValueList<QVariant> FLDataTable::primarysKeysChecked() const {
  return primarysKeysChecked_;
}

void FLDataTable::clearChecked() {
  primarysKeysChecked_.clear();
}

void FLDataTable::setPrimaryKeyChecked( const QVariant & primaryKeyValue, bool on ) {
  if ( on ) {
    if ( !primarysKeysChecked_.contains( primaryKeyValue ) ) {
      primarysKeysChecked_.append( primaryKeyValue );
      emit primaryKeyToggled( primaryKeyValue, true );
    }
  } else {
    if ( primarysKeysChecked_.contains( primaryKeyValue ) ) {
      primarysKeysChecked_.remove( primaryKeyValue );
      emit primaryKeyToggled( primaryKeyValue, false );
    }
  }
}

void FLDataTable::setPersistentFilter( const QString & pFilter ) {
  persistentFilter_ = pFilter;
}
