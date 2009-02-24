/***************************************************************************
                      FLReportEngine.cpp  -  description
                         -------------------
begin                : jue jun 27 2002
copyright            : (C) 2002-2004 by InfoSiAL S.L.
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

#include "FLReportEngine.h"
#include "FLSqlCursor.h"
#include "FLSqlQuery.h"
#include "FLGroupByQuery.h"
#include "FLManagerModules.h"
#include "FLTableMetaData.h"
#include "FLFieldMetaData.h"
#include "FLManager.h"
#include "FLSqlDatabase.h"
#include "FLSqlConnections.h"

#include <qdir.h>
#include <qfile.h>

FLReportEngine::FLReportEngine( QObject * parent ) : MReportEngine( parent ), rows( 0 ), qry_( 0 ) {}

FLReportEngine::~FLReportEngine() {
  if ( rows )
    delete rows;
}

void FLReportEngine::addRowToReportData( FLSqlQuery * q, const int l ) {
  if ( !q || !q->isValid() )
    return ;

  QDomElement * row = new QDomElement( rd->createElement( "Row" ) );

  row->setAttribute( "level", l );
  int i = 0;

  QStringList fL = q->fieldList();
  for ( QStringList::Iterator it = fL.begin(); it != fL.end(); ++it, i++ ) {
    row->setAttribute(( *it ), q->value( i ).toString() );
  }

  rows->appendChild( *row );

  delete row;

  qry_ = q;
}

void FLReportEngine::groupBy( int levelMax, FLSqlQuery * q, QString vA[ FL_MAXGROUPLEVEL ] ) {
  int l, i;
  QString v;

  if ( !q )
    return ;

  if ( !q->isValid() )
    return ;

  FLSqlQuery::FLGroupByQueryDict * g = q->groupDict();

  for ( l = 0; l < levelMax && vA[ l ] == q->value(( *g )[ QString::number( l )] ->field() ).toString(); l++ )
    v = q->value(( *g )[ QString::number( l )] ->field() ).toString();

  for ( i = l; i < levelMax; i++ ) {
    addRowToReportData( q, i );
    v = q->value(( *g )[ QString::number( i )] ->field() ).toString();
    vA[ i ] = v;
  }
  addRowToReportData( q, levelMax );

  if ( q->next() )
    groupBy( levelMax, q, vA );
}

bool FLReportEngine::setReportData( FLSqlQuery * q ) {
  QString vA[ FL_MAXGROUPLEVEL ];

  if ( !q )
    return false;

  rows = new QDomDocumentFragment( rd->createDocumentFragment() );

  q->setForwardOnly( true );

  if ( !q->exec() ) {
#ifdef FL_DEBUG
    qWarning( "FLReportEngine : " +  tr( " No se ha podido ejecutar la consulta del informe" ) );
#endif
    return false;
  }

  if ( !q->next() ) {
#ifdef FL_DEBUG
    qWarning( "FLReportEngine : " +  tr( " Consulta del informe vacía" ) );
#endif
    return false;
  }

  FLSqlQuery::FLGroupByQueryDict * g = q->groupDict();

  if ( !g ) {
    do {
      addRowToReportData( q, 0 );
      qApp->processEvents();
    } while ( q->next() );
  } else {
    for ( int i = 0; i < FL_MAXGROUPLEVEL; i++ )
      vA[ i ] = QString::null;
    groupBy( g->count(), q, vA );
  }

  QDomElement * data = new QDomElement( rd->createElement( "KugarData" ) );
  data->appendChild( *rows );
  rd->appendChild( *data );

  delete data;
  delete rows;
  rows = 0;

  initData();

  /*QString file = QString( FL_HOME ) + "/rptengineout.txt";
  QFile fi( file );
  if ( fi.open( IO_WriteOnly ) ) {
   QTextStream t( &fi );
   rd.save( t, 4 );
   fi.close();
  }*/

  return true;
}

bool FLReportEngine::setReportData( FLSqlCursor * t ) {
  t = 0;
#ifdef FL_DEBUG
  qWarning( "No implementadio todavía" );
#endif

  return false;
}

bool FLReportEngine::setFLReportData( QDomNode d ) {
  MReportEngine::setReportData( d );
  return true;
}

bool FLReportEngine::setFLReportTemplate( const QString & t ) {
  if ( !qry_ )
    return setReportTemplate( FLSqlConnections::database()->managerModules()->contentCached( t + ".kut" ) );
  else
    return setReportTemplate( qry_->db()->managerModules()->contentCached( t + ".kut" ) );
}
