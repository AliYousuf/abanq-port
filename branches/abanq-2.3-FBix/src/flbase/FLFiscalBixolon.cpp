FLFiscalBixolon::FLFiscalBixolon()
{

}

FLFiscalBixolon::~FLFiscalBixolon()
{

}

bool FLFiscalBixolon::OpenPort( QString port )
{
  return true;
}

bool FLFiscalBixolon::ClosedPort()
{
  return true;
}

bool CheckPrinter()
{
  return true;
}

bool FLFiscalBixolon::ReadStatus( int * status, int * error )
{
  return true;
}

bool FLFiscalBixolon::SendCmd( int * status, int * error, QString cmd )
{
  return true;
}

int FLFiscalBixolon::SendNcmd( int * status, int * error, QString cmd )
{
  return 0;
}

int FLFiscalBixolon::SendFile( int * status, int * error, QString file )
{
  return 0;
}

bool FLFiscalBixolon::UploadStatus( int * status, int * error, char *cmd, QString file )
{
  return true;
}
