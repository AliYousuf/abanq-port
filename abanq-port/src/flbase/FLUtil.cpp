/***************************************************************************
                             FLUtil.cpp
                         -------------------
begin                : Sun Jan 27 2002
copyright            : (C) 2002-2004 by InfoSiAL S.L.,
                       Eloy Ordu�a Gonz�lez, Andr�s Ot�n
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

#include <math.h>

#include "FLUtil.h"
#include "FLSqlCursor.h"
#include "FLSqlQuery.h"
#include "FLFieldMetaData.h"
#include "FLManager.h"
#include "FLApplication.h"
#include "FLSha1.h"
#include "FLManagerModules.h"
#include "FLSqlDatabase.h"
#include "FLSqlConnections.h"

FL_EXPORT QDict<QProgressDialog> FLUtil::dictProgressD_;
FL_EXPORT const char *FLUtil::vecUnidades[ 30 ] = { "", "uno", "dos", "tres", "cuatro",
    "cinco", "seis", "siete", "ocho", "nueve",
    "diez", "once", "doce", "trece", "catorce",
    "quince", "dieciseis", "diecisiete", "dieciocho", "diecinueve",
    "veinte", "veintiun", "veintidos", "veintitres", "veinticuatro",
    "veinticinco", "veintiseis", "veintisiete", "veintiocho", "veintinueve"
                                                  };
FL_EXPORT const char *FLUtil::vecDecenas[ 10 ] = { "", "", "", "treinta", "cuarenta", "cincuenta",
    "sesenta", "setenta", "ochenta", "noventa"
                                                 };
FL_EXPORT const char *FLUtil::vecCentenas[ 10 ] = { "", "ciento", "doscientos", "trescientos", "cuatrocientos",
    "quinientos", "seiscientos", "setecientos", "ochocientos", "novecientos"
                                                  };
Q_ULLONG FLUtil::partInteger( double n ) {
  return ( Q_ULLONG ) n;
}

Q_ULLONG FLUtil::partDecimal( double n ) {
  QString s = QString::number( n, 'f', 2 );
  s = s.right( 2 );
  return s.toULong();
}

QString FLUtil::enLetra( long n ) {
  QString buffer;

  if ( n > 1000000000L ) {
    buffer = "S�lo hay capacidad hasta mil millones";
    return buffer;
  }

  if ( n < 1000000L ) {
    buffer = centenamillar( n );
    return buffer;
  } else {
    if ( n / 1000000L == 1 )
      buffer = "un millon ";
    else {
      buffer = centenas(( long )( n / 1000000L ) );
      buffer = buffer + " millones ";
    }
  }

  buffer = buffer + centenamillar( n % 1000000L );

  return buffer.upper();
}

QString FLUtil::enLetraMoneda( double n, const QString & m ) {

  double nTemp = ( n < 0.00 ) ? ( n * -1.00 ) : n;
  long entero = partInteger( nTemp ), decimal = partDecimal( nTemp );
  QString res = QString::null;

  if ( entero > 0 )
    res = enLetra( entero ) + " " + m;

  if ( entero > 0 && decimal > 0 )
    res += " con " + enLetra( decimal ) + " c�ntimos";

  if ( entero <= 0 && decimal > 0 )
    res = enLetra( decimal ) + " c�ntimos";

  if ( n < 0.00 )
    res = "menos " + res;

  return res.upper();
}

QString FLUtil::enLetraMonedaEuro( double n ) {
  return enLetraMoneda( n, "euros" );
}

QString FLUtil::unidades( long n ) {
  return vecUnidades[ n ];
}

QCString FLUtil::utf8( const QString & s ) {
  return s.utf8();
}

QString FLUtil::centenamillar( long n ) {
  QString buffer;

  if ( n < 10000 ) {
    buffer = decenasmillar( n );
    return buffer;
  }
  buffer = centenas( n / 1000 );
  buffer = buffer + " mil ";
  buffer = buffer + centenas( n % 1000 );

  return buffer;
}

QString FLUtil::decenas( long n ) {
  QString buffer;
  if ( n < 30 )
    buffer = unidades( n );
  else {
    buffer = vecDecenas[ n / 10L ];
    if ( n % 10L ) {
      buffer = buffer + " y ";
      buffer = buffer + unidades( n % 10L );
    }
  }
  return buffer;
}

QString FLUtil::centenas( long n ) {
  QString buffer;
  if ( n == 100 )
    buffer = "cien";
  else if ( n < 100 )
    buffer = decenas( n );
  else {
    buffer = buffer + vecCentenas[ n / 100L ];
    buffer = buffer + " ";
    buffer = buffer + decenas( n % 100 );
  }
  return buffer;
}


QString FLUtil::unidadesmillar( long n ) {
  QString buffer;

  if ( n < 1000 )
    buffer = "";

  if ( n / 1000 == 1 )
    buffer = "mil ";

  if ( n / 1000 > 1 ) {
    buffer = unidades( n / 1000 );
    buffer = buffer + " mil ";
  }
  buffer = buffer + centenas( n % 1000 );

  return buffer;
}

QString FLUtil::decenasmillar( long n ) {
  QString buffer;

  if ( n < 10000 ) {
    buffer = unidadesmillar( n );
    return buffer;
  }

  buffer = decenas( n / 1000 );
  buffer = buffer + " mil ";
  buffer = buffer + centenas( n % 10000 );

  return ( buffer );
}

QStringList FLUtil::nombreCampos( const QString & tabla ) {
  QStringList res;

  if ( FLSqlConnections::database()->managerModules()->shaOfFile( tabla + ".mtd" ).isEmpty() )
    return res;

  FLSqlCursor c( tabla );
  FLTableMetaData * tmd = c.metadata();
  if ( !tmd )
    return res;

  res = QStringList::split( ",", tmd->fieldsNames() );
  res.prepend( QString::number( res.size() ) );

  return res;
}

QChar FLUtil::letraDni( long n ) {
  QString letras = "TRWAGMYFPDXBNJZSQVHLCKE";

  return letras.at( n % 23 );
}

QChar FLUtil::calcularDC( const QString & n ) {
  int Tabla[ 10 ] = {
    6,
    3,
    7,
    9,
    10,
    5,
    8,
    4,
    2,
    1
  };
  int DC, Suma = 0, nDigitos;

  nDigitos = n.length() - 1;
  for ( uint ct = 1; ct <= n.length(); ct++ ) {
    Suma += Tabla[ nDigitos ] * ( n[( int ) ct - 1 ] - '0' );
    nDigitos--;
  }

  DC = 11 - ( Suma % 11 );
  if ( DC == 11 )
    DC = 0;
  else if ( DC == 10 )
    DC = 1;

  return QChar::QChar( DC + 48 );
}

QString FLUtil::dateDMAtoAMD( const QString & f ) {
  QString res = f;

  res = res.replace( QRegExp( "[\\s/-]" ), "" );

  if ( res.length() <= 2 )
    return ( res.left( 2 ) );

  if ( res.length() <= 4 )
    return ( res.right( res.length() - 2 ) + "-" + res.left( 2 ) );

  if ( res.length() <= 8 )
    return ( res.right( res.length() - 4 ) + "-" + res.mid( 2, 2 ) + "-" +
             res.left( 2 ) );

  return res;
}

QString FLUtil::dateAMDtoDMA( const QString & f ) {
  QString res = f;

  res = res.replace( QRegExp( "[\\s/-]" ), "" );

  if ( res.length() > 8 )
    res = res.left( 8 );

  if ( res.length() <= 2 )
    return ( res.left( 2 ) );

  if ( res.length() <= 4 )
    return ( res.right( res.length() - 2 ) + "-" + res.left( 2 ) );

  if ( res.length() <= 8 )
    return ( res.right( res.length() - 6 ) + "-" + res.mid( 4, 2 ) + "-" + res.left( 4 ) );

  return res;
}

QString FLUtil::formatoMiles( const QString & s ) {
  QString ret;
  QString decimal;
  QString entera;
  QString dot = QApplication::tr( "." );
  bool neg = ( s.at( 0 ) == '-' );

  if ( s.contains( '.' ) ) {
    decimal = QApplication::tr( "," ) + s.section( '.', -1, -1 );
    entera = s.section( '.', -2, -2 ).remove( '.' );
  } else
    entera = s;

  if ( neg )
    entera.remove( 0, 1 );

  int len = entera.length();

  while ( len > 3 ) {
    ret = dot + entera.right( 3 ) + ret;
    entera.remove( len - 3, 3 );
    len = entera.length();
  }

  ret = entera + ret + decimal;

  if ( neg )
    ret.prepend( '-' );

  return ret;
}

QString FLUtil::translate( const QString & contexto, const QString & s ) {
  return qApp->translate( contexto, s );
}

bool FLUtil::numCreditCard( const QString & num ) {
  int nSum = 0;
  int nRest, nCard;

  nCard = num.toInt();
  nRest = nCard;

  for ( int i = 0; i < 10; i += 2 ) {
    nSum = nSum + ( num.mid( i, 1 ) ).toInt();
    nRest = ( num.mid( i + 1, 1 ) ).toInt() * 2;
    if ( nRest > 9 ) {
      nRest = nRest - 9;
    }
    nSum = nSum + nRest;
  }

  if ( nSum % 10 == 0 )
    return true;
  else
    return false;

}

QVariant FLUtil::nextCounter( const QString & name, FLSqlCursor * cursor_ ) {
  if ( !cursor_ )
    return QVariant();

  FLTableMetaData *tMD = cursor_->metadata();

  if ( !tMD )
    return QVariant();

  FLFieldMetaData *field = tMD->field( name );

  if ( !field )
    return QVariant();

  int type = field->type();

  if ( type != QVariant::String && type != QVariant::Double )
    return QVariant();

  unsigned int len = field->length();
  QString cadena;

  FLSqlQuery q( 0, cursor_->db()->connectionName() );
  q.setForwardOnly( true );
  q.setTablesList( tMD->name() );
  q.setSelect( name );
  q.setFrom( tMD->name() );
  q.setWhere( "LENGTH(" + name + ")=" + QString::number( len ) );
  q.setOrderBy( name + " DESC" );

  if ( !q.exec() )
    return QVariant();

  double maxRange = pow( 10, len );
  double numero = maxRange;

  while ( numero >= maxRange ) {
    if ( !q.next() ) {
      numero = 1;
      break;
    }
    numero = q.value( 0 ).toDouble();
    numero++;
  }

  if ( type == QVariant::String ) {
    cadena = QString::number( numero, 'f', 0 );
    if ( cadena.length() < len ) {
      QString str;
      str.fill( '0', ( len - cadena.length() ) );
      cadena = str + cadena;
    }
    return QVariant( cadena );
  }

  if ( type == QVariant::Double )
    return QVariant( numero );

  return QVariant();
}

QString FLUtil::nextSequence( int nivel, const QString & secuencia, const QString & ultimo ) {
  QString cadena;
  QString valor;
  QString string;
  QString string2;

  QChar temp;
  long num;
  int pos2 = 0;
  int nivpas = 0;
  int posult = 0;

  int niveles = secuencia.contains( "%A" ) + secuencia.contains( "%N" );
  int pos = 1;
  pos2 = secuencia.find( "%", 0 );

  while ( pos2 != -1 ) {
    if ( nivpas == 0 ) {
      cadena = secuencia.mid( 0, pos2 );
      posult = pos2;
    } else {
      string2 = secuencia.mid( pos + 2, pos2 - pos - 2 );
      posult = ultimo.find( string2, posult ) + string2.length();
      cadena = cadena + secuencia.mid( pos + 2, pos2 - pos - 2 );
    }

    pos = pos2;
    valor = "";
    if ( nivpas < niveles - nivel ) {
      for ( uint i = posult; i < ultimo.length(); ++i ) {
        if ( secuencia.mid( pos + 1, 1 ) == "A" ) {
          temp = ultimo.at( i );
          if ( temp.isLetter() == false ) {
            cadena = cadena + valor;
            break;
          }
          valor = valor + temp;
        }
        if ( secuencia.mid( pos + 1, 1 ) == "N" ) {
          temp = ultimo.at( i );
          if ( temp.isNumber() == false ) {
            cadena = cadena + valor;
            break;
          }
          valor = valor + temp;
        }
      }
      posult = posult + valor.length();
    } else if ( nivpas == niveles - nivel ) {
      for ( uint i = posult; i < ultimo.length(); ++i ) {
        if ( secuencia.mid( pos + 1, 1 ) == "A" ) {
          temp = ultimo.at( i );
          if ( i == ( ultimo.length() - 1 ) ) {
            valor = valor + temp;
            num = serialLettertoNumber( valor ).toLong() + 1;
            cadena = cadena + serialNumbertoLetter( num );
            break;
          }
          if ( temp.isLetter() == false ) {
            num = serialLettertoNumber( valor ).toLong() + 1;
            cadena = cadena + serialNumbertoLetter( num );
            break;
          }
          valor = valor + temp;
        }
        if ( secuencia.mid( pos + 1, 1 ) == "N" ) {
          temp = ultimo.at( i );
          if ( i == ( ultimo.length() - 1 ) ) {
            valor = valor + temp;
            cadena = cadena + string.setNum( valor.toInt() + 1 );
            break;
          }
          if ( temp.isNumber() == false ) {
            cadena = cadena + string.setNum( valor.toInt() + 1 );
            break;
          }
          valor = valor + temp;
        }
      }
      posult = posult + valor.length();
    } else {
      if ( secuencia.mid( pos + 1, 1 ) == "N" )
        cadena = cadena + "1";
      if ( secuencia.mid( pos + 1, 1 ) == "A" )
        cadena = cadena + "A";
    }
    pos2 = secuencia.find( "%", pos + 1 );
    nivpas++;
  }
  return cadena;
}

QString FLUtil::serialLettertoNumber( const QString & letter ) {
  int len = letter.length(), num = 0;
  QString string;

  for ( int i = 1; i <= len; ++i )
    num += ((( int ) pow( 26, i - 1 ) ) * ( letter.at( len - i ).unicode() - 64 ) );

  return string.setNum( num );
}

QString FLUtil::serialNumbertoLetter( int number ) {
  QString letter;
  int cocciente = number, resto = 0;

  while ( cocciente > 26 ) {
    cocciente = number / 26;
    resto = number % 26;
    letter = QString( QChar( resto + 64 ) ) + letter;
    number = cocciente;
  }
  letter = QString( QChar( cocciente + 64 ) ) + letter;

  return letter;
}

bool FLUtil::isFLDefFile( const QString & head ) {
  if ( head.find( "<!DOCTYPE UI>", 0, true ) != -1 )
    return true;

  if ( head.find( "<!DOCTYPE QRY>", 0, true ) != -1 )
    return true;

  if ( head.find( "<!DOCTYPE KugarTemplate", 0, true ) != -1 )
    return true;

  if ( head.find( "<!DOCTYPE TMD>", 0, true ) != -1 )
    return true;

  if ( head.find( "<!DOCTYPE TS>", 0, true ) != -1 )
    return true;

  if ( head.find( "<ACTIONS>", 0, true ) != -1 )
    return true;

  return false;
}

QDate FLUtil::addDays( const QDate &d, int nd ) {
  return d.addDays( nd );
}

QDate FLUtil::addMonths( const QDate &d, int nm ) {
  return d.addMonths( nm );
}

QDate FLUtil::addYears( const QDate &d, int ny ) {
  return d.addYears( ny );
}

int FLUtil::daysTo( const QDate &d1, const QDate &d2 ) {
  return d1.daysTo( d2 );
}

QVariant FLUtil::sqlSelect( const QString & f, const QString & s,
                            const QString & w, const QString & tL, int * size, const QString & connName ) {
  FLSqlQuery q_( 0, connName );

  if ( size )
    *size = 0;

  if ( !tL.isEmpty() )
    q_.setTablesList( tL );
  else
    q_.setTablesList( f );
  q_.setSelect( s );
  q_.setFrom( f );
  q_.setWhere( w );
  q_.setForwardOnly( true );
  if ( !q_.exec() ) {
    if ( size )
      *size = -1;
    return QVariant( false, 0 );
  }
  if ( q_.next() ) {
    if ( size )
      *size = q_.size();
    return q_.value( 0 );
  }

  return QVariant( false, 0 );
}

bool FLUtil::sqlInsert( const QString & t, const QString & fL, const QString & vL, const QString & connName ) {
  int fN = fL.contains( ',' ), vN = vL.contains( ',' );

  if ( fN != vN )
    return false;

  ++fN;
  QString fL2( fL ), v, f;
  fL2.remove( " " );

  FLSqlCursor c( t, true, connName );
  c.setModeAccess( FLSqlCursor::INSERT );
  c.refreshBuffer();
  for ( int i = 0; i < fN; ++i ) {
    v = vL.section( ',', i, i );
    f = fL2.section( ',', i, i );
    if ( v == "NULL" )
      c.bufferSetNull( f );
    else
      c.setValueBuffer( f, v );
  }

  return c.commitBuffer();
}

bool FLUtil::sqlUpdate( const QString & t, const QString & fL,
                        const QString & vL, const QString & w, const QString & connName ) {
  int fN = fL.contains( ',' ), vN = vL.contains( ',' );

  if ( fN != vN )
    return false;

  FLSqlCursor c( t, true, connName );
  c.setForwardOnly( true );

  if ( !c.select( w ) )
    return false;

  ++fN;
  QString fL2( fL ), v, f;
  fL2.remove( " " );

  while ( c.next() ) {
    c.setModeAccess( FLSqlCursor::EDIT );
    c.refreshBuffer();
    for ( int i = 0; i < fN; ++i ) {
      v = vL.section( ',', i, i );
      f = fL2.section( ',', i, i );
      if ( v == "NULL" )
        c.bufferSetNull( f );
      else
        c.setValueBuffer( f, v );
    }
    if ( !c.commitBuffer() )
      return false;
  }

  return true;
}

bool FLUtil::sqlDelete( const QString & t, const QString & w, const QString & connName ) {
  FLSqlCursor c( t, true, connName );
  c.setForwardOnly( true );

  if ( !c.select( w ) )
    return false;

  while ( c.next() ) {
    c.setModeAccess( FLSqlCursor::DEL );
    c.refreshBuffer();
    if ( !c.commitBuffer() )
      return false;
  }

  return true;
}

void FLUtil::createProgressDialog( const QString & l, int tS, const QString & id ) {
  QProgressDialog * pd = dictProgressD_.find( id );
  if ( !pd ) {
    pd = new QProgressDialog( l, 0, tS, 0, 0, true );
    dictProgressD_.insert( id, pd );
    pd->setCaption( "AbanQ" );
  }
}

void FLUtil::destroyProgressDialog( const QString & id ) {
  QProgressDialog * pd = dictProgressD_.take( id );
  if ( pd )
    pd->deleteLater();
}

void FLUtil::setProgress( int p, const QString & id ) {
  QProgressDialog * pd = dictProgressD_.find( id );
  if ( pd ) {
    pd->setProgress( p );
    qApp->processEvents();
  }
}

void FLUtil::setLabelText( const QString & l, const QString & id ) {
  QProgressDialog * pd = dictProgressD_.find( id );
  if ( pd )
    pd->setLabelText( l );
}

void FLUtil::setTotalSteps( int tS, const QString & id ) {
  QProgressDialog * pd = dictProgressD_.find( id );
  if ( pd )
    pd->setTotalSteps( tS );
}

bool FLUtil::domDocumentSetContent( QDomDocument & doc, const QString & content ) {
  if ( content.isEmpty() ) {
#ifdef FL_DEBUG
    qWarning( "FLUtil : " + QApplication::tr( "Se ha intentado cargar un fichero XML vac�o" ) );
#endif
    return false;
  }
  QString ErrMsg;
  int errLine, errColumn;
  if ( !doc.setContent( content, &ErrMsg, &errLine, &errColumn ) ) {
#ifdef FL_DEBUG
    qWarning( "FLUtil : " + QApplication::tr( "Error en fichero XML.\nError : %1\nLinea : %2\nColumna : %3" ).
              arg( ErrMsg, QString::number( errLine ), QString::number( errColumn ) ) );
#endif
    return false;
  }
  return true;
}

QString FLUtil::sha1( const QString & str ) {
  QBuffer b;
  b.open( IO_WriteOnly );
  QTextStream t( &b );
  t << str;
  b.close();

  FLSha1 sha1;
  sha1.Update(( unsigned char * ) b.buffer().data(), b.buffer().size() );
  sha1.Final();
  char strC[ 255 ];
  strC[ 0 ] = '\0';
  sha1.ReportHash( strC );

  return strC;
}

QString FLUtil::usha1( uchar * data, uint len ) {
  FLSha1 sha1;
  sha1.Update( data, len );
  sha1.Final();
  char strC[ 255 ];
  strC[ 0 ] = '\0';
  sha1.ReportHash( strC );
  return strC;
}

QVariant FLUtil::readDBSettingEntry( const QString & key ) {
  QVariant v = sqlSelect( "flsettings", "valor", "flkey = '" + key + "'" );
  return v;
}

bool FLUtil::writeDBSettingEntry( const QString & key, const QString & value ) {
  int size;
  bool result;
  sqlSelect( "flsettings", "valor", "flkey = '" + key + "'", "flsettings", &size );
  if ( size > 0 )
    result = sqlUpdate( "flsettings", "valor", value, "flkey = '" + key + "'" );
  else
    result = sqlInsert( "flsettings", "flkey,valor", key + "," + value );
  return result;
}

QVariant FLUtil::readSettingEntry( const QString & key, const QString &def, bool *ok ) {
  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );
  return config.readEntry( keybase + key, def, ok );
}

bool FLUtil::writeSettingEntry( const QString & key, const QString & value ) {
  QSettings config;
  config.setPath( "InfoSiAL", "FacturaLUX", QSettings::User );
  QString keybase( "/facturalux/lite/" );
  return config.writeEntry( keybase + key, value );
}

QString FLUtil::roundFieldValue( const QVariant & n, const QString & table, const QString & field ) {
  FLTableMetaData * tmd = FLSqlConnections::database()->manager()->metadata( table );
  if ( !tmd )
    return 0;

  FLFieldMetaData * fmd = tmd->field( field );
  if ( !fmd )
    return 0;

  return buildNumber( n, 'f', fmd->partDecimal() );
}

QString FLUtil::buildNumber( const QVariant & v, char tipo, int partDecimal ) {
  QString ret;
  double d = QVariant( v ).toDouble() * pow( 10, partDecimal );
  d = round( d );
  d = d / pow( 10, partDecimal );
  ret.setNum( d , tipo, partDecimal );
  return ret;
}

QImage FLUtil::snapShotUI( const QString &n ) {
  QWidget * w = FLSqlConnections::database()->managerModules()->createUI( n );

  if ( w ) {
    QPixmap pix = QPixmap::grabWidget( w );
    return pix.convertToImage();
  }
}

void FLUtil::saveSnapShotUI( const QString & n, const QString & pathFile ) {
  QFile fi( pathFile );
  if ( !fi.open( IO_WriteOnly ) ) {
#ifdef FL_DEBUG
    qWarning( "FLUtil : " + QApplication::tr( "Error I/O al intentar escribir el fichero %1" ).arg( pathFile ) );
#endif
    return ;
  }
  snapShotUI( n ).save( &fi, "PNG" );
}

QVariant::Type FLUtil::flDecodeType( int fltype ) {
  return FLFieldMetaData::flDecodeType( fltype );
}

void FLUtil::saveIconFile( const QString & data, const QString & pathFile ) {
  QFile fi( pathFile );
  if ( !fi.open( IO_WriteOnly ) ) {
#ifdef FL_DEBUG
    qWarning( "FLUtil : " + QApplication::tr( "Error I/O al intentar escribir el fichero %1" ).arg( pathFile ) );
#endif

    return ;
  }

  QImage img;
  const int lengthOffset = 4;
  int baSize = data.length() / 2 + lengthOffset;
  uchar *ba = new uchar[ baSize ];
  for ( int i = lengthOffset; i < baSize; ++i ) {
    char h = data[ 2 * ( i - lengthOffset )].latin1();
    char l = data[ 2 * ( i - lengthOffset ) + 1 ].latin1();
    uchar r = 0;
    if ( h <= '9' )
      r += h - '0';
    else
      r += h - 'a' + 10;
    r = r << 4;
    if ( l <= '9' )
      r += l - '0';
    else
      r += l - 'a' + 10;
    ba[ i ] = r;
  }

  img.loadFromData(( const uchar* ) ba + lengthOffset, baSize - lengthOffset, "PNG" );
  img.save( pathFile, "PNG" );

  delete [] ba;

#ifdef FL_DEBUG
  qWarning( pathFile );
#endif
}

QString FLUtil::getIdioma() {
  QString lang( QTextCodec::locale() );
  return lang.left( 2 );
}

QString FLUtil::getOS() {
#if defined(Q_OS_WIN32)
  return "WIN32";
#endif

#if defined (Q_OS_LINUX)
  return "LINUX";
#endif

#if defined(Q_OS_MACX)
  return "MACX";
#endif
}

