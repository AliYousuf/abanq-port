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

public:

	FLFiscalBixolon();

	bool openPort( QString port );
	
	bool closedPort( );
	
	bool checkPrinter( );
	
	bool readStatus( int status, int error );
	
	bool sendCmd( int status, int error, QString cmd );
	
	int sendNcmd( int status, int error, QString cmd );
	
	int sendFile( int status, int error, QString file );
	
	bool uploadStatus( int status, int error, QString cmd, QString file );

	~FLFiscalBixolon();



};

#endif 

