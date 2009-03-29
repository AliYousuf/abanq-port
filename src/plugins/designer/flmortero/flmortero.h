/***************************************************************************
                          flmortero.h  -  description
                             -------------------
    begin                : mie nov 6 2008
    copyright            : (C) 2003-2008 by InfoSiAL S.L.
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

#include <qwidget.h>

// Uso interno
class FLMortero : public QWidget {

  Q_OBJECT

  Q_PROPERTY( QString patchtype READ patchtype WRITE setPatchtype )
  Q_PROPERTY( QString patchto READ patchto WRITE setPatchto )
  Q_PROPERTY( QString patchxpath READ patchxpath WRITE setPatchxpath )

public:

  FLMortero( QWidget * parent = 0 , const char * name = 0 )
      : QWidget( parent, name ) {}

  QString patchtype() const;
  QString patchto() const;
  QString patchxpath() const;

  void setPatchtype( const QString & p );
  void setPatchto( const QString & p );
  void sePatchxpath( const QString & p );

private:

  QString patchtype_;
  QString patchto_;
  QString patchxpath_;
};
