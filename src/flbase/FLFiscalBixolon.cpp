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

  error = 0;
	//verificado y OK!	
}
bool FLFiscalBixolon::openPort( QString port ) {

  typedef bool (* FNPTR)(char *);

  FNPTR pFn;

  bool result = false; // esto me asegura que siempre tengo un resultado

  pFn = (FNPTR) lib->resolve("OpenFpctrl");

  // esta verificacion es iportante para asegurar que este definido el apuntador
  if(pFn) {
    result = pFn(const_cast<char *>(port.ascii()));
    qDebug(QString("openPort(%1), abierto correcto").arg(port).ascii());
    QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("openPort(%1), abierto correcto").arg(port) );
  } else {
    qDebug(QString("openPort(%1, Error)").arg(port).ascii());
    QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("openPort(%1, Error)").arg(port) );
  }
  // los QMessage... me dicen si funciono o fallo abriendo el puerto


  // regreso el resultado, verdadero si se pudo abrir el puerto
  // o falso si fallo por alguna razon
  return result;
}


bool FLFiscalBixolon::closedPort(){

	typedef bool (* FNPTR)( void );

  bool result = false; // esto me asegura que siempre tengo un resultado

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("CloseFpctrl");
  if(pFn){
    
  	  result = pFn();
  }
  qDebug("FLFiscalBixolon closedPort");
  QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "closedPort" );
  return result;
}


bool FLFiscalBixolon::checkPrinter( ){
	
  typedef bool (* FNPTR)( void );

	FNPTR pFn;

  bool result = false; // esto me asegura que siempre tengo un resultado

  pFn = (FNPTR) lib->resolve("CheckFprinter");

    if(pFn){
    result = pFn();
    if( result ) {
      qDebug("FLFiscalBixolon checkPrinter, impresora lista");
      QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "impresora lista" );
    } else {
      qDebug("FLFiscalBixolon checkPrinter, impresora con error");
      QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "impresora con error" );
    }
  } else {
    qDebug("FLFiscalBixolon checkPrinter, error en la funcion");
    QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "error en la funcion checkPrinter" );
  }
  return result;
}


bool FLFiscalBixolon::readStatus( int status, int error ){
	
	int *st = &status;
        int *err = &error;

	typedef bool (* FNPTR)( int *, int *  );
	
	FNPTR pFn;

	bool result = false; // esto me asegura que siempre tengo un resultado

/*	typedef bool (* FNPTR)( int *, int * );

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("ReadFpStatus");

	if(pFn){
	return pFn( status, error );
  }*/

	pFn=(FNPTR)lib->resolve("ReadFpStatus");

	if(pFn){
	
		result = pFn( st, err );
		 if( result ) {
      			qDebug("FLFiscalBixolon readStatus, Lectura de Status");
     			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("readStatus %1").arg(*st).ascii() );
    		} else {
      			qDebug("FLFiscalBixolon readStatus, No hubo Retorno");
      			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Sin Retorno de Status" );
    		}
	}else{
		qDebug("FLFiscalBixolon readStatus, error en la función");
  		QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "error en la función de readStatus" );

	}
	
  	return result;

}

int FLFiscalBixolon::readStatus(){

  int status;

  typedef bool (* FNPTR)( int *, int *  );

  bool result = false; // esto me asegura que siempre tengo un resultado

  FNPTR pFn;

  pFn=(FNPTR)lib->resolve("ReadFpStatus");

  if(pFn){

    result = pFn( &status, &error );
     if( result ) {
            qDebug("FLFiscalBixolon readStatus, Lectura de Status");
          QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("readStatus %1").arg(status).ascii() );
        } else {
            qDebug("FLFiscalBixolon readStatus, No hubo Retorno");
            QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Sin Retorno de Status" );
            status = -1;
        }
  }else{
    qDebug("FLFiscalBixolon readStatus, error en la función");
      QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "error en la función de readStatus" );

  }

    return status;

}

bool FLFiscalBixolon::sendCmd( int status, int error, QString cmd ){

	int *st = &status;
        int *err = &error;

	typedef bool (* FNPTR)( int *, int *, char *);
	
	FNPTR pFn;

	bool result = false; // esto me asegura que siempre tengo un resultado

	
/*	typedef bool (* FNPTR)( int *, int *, char *);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();
	
	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");
	
	if(pFn){
	return pFn( status, error, cmdcmd );
  }*/
	pFn=(FNPTR) lib->resolve("SendCmd");

	if(pFn){
		
		result = pFn( st, err, const_cast<char *>(cmd.ascii()) );

		 	if( result ) {
      				qDebug("FLFiscalBixolon sendCmd, Envío de Comando");
     				QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("sendCmd( status=%1, cmd=%2").arg(*st).arg(cmd).ascii() );
    			} else {
      				qDebug("FLFiscalBixolon sendCmd, No se Envió");
      				QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "No se Envió el comando" );
    			}
	
		qDebug("FLFiscalBixolon sendCmd");

  		QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Error en la Función" );
	}

	
  	
	return result;
}


