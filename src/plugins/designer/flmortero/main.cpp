/***************************************************************************
                          main.cpp  -  description
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

#include <qwidgetplugin.h>

#include "flmortero.h"

QString FLMortero::patchtype() const {
  return patchtype_;
}

QString FLMortero::patchto() const {
  return patchto_;
}

QString FLMortero::patchxpath() const {
  return patchxpath_;
}

void FLMortero::setPatchtype( const QString & p ) {
  patchtype_ = p;
}

void FLMortero::setPatchto( const QString & p ) {
  patchto_ = p;
}

void FLMortero::sePatchxpath( const QString & p ) {
  patchxpath_ = p;
}

// Uso interno
class FLMorteroWidgetInterface: public QWidgetPlugin {

public:

  FLMorteroWidgetInterface();

  QStringList keys() const;
  QWidget *create( const QString & classname, QWidget * parent = 0, const char *name = 0 );
  QString group( const QString & ) const;
  QIconSet iconSet( const QString & ) const;
  QString includeFile( const QString & ) const;
  QString toolTip( const QString & ) const;
  QString whatsThis( const QString & ) const;
  bool isContainer( const QString & ) const;
};

FLMorteroWidgetInterface::FLMorteroWidgetInterface() {}

QStringList FLMorteroWidgetInterface::keys() const {
  QStringList list;
  list << "FLMortero";
  return list;
}

QWidget *FLMorteroWidgetInterface::create( const QString & classname, QWidget * parent, const char * name ) {
  QWidget * w = 0;
  if ( classname == "FLMortero" ) {
    w = new FLMortero( parent, name );
  }
  return w;
}

QString FLMorteroWidgetInterface::group( const QString & description ) const {
  if ( description == "FLMortero" )
    return "Display";
  return QString::null;
}

QIconSet FLMorteroWidgetInterface::iconSet( const QString & ) const {
  return QIconSet( QPixmap::fromMimeSource( "mortero.png" ) );
}

QString FLMorteroWidgetInterface::includeFile( const QString & description ) const {
  return QString::null;
}

QString FLMorteroWidgetInterface::toolTip( const QString & description ) const {
  return QString::null;
}

QString FLMorteroWidgetInterface::whatsThis( const QString & description ) const {
  return QString::null;
}

bool FLMorteroWidgetInterface::isContainer( const QString & ) const {
  return FALSE;
}

Q_EXPORT_PLUGIN( FLMorteroWidgetInterface )
