FLFiscalBixolon::FLFiscalBixolon()
{

}

FLFiscalBixolon::~FLFiscalBixolon()
{

}

bool FLFiscalBixolon::openPort( QString port )
{
  return true;
}

bool FLFiscalBixolon::closedPort()
{
  return true;
}

bool checkPrinter()
{
  return true;
}

bool FLFiscalBixolon::readStatus( int * status, int * error )
{
  return true;
}

bool FLFiscalBixolon::sendCmd( int * status, int * error, QString cmd )
{
  return true;
}

int FLFiscalBixolon::sendNcmd( int * status, int * error, QString cmd )
{
  return 0;
}

int FLFiscalBixolon::sendFile( int * status, int * error, QString file )
{
  return 0;
}

bool FLFiscalBixolon::uploadStatus( int * status, int * error, char *cmd, QString file )
{
  return true;
}
