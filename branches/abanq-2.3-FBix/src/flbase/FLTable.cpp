/***************************************************************************
                           FLTable.cpp
                         -------------------
begin                : jue abr 23 2009
copyright            : (C) 2001-2009 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
   versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLTable.h"

FLTable::FLTable( QWidget * parent, const char * name ) : QTable( parent, name ) {
}
void FLTable::paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg ) {
  if ( !selected ) {
    QString key( QString( "%1,%2" ).arg( row ).arg( col ) );

    if ( cellBgColors_.contains( key ) ) {
      QTableItem * itm = item( row, col );

      if ( itm ) {
        QTable::paintCell( p, row, col, cr, selected, cg );

        QBrush bu;
        bu.setColor( cellBgColors_[ key ] );
        bu.setStyle( SolidPattern );

        p->setPen( selected ? cg.highlightedText() : cg.text() );
        p->fillRect( 1, 1, cr.width() - 2, cr.height() - 2, bu );
        p->drawText( 2, 2, cr.width() - 4, cr.height() - 4,
                     itm->wordWrap() ? ( itm->alignment() | Qt::WordBreak ) : itm->alignment(), itm->text() );

        return;
      }
    }
  }

  QTable::paintCell( p, row, col, cr, selected, cg );
}

void FLTable::setCellBackgroundColor( int row, int col, const QColor & color ) {
  QString key( QString( "%1,%2" ).arg( row ).arg( col ) );
  cellBgColors_[ key ] = color;
}
