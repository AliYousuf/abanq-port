/***************************************************************************
                          FLCompoundKey.cpp
                         -------------------
begin                : Fri Feb 1 2002
copyright            : (C) 2002-2005 by InfoSiAL S.L.
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

#include "FLCompoundKey.h"
#include "FLFieldMetaData.h"

FLCompoundKey::FLCompoundKey() : fieldList_( 0 ) {
  fieldList_ = new FLTableMetaData::FLFieldMetaDataList();
}

FLCompoundKey::~FLCompoundKey() {
  if ( fieldList_ )
    delete fieldList_;
}

void FLCompoundKey::addFieldMD( const FLFieldMetaData * f ) {
  if ( fieldList_ )
    fieldList_->insert( f->name().lower(), f );
}

bool FLCompoundKey::hasField( const QString & fN ) const {
  return ( fieldList_ && fieldList_->find( fN.lower() ) );
}