int FLFiscalBixolon::sendNcmd( int status, int error,  QString cmd ){

	int *st = &status;
        int *err = &error;

	typedef int (* FNPTR)( int *, int *, char * );

	FNPTR pFn;

	int result; // esto me asegura que siempre tengo un resultado
	
/*	typedef int (* FNPTR)( int *, int *, char * );
	
	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);
	
	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendCmd");

	if(pFn){	
	return pFn( status, error, cmdcmd );
  }*/
	pFn=(FNPTR) lib->resolve("SendCmd");
	
	if(pFn){
		result =  pFn( st, err, const_cast<char *>(cmd.ascii()));

		if(result){

			qDebug("FLFiscalBixolon sendNcmd, Envío de Lotes de Comandos");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("sendNCmd( status=%1, cmd=%2").arg(*st).arg(cmd).ascii() );	

		}else{
			qDebug("FLFiscalBixolon sendNcmd, No se Envió lotes de Comandos");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "No se envió Comando" );
		}
		qDebug("FLFiscalBixolon sendNcmd");
  		QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Hubo un error en la Función" );
	}
  	return result;
}


int FLFiscalBixolon::sendFile( int status, int error,  QString file ){

	int *st = &status;
        int *err = &error;

	typedef int (* FNPTR)( int *, int *,  char * );

	FNPTR pFn;

	int result = -1; // esto me asegura que siempre tengo un resultado

/*	typedef int (* FNPTR)( int *, int *,  char * );

	const char * cfile = file.ascii();

//	const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("SendFileCmd");

	if(pFn){

		return pFn( status, error, cmdfile );
  }*/
	pFn=(FNPTR) lib->resolve("SendFileCmd");

	if(pFn){
		result = pFn( st, err, const_cast<char *>(file.ascii()) );
		if(result != -1){

			qDebug("FLFiscalBixolon sendFile, Se Envío Archivo");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("sendFile( status=%1, file=%2").arg(*st).arg(file).ascii());

		}else{

			qDebug("FLFiscalBixolon sendFile");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Hubo error en el envío");

		}

		qDebug("FLFiscalBixolon sendFile");
  		QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Hubo error en la función" );
	}

  	
  	return result;
}


bool FLFiscalBixolon::uploadStatus( int status, int error,  QString cmd,  QString file ){

	int *st = &status;
        int *err = &error;

	typedef bool (* FNPTR)( int *, int *,  char *,  char * );

	FNPTR pFn;

	bool result = false; // esto me asegura que siempre tengo un resultado

/*	typedef bool (* FNPTR)( int *, int *,  char *,  char * );

	const char * cfile = file.ascii();

	//const char * f = cfile.data();

	char * cmdfile = const_cast<char *>(cfile);

	const char * ccmd = cmd.ascii();

	//const char * cm = ccmd.data();

	char * cmdcmd = const_cast<char *>(ccmd);

	FNPTR pFn;

	pFn=(FNPTR)lib->resolve("UploadStatusCmd");

	if(pFn){
		return pFn( status, error, cmdcmd, cmdfile );
  }*/
		
	pFn=(FNPTR) lib->resolve("UploadStatusCmd");

	if(pFn){
		result = pFn( st, err, const_cast<char *>(cmd.ascii()), const_cast<char *>(file.ascii()) );

		if(result){

			qDebug("FLFiscalBixolon uploadStatus, Se envió correctamente Archivo de Status");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", QString("uploadStatus( status=%1, cmd=%2 file=%3").arg(*st).arg(cmd).arg(file).ascii() );

		}else{
			qDebug("FLFiscalBixolon uploadStatus, Ocurrió un Error en el envio de status");
  			QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", " error enviando status " );
		}


	qDebug("FLFiscalBixolon uploadStatus");
  	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Error en la Función" );
	}


	return result;
}
FLFiscalBixolon::~FLFiscalBixolon(){

	lib->unload();
	
	qDebug("FLFiscalBixolon Destructor");
  	
	QMessageBox::information( qApp->mainWidget(), "FLFiscalBixolon", "Destructor" );	
};
