#ifndef FLFISCALBIXOLON_H
#define FLFISCALBIXOLON_H

#include <config.h>

class FL_EXPORT FLFiscalBixolon :public QObject
{
  Q_OBJECT

public:

  FLFiscalBixolon();

  ~FLFiscalBixolon();

  bool OpenPort( QString port );

  bool ClosedPort();

  bool CheckPrinter();

  bool ReadStatus( int * status, int * error );

  bool SendCmd( int * status, int * error, QString cmd );

  int SendNcmd( int * status, int * error, QString cmd );

  int SendFile( int * status, int * error, QString file );

  bool UploadStatus( int * status, int * error, char *cmd, QString file );

};

#endif

