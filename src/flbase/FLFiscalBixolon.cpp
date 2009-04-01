/***************************************************************************
                            FLFiscalBixolon.cpp
                          -------------------
 begin                : mie mar 18 2009
 
***************************************************************************/

#include "FLFiscalBixolon.h"
#include <string.h>
#include <stdlib.h>


FLFiscalBixolon::FLFiscalBixolon() {

	lib = new QLibrary("tfhkaif");	
}
bool FLFiscalBixolon::openPort( QString port ) {

	typedef bool (* FNPTR)(char *);
	
	const char * cport = port.ascii();

	//const char * prt = port.data();

	char * cmdport = const_cast<char *>(cport);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("OpenFpctrl");

	return pFn(cmdport);	
	
}


bool FLFiscalBixolon::closedPort(){
	
	typedef bool (* FNPTR)( void );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("CloseFpctrl");

	return pFn();	
}


bool FLFiscalBixolon::checkPrinter( ){
	
	typedef bool (* FNPTR)( void );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("CheckFprinter");

	return pFn();		
}


bool FLFiscalBixolon::readStatus( int * status, int * error ){

	typedef bool (* FNPTR)( int *, int * );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("ReadFpStatus");

	return pFn( status, error );

}

bool FLFiscalBixolon::sendCmd( int * status, int * error, QString cmd ){
	
	typedef bool (* FNPTR)( int *, int *, char *);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();
	
	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");

	return pFn( status, error, cmdcmd );
}


int FLFiscalBixolon::sendNcmd( int * status, int * error,  QString cmd ){
	
	typedef int (* FNPTR)( int *, int *, char * );
	
	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);
	
	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");

	
	return pFn( status, error, cmdcmd );
}


int FLFiscalBixolon::sendFile( int * status, int * error,  QString file ){

	typedef int (* FNPTR)( int *, int *,  char * );

	const char * cfile = file.ascii();

//	const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendFileCmd");

	return pFn( status, error, cmdfile );
}


bool FLFiscalBixolon::uploadStatus( int * status, int * error,  QString cmd,  QString file ){

	typedef bool (* FNPTR)( int *, int *,  char *,  char * );	

	const char * cfile = file.ascii();

	//const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendFileCmd");
	
	return pFn( status, error, cmdcmd, cmdfile );
}
FLFiscalBixolon::~FLFiscalBixolon(){
	lib->unload();	
};
