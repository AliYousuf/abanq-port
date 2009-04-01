/***************************************************************************
                            FLFiscalBixolon.cpp
                          -------------------
 begin                : mie mar 18 2009
 
***************************************************************************/

#include "FLFiscalBixolon.h"
#include <string.h>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qapplication.h>


FLFiscalBixolon::FLFiscalBixolon() {

	lib = new QLibrary("tfhkaif");

	qDebug("FLFiscalBixolon Constructor");

  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Constructor" );
	//verificado y OK!	
}
bool FLFiscalBixolon::openPort( QString port ) {
/*
	typedef bool (* FNPTR)(char *);
	
	const char * cport = port.ascii();

	//const char * prt = port.data();

	char * cmdport = const_cast<char *>(cport);

	FNPTR pFn;

	//esta linea parece no devolver nada o estar mal

	pFn=(FNPTR)lib->resolve("OpenFpctrl");

	//if (pFn){
	//justo aquí se cierra
	 pFn(cmdport);	
	//} 
	//pero envía la información al puerto
	qDebug(QString("openPort(%1)").arg(port).ascii());
	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("openPort(%1)").arg(port) );
  return true;
*/
  typedef bool (* FNPTR)(char *);
  FNPTR pFn;

  bool result = false; // esto me asegura que siempre tengo un resultado

  pFn = (FNPTR) lib->resolve("OpenFpctrl");

  // esta verificacion es iportante para asegurar que este definido el apuntador
  if(pFn) {
   result = pFn(const_cast<char *>(port.ascii()));
   QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("openPort(%1), abierto correcto").arg(port) );
  } else {
    QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("openPort(%1, Error)").arg(port) );
  }
  // los QMessage... me dicen si funciono o fallo abriendo el puerto


  // regreso el resultado, verdadero si se pudo abrir el puerto
  // o falso si fallo por alguna razon
  return result;
}


bool FLFiscalBixolon::closedPort(){
/*
	typedef bool (* FNPTR)( void );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("CloseFpctrl");
	if(pFn){
		return pFn();	
  }*/
	qDebug("FLFiscalBixolon closedPort");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "closedPort" );
  	return true;
}


bool FLFiscalBixolon::checkPrinter( ){
	
/*	typedef bool (* FNPTR)( void );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("CheckFprinter");

	if(pFn){
		return pFn();	
  }*/
	qDebug("FLFiscalBixolon checkPrinter");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "checkPrinter" );
  	return true;	
}


bool FLFiscalBixolon::readStatus( int * status, int * error ){

/*	typedef bool (* FNPTR)( int *, int * );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("ReadFpStatus");

	if(pFn){
	return pFn( status, error );
  }*/
	qDebug("FLFiscalBixolon readStatus");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("readStatus %1").arg(*status).ascii() );
  	return true;

}

bool FLFiscalBixolon::sendCmd( int * status, int * error, QString cmd ){
	
/*	typedef bool (* FNPTR)( int *, int *, char *);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();
	
	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");
	
	if(pFn){
	return pFn( status, error, cmdcmd );
  }*/
	qDebug("FLFiscalBixolon sendCmd");

  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("sendCmd( status=%1, cmd=%2").arg(*status).arg(cmd).ascii() );
  	
	return true;
}


int FLFiscalBixolon::sendNcmd( int * status, int * error,  QString cmd ){
	
/*	typedef int (* FNPTR)( int *, int *, char * );
	
	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);
	
	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");

	if(pFn){	
	return pFn( status, error, cmdcmd );
  }*/
	qDebug("FLFiscalBixolon sendNcmd");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("sendNCmd( status=%1, cmd=%2").arg(*status).arg(cmd).ascii() );
  	return 0;
}


int FLFiscalBixolon::sendFile( int * status, int * error,  QString file ){

/*	typedef int (* FNPTR)( int *, int *,  char * );

	const char * cfile = file.ascii();

//	const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendFileCmd");

	if(pFn){

		return pFn( status, error, cmdfile );
  }*/
  	qDebug("FLFiscalBixolon SendFile");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    	QString("sendFile( status=%1, file=%2").arg(*status).arg(file).ascii() );
  	return 0;
}


bool FLFiscalBixolon::uploadStatus( int * status, int * error,  QString cmd,  QString file ){

/*	typedef bool (* FNPTR)( int *, int *,  char *,  char * );

	const char * cfile = file.ascii();

	//const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendFileCmd");

	if(pFn){
		return pFn( status, error, cmdcmd, cmdfile );
  }*/
	qDebug("FLFiscalBixolon uploadStatus");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon",
    	QString("UploadStatus( status=%1, cmd=%2 file=%3").arg(*status).arg(cmd).arg(file).ascii() );
	return true;
}
FLFiscalBixolon::~FLFiscalBixolon(){

	lib->unload();
	
	qDebug("FLFiscalBixolon Destructor");
  	
	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Destructor" );	
};
