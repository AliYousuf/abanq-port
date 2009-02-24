/***************************************************************************
                              FLVar.cpp
                         -------------------
begin                : Thu Jul 29 2004
copyright            : (C) 2004-2005 by InfoSiAL S.L.,
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

#include <math.h>

#include "FLUtil.h"
#include "FLVar.h"
#include "FLApplication.h"
#include "FLSqlQuery.h"

FLVar::FLVar() {}

FLVar::~FLVar() {}

bool FLVar::set( const QString & n, const QVariant & v ) {
  QString idSesion = static_cast<FLApplication *>( qApp ) ->timeUser().toString( Qt::ISODate );
  QString where = "idvar = '" + n + "' AND idsesion = '" + idSesion + "'";

  FLSqlQuery q;
  q.setTablesList( "flvar" );
  q.setSelect( "idvar" );
  q.setFrom( "flvar" );
  q.setWhere( where );
  q.setForwardOnly( true );
  if ( !q.exec() )
    return false;

  if ( q.next() )
    return FLUtil::sqlUpdate( "flvar", "valor", v.toString(), where );
  else {
    QString values = n + "," + idSesion + "," + v.toString();
    return FLUtil::sqlInsert( "flvar", "idvar, idsesion, valor", values );
  }
}

QVariant FLVar::get( const QString & n, bool * exists ) {
  QString where = "idvar = '" + n + "' AND idsesion = '" + static_cast<FLApplication *>( qApp ) ->timeUser().toString( Qt::ISODate ) + "'";
  int size;
  QVariant result = FLUtil::sqlSelect( "flvar", "valor", where, "flvar", &size );
  if ( exists )
    *exists = !!size;
  return result;
}

bool FLVar::del( const QString & n ) {
  QString where = "idvar = '" + n + "' AND idsesion = '" + static_cast<FLApplication *>( qApp ) ->timeUser().toString( Qt::ISODate ) + "'";
  return FLUtil::sqlDelete( "flvar", where );
}

bool FLVar::clean() {
  QString where = "idsesion = '" + static_cast<FLApplication *>( qApp ) ->timeUser().toString( Qt::ISODate ) + "'";
  return FLUtil::sqlDelete( "flvar", where );
}
