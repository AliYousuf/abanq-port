#include "FLFiscalBixolon.h"

FLFiscalBixolon::FLFiscalBixolon()
{
  qDebug("FLFiscalBixolon Constructor");
}

FLFiscalBixolon::~FLFiscalBixolon()
{
  qDebug("FLFiscalBixolon Destructor");
}

bool FLFiscalBixolon::openPort( QString port )
{
  qDebug(QString("FLFiscalBixolon openPort(%1)").arg(port).ascii());
  return true;
}

bool FLFiscalBixolon::closedPort()
{
  qDebug("FLFiscalBixolon closedPort");
  return true;
}

bool FLFiscalBixolon::checkPrinter()
{
  qDebug("FLFiscalBixolon checkPrinter");
  return true;
}

bool FLFiscalBixolon::readStatus( int * status, int * error )
{
  qDebug("FLFiscalBixolon readStatus");
  return true;
}

bool FLFiscalBixolon::sendCmd( int * status, int * error, QString cmd )
{
  qDebug("FLFiscalBixolon sendCmd");
  return true;
}

int FLFiscalBixolon::sendNcmd( int * status, int * error, QString cmd )
{
  qDebug("FLFiscalBixolon sendNcmd");
  return 0;
}

int FLFiscalBixolon::sendFile( int * status, int * error, QString file )
{
  qDebug("FLFiscalBixolon SendFile");
  return 0;
}

bool FLFiscalBixolon::uploadStatus( int * status, int * error, QString cmd, QString file )
{
  qDebug("FLFiscalBixolon uploadStatus");
  return true;
}
