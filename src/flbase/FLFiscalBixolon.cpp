#include <qmessagebox.h>
#include <qapplication.h>

#include "FLFiscalBixolon.h"

FLFiscalBixolon::FLFiscalBixolon()
{
  qDebug("FLFiscalBixolon Constructor");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    "Constructor" );
}

FLFiscalBixolon::~FLFiscalBixolon()
{
  qDebug("FLFiscalBixolon Destructor");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    "Destructor" );
}

bool FLFiscalBixolon::openPort( QString port )
{
  qDebug(QString("openPort(%1)").arg(port).ascii());
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("openPort(%1)").arg(port) );
  return true;
}

bool FLFiscalBixolon::closedPort()
{
  qDebug("FLFiscalBixolon closedPort");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    "closedPort" );
  return true;
}

bool FLFiscalBixolon::checkPrinter()
{
  qDebug("FLFiscalBixolon checkPrinter");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    "checkPrinter" );
  return true;
}

bool FLFiscalBixolon::readStatus( int * status, int * error )
{
  qDebug("FLFiscalBixolon readStatus");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("readStatus %1").arg(*status).ascii() );
  return true;
}

bool FLFiscalBixolon::sendCmd( int * status, int * error, QString cmd )
{
  qDebug("FLFiscalBixolon sendCmd");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("sendCmd( status=%1, cmd=%2").arg(*status).arg(cmd).ascii() );
  return true;
}

int FLFiscalBixolon::sendNcmd( int * status, int * error, QString cmd )
{
  qDebug("FLFiscalBixolon sendNcmd");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("sendNCmd( status=%1, cmd=%2").arg(*status).arg(cmd).ascii() );
  return 0;
}

int FLFiscalBixolon::sendFile( int * status, int * error, QString file )
{
  qDebug("FLFiscalBixolon SendFile");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("sendFile( status=%1, file=%2").arg(*status).arg(file).ascii() );
  return 0;
}

bool FLFiscalBixolon::uploadStatus( int * status, int * error, QString cmd, QString file )
{
  qDebug("FLFiscalBixolon uploadStatus");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    QString("UploadStatus( status=%1, cmd=%2 file=%3").arg(*status).arg(cmd).arg(file).ascii() );
  return true;
}
