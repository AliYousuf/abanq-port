/***************************************************************************                             
* 			    FLFiscalBixolon.h
*                           ------------------- 
*			  begin: vie mar 13 2009

***************************************************************************/
#ifndef FLFISCALBIXOLON_H
#define FLFISCALBIXOLON_H

#include <config.h>
#include <qobject.h>
#include <qlibrary.h>

class FLFiscalBixolonInterface;

class FL_EXPORT FLFiscalBixolon : public QObject
{

   Q_OBJECT

   friend class FLFiscalBixolonInterface;

private:

  FLFiscalBixolonInterface *f;

  QLibrary *lib; 

  int error;

public:

	FLFiscalBixolon();

	bool openPort( QString port );
	
	bool closedPort( );
	
	bool checkPrinter( );
	
 	bool readStatus( int status, int _error );

	int readStatus();

	int lastError() { return error; }
	
	bool sendCmd( int status, int _error, QString cmd );
	
	int sendCmd( QString cmd );

	int sendNcmd( int status, int _error, QString cmd );

	int sendNcmd( QString cmd );
	
 	int sendFile( int status, int _error, QString file );

	int sendFile( QString file );
	
  	bool uploadStatus( int status, int _error, QString cmd, QString file );

	int uploadStatus( QString cmd, QString file );

	bool uploadReport( int status, int _error, QString cmd, QString file );

	int uploadReport( QString cmd, QString file );

	~FLFiscalBixolon();

};

#endif 

