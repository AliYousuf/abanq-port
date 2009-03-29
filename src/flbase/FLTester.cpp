/***************************************************************************
                          FLTester.cpp
                       -------------------
begin                : lun 29 Mar 2004
copyright            : (C) 2004-2005 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
   Este  programa es software libre. Puede redistribuirlo y/o modificarlo
   bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
   versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#include "FLAction.h"
#include "FLApplication.h"
#include "FLDataTable.h"
#include "FLFieldDB.h"
#include "FLFieldMetaData.h"
#include "FLFormDB.h"
#include "FLFormRecordDB.h"
#include "FLFormSearchDB.h"
#include "FLManager.h"
#include "FLManagerModules.h"
#include "FLSqlCursor.h"
#include "FLSqlQuery.h"
#include "FLTableMetaData.h"
#include "FLTableDB.h"
#include "FLTester.h"
#include "FLSqlDatabase.h"
#include "FLSqlDriver.h"
#include "FLSqlConnections.h"

#define DELAY 20

FLTester::FLTester() : testing_( false ), dbTest_( 0 ), currentForm_( 0 ), fallos_( 0 ), queueTesterEvents( 0 ), fdbQueue_( 0 ) {}

FLTester::~FLTester() {}

void FLTester::start( const QString &codSessionCat, const QString &idFuncional, const QString &bd ) {
#ifdef FL_TEST
  FLTester * appTester = (( FLApplication * ) qApp ) ->tester();
#else
  FLTester * appTester = 0;
#endif
  if ( appTester != this ) {
    currentSession_ = appTester->currentSession();
    dbTest_ = appTester->dbTest();
  } else {
    if ( !bd.isNull() )
      openTestDB( bd );
    else
      openTestDB( FLSqlConnections::database()->database() );

    if ( !dbTest_ ) {
#ifdef FL_DEBUG
      qWarning( "Error al conectar con la base de datos de pruebas" );
#endif
      return;
    }
    currentSession_ = createSession( codSessionCat, idFuncional );
    if ( currentSession_ == 0 ) {
#ifdef FL_DEBUG
      qWarning( "Error al generar el guión de la sesión" );
#endif
      return ;
    }
  }

  currentModule_ = "";
  errorDescription_ = "";

  sessionStatus_ = "Pendant";
  testing_ = true;

  if ( !fdbQueue_ ) {
    fdbQueue_ = new QPtrQueue < FLTestFdb >;
    fdbQueue_->setAutoDelete( true );
  }
  objectCondition_ = QString::null;
  condition_ = NO_CONDITION;
  enqueueTesterEvent( condition_, objectCondition_ );

#ifdef FL_DEBUG
  qWarning( "Fin" );
#endif
}

bool FLTester::tryContinue( ) {
  if ( !queueTesterEvents )
    return false;

  if ( queueTesterEvents->isEmpty() ) {
    QApplication::eventLoop() ->processEvents( QEventLoop::AllEvents );
    qWarning( "Hay eventos pendientes" );
    return false;
  }

  FLTesterEvent * testerEvent = queueTesterEvents->dequeue();
  int c = testerEvent->condition;
  QString oC = testerEvent->objectCondition;
  delete testerEvent;

  return tryContinue( c, oC );
}

bool FLTester::tryContinue( const int c, const QString & oC ) {
  if ( !testing_ )
    return false;

  while ( QApplication::eventLoop() ->hasPendingEvents() )
    QApplication::eventLoop() ->processEvents( QEventLoop::AllEvents );

  qWarning( QString::number( c ) + " - " + oC );
  if ( c == NO_CONDITION ) {
    if ( !execTest() )
      end();
    else
      return true;
  } else if ( c == condition_ && ( oC == objectCondition_ || objectCondition_ == "ANY" ) ) {
    if ( !execTest() )
      end();
    else
      return true;
  } else
    qWarning( "NO ACEPTADO (" + QString::number( condition_ ) + " - " + objectCondition_ + ")" );

  return false;
}

void FLTester::end() {
  testing_ = false;
  QMessageBox::information( qApp->focusWidget(), tr( "Tests" ), tr( "La sesión de pruebas ha terminado con estado: " ) + sessionStatus_, QMessageBox::Ok, 0, 0 );

  if ( queueTesterEvents ) {
    queueTesterEvents->clear();
    delete queueTesterEvents;
    queueTesterEvents = 0;
  }

  if ( fdbQueue_ ) {
    fdbQueue_->clear();
    delete fdbQueue_;
    fdbQueue_ = 0;
  }
}

bool FLTester::execTransaction( const QString &qrySql ) {
  if ( dbTest_ && dbTest_->db()->transaction() ) {
    QSqlQuery q( qrySql, dbTest_->db() );
    if ( q.exec() ) {
      if ( dbTest_->db()->commit() ) {
        return true;
      }
    } else {
      if ( dbTest_->db()->rollback() ) {
        return false;
      }
    }
  }
  return false;
}

bool FLTester::execTest() {
  bool ret = false;
  uint status = 0;
  QString idStep, idStepCat, testLevel;
  QSqlQuery q( QString::null, dbTest_->db() );

  q.setForwardOnly( true );
  if ( fdbQueue_->isEmpty() ) {
    q.exec( "SELECT idstep, idtest, idstepcat, testlevel FROM tt_step WHERE status = 'Pendant' ORDER BY idstep" );
  } else {
    q.exec( "SELECT idstep, idtest, idstepcat, testlevel FROM tt_step WHERE status = 'Progress' ORDER BY idstep" );
  }

  if ( q.next() ) {
    idStep = q.value( 0 ).toString();
    currentTest_ = q.value( 1 ).toString();
    idStepCat = q.value( 2 ).toString();
    testLevel = q.value( 3 ).toString();

    if ( previousTest_ != currentTest_ ) {
      q.exec( "SELECT ts.parameters, ts.codtestcat, ts.idfuncional FROM tt_test t INNER JOIN tt_tcatscat ts ON t.idtcatscat = ts.idtcatscat WHERE idtest = " + currentTest_ );
      if ( !q.next() )
        return false;
      if ( !assignateDefaultValues( "<FLTest:Test codtest=\"" + q.value( 1 ).toString() + "\" idfuncional=\"" + q.value( 2 ).toString() + "\"/>" ) )
        return false;
      QString testParam = q.value( 0 ).toString();
      if ( !processParameters( testParam ) )
        return false;
      if ( !assignateValues( testParam ) )
        return false;
    }

    if ( !execTransaction( "UPDATE tt_step SET status = 'Progress' WHERE idstep = " + idStep ) )
      return false;
    status = execStep( idStepCat, idStep );
    switch ( status ) {
      case OK: {
        if ( fdbQueue_->isEmpty() ) {
          if ( !execTransaction( "UPDATE tt_step SET status = 'Ok' WHERE idstep = " + idStep ) ) {
            ret = false;
            break;
          }
          if ( previousTest_ != currentTest_ )
            setTestStatus( "Ok" );
        }
        fallos_ = 0;
        currentForm_ = 0;
        ret = true;
        break;
      }
      case NEXT: {
        if ( fdbQueue_->isEmpty() ) {
          if ( !execTransaction( "UPDATE tt_step SET status = 'Ok' WHERE idstep = " + idStep ) ) {
            ret = false;
            break;
          }
          if ( previousTest_ != currentTest_ )
            setTestStatus( "Ok" );
        }
        fallos_ = 0;
        ret = execTest();
        break;
      }
      case REPEAT: {
#ifdef FL_DEBUG
        qWarning( "repitiendo..." );
#endif

        if ( !execTransaction( "UPDATE tt_step SET status = 'Pendant' WHERE idstep = " + idStep ) ) {
          ret = false;
          break;
        }
        if ( fallos_++ > 10 ) {
          ret = false;
          break;
        }
        currentForm_ = 0;
        ret = false;
        QTime start = QTime::currentTime();
        QTime now;
        while ( testing_ ) {
          FLApplication::eventLoop() ->processEvents( QEventLoop::AllEvents );
          now = QTime::currentTime();
          if ( start.msecsTo( now ) > DELAY )
            break;
        }
        ret = execTest();
        break;
      }
      case SKIP_TEST: {
        QString qrySql;
        QSqlQuery qryNextStep( QString::null, dbTest_->db() );
        qryNextStep.setForwardOnly( true );
        if ( !qryNextStep.exec( "SELECT idstep FROM tt_step WHERE idstep > " + idStep + " AND (testlevel < " + testLevel + " OR (testlevel = " + testLevel + " AND firststep = true)) ORDER BY idstep" ) ) {
          ret = false;
          break;
        }
        if ( !qryNextStep.next() )
          qrySql = "UPDATE tt_step SET status = 'Skipped' WHERE idtest = " + currentTest_ + " AND status IN ('Pendant', 'Progress')";
        else
          qrySql = "UPDATE tt_step SET status = 'Skipped' WHERE idtest = " + currentTest_ + " AND idstep < " + qryNextStep.value( 0 ).toString() + " AND status IN ('Pendant', 'Progress')";

        if ( !execTransaction( qrySql ) ) {
          ret = false;
          break;
        }
        fallos_ = 0;
        currentForm_ = 0;
        ret = execTest();
        break;
      }
      case ERR:
      case TESTER_ERROR: {
        if ( !execTransaction( "UPDATE tt_step SET status = 'Error', errordescription = '" + errorDescription_ + "' WHERE idstep = " + idStep ) ) {
          ret = false;
          break;
        }
        setTestStatus( "Error" );
        setSessionStatus( "Error" );
        fallos_ = 0;
        currentForm_ = 0;
        ret = false;
      }
    }
  } else {
    setTestStatus( "Ok" );
    setSessionStatus( "Ok" );
    ret = false;
    fallos_ = -1;
    currentForm_ = 0;
#ifdef FL_DEBUG
    qWarning( "La sesión de pruebas ha terminado correctamente" );
#endif

  }

  return ret;
}

int FLTester::execStep( const QString & idStepCat, const QString & idStep ) {
  QSqlQuery qryStep( QString::null, dbTest_->db() );

  qryStep.setForwardOnly( true );
  if ( !qryStep.exec( "SELECT idaction, objectname, idobjecttype, container, formtype, parameters, idmodule FROM tt_stepcat WHERE idstepcat = '" + idStepCat + "'" ) ) {
    errorDescription_ = tr( "La consulta para obtener los datos asociados al paso " ) + idStep + tr( " falló" );
    return ERR;
  }

  if ( !qryStep.next() ) {
    errorDescription_ = tr( "No existen los datos asociados al paso" ) + idStep;
    return ERR;
  }

  QString idAction = qryStep.value( 0 ).toString();
  QString objectName = qryStep.value( 1 ).toString();
  QString idObjectType = qryStep.value( 2 ).toString();
  QString formType = qryStep.value( 4 ).toString();
  QString container = qryStep.value( 4 ).toString() + qryStep.value( 3 ).toString();
  QString parameters = qryStep.value( 5 ).toString();
  QString idModule = qryStep.value( 6 ).toString();

  if ( !processParameters( parameters ) )
    return ERR;

#ifdef FL_DEBUG

  qWarning( "C: " + container + " T: " + idObjectType + " O: " + objectName + " A: " + idAction );
#endif

  if ( !idModule.isEmpty() && idModule != currentModule_ ) {
    currentModule_ = idModule;
    (( FLApplication * ) qApp ) ->activateModule( idModule );
  }

  if ( !container.isEmpty() && !( fallos_ && currentForm_ ) ) {
    QObject * cF = qApp->activeWindow();
    if ( !cF )
      cF = qApp->child( container );
    if ( cF ) {
      if ( cF->name() == container ) {
        if ( cF->isA( "FLFormRecordDB" ) )
          currentForm_ = ( FLFormRecordDB * ) cF;
        if ( cF->isA( "FLFormSearchDB" ) )
          currentForm_ = ( FLFormSearchDB * ) cF;
        if ( cF->isA( "FLFormDB" ) ) {
          if ( FLSqlCursor::inTransaction() ) {
#ifdef FL_DEBUG
            qWarning( "En transacción" );
#endif
            return REPEAT;
          }
          currentForm_ = ( FLFormDB * ) cF;
        }
      } else
        currentForm_ = 0;
    }
    if ( !currentForm_ ) {
      QWidgetList * list = QApplication::allWidgets();
      QWidgetListIt it( *list );
      QWidget * w;
      while (( w = it.current() ) != 0 ) {
        ++it;
        if ( w->name() == container ) {
          if ( w->isA( "FLFormRecordDB" ) )
            currentForm_ = ( FLFormRecordDB * ) w;
          if ( w->isA( "FLFormSearchDB" ) )
            currentForm_ = ( FLFormSearchDB * ) w;
          if ( w->isA( "FLFormDB" ) ) {
            if ( FLSqlCursor::inTransaction() ) {
#ifdef FL_DEBUG
              qWarning( "En transacción" );
#endif

              return REPEAT;
            }
            currentForm_ = ( FLFormDB * ) w;
          }

          if ( currentForm_ )
            break;
        } else
          currentForm_ = 0;
      }
      delete list;
    }
  }

  if ( idObjectType == "Test" ) {
    if ( !assignateDefaultValues( objectName ) ) {
      errorDescription_ = tr( "Error en la asignación de parámetros por defecto" );
      return TESTER_ERROR;
    }
    if ( !assignateValues( parameters ) ) {
      errorDescription_ = tr( "Error en la asignación de parámetros" );
      return TESTER_ERROR;
    }
    return NEXT;
  }

  if ( idObjectType == "ScriptInput" ) {
    QWidget * msg = qApp->activeWindow();

    if ( !msg )
      return REPEAT;

    if ( !msg->isA( "QInputDialog" ) )
      return REPEAT;

    if ( idAction == "setInputValue" ) {
      QWidget * w = msg->focusWidget();
      if ( !w )
        return REPEAT;
      if ( w->isA( "QLineEdit" ) )
        (( QLineEdit * ) w ) ->setText( parameters );
      else if ( w->isA( "QComboBox" ) )
        (( QComboBox * ) w ) ->setCurrentText( parameters );
      else if ( w->isA( "QSpinBox" ) ) {
        bool bOk = true;
        int i = parameters.toInt( &bOk );
        if ( !bOk ) {
          errorDescription_ = "Se intentó convertir el valor  " + parameters + " a entero";
          return TESTER_ERROR;
        }
        (( QSpinBox * ) w ) ->setValue( i );
      } else {
        errorDescription_ = "El control " + QString( w->name() ) + " no es de ninguna clase controlada";
        return TESTER_ERROR;
      }
      (( QButton * )( msg->child( "qt_ok_btn" ) ) ) ->animateClick();
    }
  }

  if ( idObjectType == "ScriptDialog" ) {
    QWidget * msg = qApp->activeWindow();

    if ( !msg )
      return REPEAT;
    if ( !msg->isA( "QDialog" ) )
      return REPEAT;

    if ( idAction == "fillDialog" ) {

      QDomDocument doc;
      if ( !doc.setContent( parameters ) )
        return ERR;

      setConditions( doc.firstChild().toElement(), MESSAGE_BOX_CLOSED, "MSG" );

      QDomNodeList nL = doc.elementsByTagName( "FLTest:DialogObject" );
      QDomElement e;
      QObject *o;
      for ( int i = 0; i < nL.count(); i++ ) {
        e = nL.item( i ).toElement();
        QString tipo = e.attribute( "type" );
        QString nombre = e.attribute( "name" );
        QVariant valor = e.attribute( "value" );
        if ( tipo == "QSLabeled" )
          tipo = "QLabel";
        QObjectList * oL = msg->queryList( tipo );
        QObjectListIt it( *oL );
        QObject *o;
        while (( o = it.current() ) != 0 ) {
          ++it;
          qWarning( QString( o->name() ) );
          if ( tipo == "QCheckBox" ) {
            if ( o->isA( "QRadioButton" ) && (( QCheckBox * ) o ) ->text() == tr( nombre ) ) {
              (( QCheckBox * ) o ) ->setChecked( valor.toBool() );
              break;
            }
          } else if ( tipo == "QRadioButton" ) {
            if ( o->isA( "QRadioButton" ) && (( QRadioButton * ) o ) ->text() == tr( nombre ) ) {
              (( QRadioButton * ) o ) ->setChecked( valor.toBool() );
              break;
            }
          } else if ( tipo == "QPushButton" ) {
            if ( o->isA( "QPushButton" ) && (( QPushButton * ) o ) ->text() == tr( nombre ) ) {
              (( QPushButton * ) o ) ->animateClick();
              break;
            }
          } else if ( tipo == "QLabel" ) {
            if ( o->isA( "QLabel" ) && (( QLabel * ) o ) ->text() == tr( nombre ) ) {
              QWidget * w = (( QLabel * ) o ) ->buddy();
              if ( w->isA( "QLineEdit" ) )
                (( QLineEdit * ) w ) ->setText( valor.toString() );
              else if ( w->isA( "QDateEdit" ) )
                (( QDateEdit * ) w ) ->setDate( valor.toDate() );
              else if ( w->isA( "QTimeEdit" ) )
                (( QTimeEdit * ) w ) ->setTime( valor.toTime() );
              else if ( w->isA( "QSpinBox" ) )
                (( QSpinBox * ) w ) ->setValue( valor.toInt() );
              else if ( w->isA( "QComboBox" ) )
                (( QComboBox * ) w ) ->setCurrentText( valor.toString() );
              break;
            }
            if ( o->isA( "QTextEdit" ) && o->name() == nombre ) {
              (( QTextEdit * ) o ) ->setText( valor.toString() );
              break;
            }
          }
        }
        delete oL;
        if ( !o ) {
          errorDescription_ = "No se encontró el objeto " + nombre + " de tipo " + tipo;
          return TESTER_ERROR;
        }
      }
    }
  }

  if ( idObjectType == "MessageBox" ) {
    if ( !parameters.isEmpty() ) {
      QDomDocument doc;
      if ( !doc.setContent( parameters ) )
        return ERR;
      setConditions( doc.firstChild().toElement(), MESSAGE_BOX_CLOSED, "MSG" );
    }

    QWidget *msg = qApp->activeWindow();

    if ( !msg )
      return REPEAT;

    if ( !msg->isA( "QMessageBox" ) )
      return REPEAT;

    QString buttonText;

    if ( idAction == "pushOk" ) {
      buttonText = QMessageBox::tr( "OK" );
    } else if ( idAction == "pushCancel" ) {
      buttonText = QMessageBox::tr( "Cancel" );
    } else if ( idAction == "pushYes" ) {
      buttonText = QMessageBox::tr( "&Yes" );
    } else if ( idAction == "pushNo" ) {
      buttonText = QMessageBox::tr( "&No" );
    } else if ( idAction == "pushAbort" ) {
      buttonText = QMessageBox::tr( "&Abort" );
    } else if ( idAction == "pushRetry" ) {
      buttonText = QMessageBox::tr( "&Retry" );
    } else if ( idAction == "pushIgnore" ) {
      buttonText = QMessageBox::tr( "&Ignore" );
    } else if ( idAction == "pushYesToAll" ) {
      buttonText = QMessageBox::tr( "Yes to &All" );
    } else if ( idAction == "pushNoToAll" ) {
      buttonText = QMessageBox::tr( "N&o to All" );
    }
    QObjectList * bL = msg->queryList( "QPushButton" );
    QObjectListIt it( *bL );
    QObject *boton;
    while (( boton = it.current() ) != 0 ) {
      ++it;
      if ((( QPushButton* ) boton ) ->text() == buttonText )
        break;
    }
    if ( boton )
      QTimer::singleShot( 0, boton, SLOT( animateClick() ) );
    delete bL;
  }

  if ( idObjectType == "FLFormDB" ) {
    if ( idAction == "open" ) {
      if ( FLSqlCursor::inTransaction() ) {
#ifdef FL_DEBUG
        qWarning( "En transacción open" );
#endif

        return REPEAT;
      }
      condition_ = FORM_READY;
      objectCondition_ = container;
      QPixmap icon;
      (( FLApplication * ) qApp ) ->openMasterForm( objectName, icon );
    } else if ( idAction == "close" ) {
      condition_ = FORM_CLOSED;
      objectCondition_ = container;
      if ( !currentForm_ ) {
#ifdef FL_DEBUG
        qWarning( "No hay formulario" );
#endif

        return REPEAT;
      }
      currentForm_->setActiveWindow();
      if ( !currentForm_->isActiveWindow() || !currentForm_->isEnabled() ) {
#ifdef FL_DEBUG
        qWarning( "No activo" );
#endif

        return REPEAT;
      }

      if ( FLSqlCursor::inTransaction() ) {
#ifdef FL_DEBUG
        qWarning( "En transacción close" );
#endif

        return REPEAT;
      }
      QTimer::singleShot( 0, currentForm_, SLOT( close() ) );
    } else if ( idAction == "setFieldValues" ) {
      FLTestFdb * fdb;
      if ( fdbQueue_->isEmpty() ) {
        if ( parameters == "**UNDEFINED**" )
          return NEXT;

        QDomDocument doc;
        if ( !doc.setContent( parameters ) ) {
          errorDescription_ = tr( "El formato XML de los parámetros el incorrectos" );
          return ERR;
        }
        QDomNodeList nL = doc.elementsByTagName( "FLTest:FieldDB" );
        int i = 0;
        for ( i = 0; i < nL.count(); i++ ) {
          fdb = new FLTestFdb();
          fdb->fdbName = nL.item( i ).toElement().attribute( "name" );
          fdb->fdbValue = nL.item( i ).toElement().attribute( "value" );
          fdbQueue_->enqueue( fdb );
        }
      }
      if ( fdbQueue_->isEmpty() )
        return NEXT;
      fdb = fdbQueue_->dequeue();
      idObjectType = "FLFieldDB";
      idAction = "setValue";
      objectName = fdb->fdbName;
      parameters = fdb->fdbValue;
      delete fdb;
    }
  }

  if ( idObjectType == "Button" ) {
    if ( !currentForm_ ) {
#ifdef FL_DEBUG
      qWarning( "No hay formulario" );
#endif

      return REPEAT;
    }
    currentForm_->setActiveWindow();
    if (( !currentForm_->isActiveWindow() || !currentForm_->isEnabled() ) && !currentForm_->isDialog() ) {
#ifdef FL_DEBUG
      qWarning( QString( currentForm_->name() ) + " No activa" );
#endif

      return REPEAT;
    }

    QButton *boton = ( QButton * ) currentForm_->child( objectName );
    if ( !boton ) {
      if ( objectName.contains( "Buttom" ) ) {
        QString hackName( objectName );
        boton = ( QButton * ) currentForm_->child( hackName.replace( "Buttom", "Button" ) );
      }
    }

    if ( !boton ) {
#ifdef FL_DEBUG
      qWarning( "No hay botón" );
#endif
      return REPEAT;
    }

    QStringList pL;

    if ( !parameters.isEmpty() ) {
      QDomDocument doc;
      if ( !doc.setContent( parameters ) ) {
        errorDescription_ = tr( "El formato XML de los parámetros el incorrectos" );
        return ERR;
      }
      setConditions( doc.firstChild().toElement(), FORM_CLOSED, container );
    } else {
      if ( objectName == "pushButtonAccept" ) {
        condition_ = FORM_CLOSED;
        objectCondition_ = container;
      } else if ( objectName == "pushButtonAcceptContinue" || objectName == "pushButtonFirst" || objectName == "pushButtonPrevious" || objectName == "pushButtonNext" || objectName == "pushButtonLast" ) {
        condition_ = FORM_RECORD_READY;
        objectCondition_ = container;
      } else {
        if ( !qryStep.exec( "SELECT idaction, objectname, idobjecttype, container, formtype, parameters, idmodule FROM tt_step INNER JOIN tt_stepcat ON tt_step.idstepcat = tt_stepcat.idstepcat WHERE idstep > " + idStep + " AND idobjecttype <> 'Test' ORDER BY idstep" ) ) {
          errorDescription_ = "error1";
          return ERR;
        }
        qryStep.first();

        QString nextObjectName = qryStep.value( 2 ).toString();
        QString nextContName = qryStep.value( 3 ).toString();
        QString nextFormType = qryStep.value( 4 ).toString();
        QString nextContainer = nextFormType + nextContName;

        if ( nextContainer != container ) {
          if ( nextContName.isEmpty() ) {
            if ( nextObjectName == "MessageBox" ) {
              condition_ = MESSAGE_BOX_READY;
              objectCondition_ = "MSG";
            }
          } else {
            if ( nextFormType == "formRecord" ) {
              condition_ = FORM_RECORD_READY;
              objectCondition_ = nextContainer;
            } else if ( nextFormType == "formSearch" ) {
              condition_ = FORM_SEARCH_READY;
              objectCondition_ = nextContainer;
            } else if ( nextFormType == "form" ) {
              condition_ = FORM_CLOSED;
              objectCondition_ = container;
            }
          }
        }
      }
    }
    if ( !boton->isEnabled() ) {
#ifdef FL_DEBUG
      qWarning( tr( "El botón " ) + objectName + tr( " del formulario " ) + container + tr( " está inhabilitado" ) );
#endif

      return REPEAT;
    }
    if ( idAction == "animateClick" ) {
      QWidget::FocusPolicy focusPol = boton->focusPolicy();
      boton->setFocusPolicy( QWidget::StrongFocus );
      QWidget * focusWidget = qApp->focusWidget();
      boton->setFocus( );

      if ( !boton->hasFocus() ) {
        boton->setFocusPolicy( focusPol );
        if ( focusWidget )
          focusWidget->setFocus();
#ifdef FL_DEBUG

        qWarning( "Botón sin foco" );
#endif

        return REPEAT;
      }

      boton->setFocusPolicy( focusPol );
      if ( focusWidget )
        focusWidget->setFocus();
      QTimer::singleShot( 0, boton, SLOT( animateClick() ) );
      if ( condition_ == NO_CONDITION )
        return NEXT;
    }
  }

  if ( idObjectType == "FLFieldDB" ) {

    if ( !currentForm_ ) {
#ifdef FL_DEBUG
      qWarning( "No hay formulario" );
#endif
      return REPEAT;
    }

    if (( !currentForm_->isActiveWindow() || !currentForm_->isEnabled() || !currentForm_->hasFocus() ) && !currentForm_->isDialog() ) {
#ifdef FL_DEBUG
      qWarning( "No activa" );
#endif

      return REPEAT;
    }

    FLFieldDB *campo = ( FLFieldDB * ) currentForm_->child( objectName );
    if ( !campo ) {
#ifdef FL_DEBUG
      qWarning( "No hay campo" );
#endif
      return REPEAT;
    }
    campo->show();
    if ( idAction == "setValue" ) {
      if ( parameters == "**UNDEFINED**" )
        return NEXT;

      bool iguales;
      if ( parameters.upper() == "FALSE" || parameters.upper() == "TRUE" ) {
        iguales = ( QVariant( parameters ) ).toBool() == ( campo->value() ).toBool();
      } else {
        if ( campo->value() == parameters ) {
          iguales = true;
          if ( campo->value() == "0" ) {
            if ( currentForm_->cursor()->bufferIsNull( campo->fieldName() ) )
              iguales = false;
          }
        } else {
          double d1 = parameters.toDouble( &iguales );
          if ( iguales ) {
            double d2 = ( campo->value() ).toDouble( &iguales );
            if ( iguales )
              iguales = ( d1 == d2 );
          }
        }
      }
      if ( iguales )
        return NEXT;

      condition_ = FIELD_READY;
      objectCondition_ = campo->fieldName();
      campo->setFocus();
      if ( parameters == "true" )
        campo->setValue( true );
      else if ( parameters == "false" )
        campo->setValue( false );
      else
        campo->setValue( parameters );

    } else if ( idAction == "searchValue" ) {
      campo->searchValue();
      return NEXT;
    } else if ( idAction == "saveValue" ) {
      QVariant valor = campo->value();
      if ( !setVar( parameters, valor.toString() ) ) {
        errorDescription_ = "Error al asignar el valor" + valor.toString() + " a la variable " + parameters;
        return TESTER_ERROR;
      }
      return NEXT;
    }
  }

  if ( idObjectType == "FLTableDB" ) {

    if ( !currentForm_ ) {
#ifdef FL_DEBUG
      qWarning( "No hay formulario" );
#endif

      return REPEAT;
    }
    currentForm_->setActiveWindow();
    if ( !currentForm_->isActiveWindow() || !currentForm_->isEnabled() ) {
#ifdef FL_DEBUG
      qWarning( "No activa" );
#endif

      return REPEAT;
    }
    FLTableDB *tabla = ( FLTableDB * ) currentForm_->child( objectName );
    if ( idAction == "selectRow" ) {
      if ( !tabla ) {
#ifdef FL_DEBUG
        qWarning( "No hay tabla" );
#endif
        return REPEAT;
      }
      FLSqlCursor * c = tabla->cursor();
      if ( !c ) {
#ifdef FL_DEBUG
        qWarning( "No hay cursor" );
#endif
        return REPEAT;
      }
      if ( !c->isActive() ) {
#ifdef FL_DEBUG
        qWarning( "Cursor no activo" );
#endif
        return REPEAT;
      }
      if ( parameters == "next" )
        c->next();
      else
        if ( parameters == "prev" )
          c->prev();
        else
          if ( parameters == "first" )
            c->first();
          else
            if ( parameters == "last" )
              c->last();
            else {
              QString previousFilter = c->mainFilter();
              parameters.replace( "\"", "'" );
              if ( previousFilter.isEmpty() )
                c->setMainFilter( parameters );
              else
                c->setMainFilter( previousFilter + " AND (" + parameters + ")" );
              tabla->refresh( false, true );
              if ( c->size() == 0 ) {
                errorDescription_ = "El filtro " + ( c->mainFilter() ).replace( "'", "\"" ) + " no contiene registros";
                c->setMainFilter( previousFilter );
                return TESTER_ERROR;
              }
              tabla->tableRecords()->selectRow( 1 );
              c->setMainFilter( previousFilter );
            }
      return NEXT;
    }
  }

  if ( idObjectType == "Query" ) {
    QDomDocument doc;
    if ( !doc.setContent( parameters ) )
      return ERR;

    QVariant v;
    int retorno = getQueryValue( doc.firstChild(), v );
    if ( retorno != OK )
      return retorno;

    return NEXT;
  }

  if ( idObjectType == "Script" ) {
    QValueList<QVariant> vL;
    if ( !parameters.isEmpty() ) {}
    if ( FLSqlCursor::inTransaction() ) {
#ifdef FL_DEBUG
      qWarning( "En transacción" );
#endif

      return REPEAT;
    }

    (( FLApplication * ) qApp ) ->call( objectName, QSArgumentList( vL ), container );
    return NEXT;
  }

  if ( idObjectType == "Compare" ) {
    QDomDocument doc;
    if ( !doc.setContent( parameters ) )
      return ERR;

    QDomNodeList nL = doc.elementsByTagName( "FLTest:CompareElement" );
    QVariant v1;

    int retorno;
    retorno = getValue( nL.item( 0 ), v1 );
    if ( retorno != OK )
      return retorno;
    QString valor1 = v1.toString();
    QVariant v2;
    retorno = getValue( nL.item( 1 ), v2 );
    if ( retorno != OK )
      return retorno;
    QString valor2 = v2.toString();

    bool ok;
    double d1, d2;
    if ( valor1 == valor2 )
      d1 = d2;
    else {
      d1 = valor1.toDouble( &ok );
      if ( ok ) {
        d2 = valor2.toDouble( &ok );
      }
      if ( !ok ) {
        d1 = 1;
        d2 = 2;
      }
    }

    if ( d1 != d2 && ( valor1.upper() == "FALSE" || valor1.upper() == "TRUE" || valor2.upper() == "FALSE" || valor2.upper() == "TRUE" ) ) {
      d1 = v1.toBool();
      d2 = v2.toBool();
    }

    QString tipoComp = doc.namedItem( "FLTest:Compare" ).toElement().attribute( "type" );
    QString accion = doc.namedItem( "FLTest:Compare" ).toElement().attribute( "action" );
    ok = false;
    if (( tipoComp.lower() == tr( "iguales" ) &&  d1 == d2 ) || ( tipoComp.lower() == tr( "distintos" ) && d1 != d2 ) || ( tipoComp.lower() == tr( "mayor que" ) && d1 > d2 ) || ( tipoComp.lower() == tr( "menor que" ) && d1 < d2 ) )
      ok = true;
    if ( !ok ) {
      if ( accion.lower() == "error" ) {
        errorDescription_ = "La condición " + valor1 + " " + tipoComp + " " + valor2 + " no se cumple";
        return TESTER_ERROR;
      } else if ( accion.lower() == "saltar" ) {
        return SKIP_TEST;
      }
    }
    return NEXT;
  }

  if ( idObjectType == "Assignation" ) {
    if ( !assignateValues( parameters ) )
      return TESTER_ERROR;
    else
      return NEXT;
  }

  if ( idObjectType == "QTabWidget" ) {

    if ( !currentForm_ ) {
#ifdef FL_DEBUG
      qWarning( "No hay formulario" );
#endif
      return REPEAT;
    }
    currentForm_->setActiveWindow();
    if ( !currentForm_->isActiveWindow() || !currentForm_->isEnabled() ) {
#ifdef FL_DEBUG
      qWarning( "No activa" );
#endif
      return REPEAT;
    }
    QTabWidget *tabW = ( QTabWidget * ) currentForm_->child( objectName );
    if ( idAction == "setPage" ) {
      if ( !tabW ) {
#ifdef FL_DEBUG
        qWarning( "No hay QTabWidget" );
#endif
        return REPEAT;
      }
      if ( !processParameters( parameters ) )
        return TESTER_ERROR;

      QWidget * w = 0;
      for ( int i = 0; i < tabW->count(); i++ ) {
        w = tabW->page( i );
        if ( w->name() == parameters ) {
          tabW->setCurrentPage( i );
          return NEXT;
        }
      }
      errorDescription_ = tr( "La pestaña %1 no pertenece al Tab %2" ).arg( parameters, objectName );
      return TESTER_ERROR;
    }
  }

  if ( idObjectType == "Table" ) {
    if ( idAction == "deleteRecords" ) {
      FLSqlCursor * c = new FLSqlCursor( objectName );
      if ( !c ) {
        errorDescription_ = tr( "No pudo crearse el cursor para la tabla " ) + objectName;
        return ERR;
      }
      c->setForwardOnly( true );
      c->select( parameters );
      while ( c->next() ) {
        c->setModeAccess( FLSqlCursor::DEL );
        c->refreshBuffer();
        c->commitBuffer();
      }
      delete c;
    }
  }

  return OK;
}

bool FLTester::assignateValues( const QString &parameters ) {
  if ( parameters.isEmpty() )
    return true;
  QDomDocument doc;
  if ( !doc.setContent( parameters ) )
    return false;

  QDomNodeList nL = doc.elementsByTagName( "FLTest:AssignationElement" );
  QDomElement e;
  for ( int i = 0; i < nL.count(); i++ ) {
    e = nL.item( i ).toElement();
    QString value = e.attribute( "value" );
    QString variable = e.attribute( "var" );
    if ( value.isEmpty() )
      continue;
    if ( value == "**UNDEFINED**" ) {
      if ( !delVar( variable ) ) {
        errorDescription_ = "Error al borrar la variable " + variable;
        return false;
      }
    } else if ( value == "__child_node__" ) {
      QDomDocument dAux;
      QDomElement eAux = dAux.createElement( "x" );
      if ( e.text() == "**UNDEFINED**" ) {
        if ( !delVar( variable ) ) {
          errorDescription_ = "Error al borrar la variable " + variable;
          return false;
        }
      } else {
        eAux = e.firstChild().cloneNode().toElement();
        dAux.appendChild( eAux );
        if ( !setVar( variable, dAux.toString( 4 ) ) ) {
          errorDescription_ = "Error al asignar el valor" + value + " a la variable " + variable;
          return false;
        }
      }
    } else {
      if ( !setVar( variable, value ) ) {
        errorDescription_ = "Error al asignar el valor" + value + " a la variable " + variable;
        return false;
      }
    }
  }
  return true;
}

bool FLTester::assignateDefaultValues( const QString & testName ) {
  QDomDocument xmlTest;
  if ( !xmlTest.setContent( testName ) )
    return false;

  QDomNode xmlParent = xmlTest.firstChild();
  if ( xmlParent.isNull() )
    return false;

  QSqlQuery qry( QString::null, dbTest_->db() );

  qry.setForwardOnly( true );
  if ( !qry.exec( "SELECT parameters FROM tt_testcat WHERE codtestcat = '" + xmlParent.toElement().attribute( "codtest" ) + "' AND idfuncional = '" + xmlParent.toElement().attribute( "idfuncional" ) + "'" ) )
    return false;

  if ( !qry.next() )
    return false;

  QString parameters = qry.value( 0 ).toString();
  if ( parameters.isEmpty() )
    return true;

  if ( !processParameters( parameters ) )
    return false;

  if ( !xmlTest.setContent( parameters ) )
    return false;

  QDomNodeList listaParam = xmlTest.elementsByTagName( "FLTest:Parameter" );
  QDomElement e;
  QVariant value;
  bool exists;
  for ( int i = 0; i < listaParam.count(); i++ ) {
    e = listaParam.item( i ).toElement();
    QString name = e.attribute( "name" );
    QString defValue = e.attribute( "default" );
    if ( defValue.isEmpty() ) {
      if ( !delVar( name ) )
        return false;
    } else {
      if ( !setVar( name, defValue ) )
        return false;
    }
  }
  return true;
}

void FLTester::setConditions( QDomElement e, const Condition & defCondition, const QString & defObjectCondition ) {
  objectCondition_ = e.attribute( "object" );
  QString condition = e.attribute( "event" );
  if ( objectCondition_.isEmpty() )
    objectCondition_ = "ANY";

  if ( condition == "NO_CONDITION" )
    condition_ = NO_CONDITION;
  else if ( condition == "FORM_READY" )
    condition_ = FORM_READY;
  else if ( condition == "FORM_CLOSED" )
    condition_ = FORM_CLOSED;
  else if ( condition == "FORM_RECORD_READY" )
    condition_ = FORM_RECORD_READY;
  else if ( condition == "FORM_SEARCH_READY" )
    condition_ = FORM_SEARCH_READY;
  else if ( condition == "FIELD_READY" )
    condition_ = FIELD_READY;
  else if ( condition == "MESSAGE_BOX_READY" )
    condition_ = MESSAGE_BOX_READY;
  else if ( condition == "MESSAGE_BOX_CLOSED" )
    condition_ = MESSAGE_BOX_CLOSED;
  else if ( condition == "RECORD_DELETED" )
    condition_ = RECORD_DELETED;
  else if ( condition == "SCRIPT_FINISHED" )
    condition_ = SCRIPT_FINISHED;
  else if ( condition == "NONE_IS_VALID" )
    condition_ = NONE_IS_VALID;
  else {
    condition_ = defCondition;
    objectCondition_ = defObjectCondition;
  }
}


void FLTester::setTestStatus( QString status ) {
  QSqlCursor *c = new QSqlCursor( "tt_test", true, dbTest_->db() );
  if ( !c )
    return ;
  c->setForwardOnly( true );
  c->select( "idtest = '" + currentTest_ + "'" );
  if ( c->next() ) {
    if ( currentSession_ == 0 )
      currentSession_ = c->value( "idsession" ).toInt();

    QSqlRecord *buffer = c->primeUpdate();
    buffer->setValue( "status", status );
    buffer->setValue( "errordescription", errorDescription_ );
    c->update();
  }
  delete c;
  previousTest_ = currentTest_;
}

void FLTester::setSessionStatus( QString status ) {
  QString iS;
  QTime sessionEnd = QTime::currentTime();
  sessionStatus_ = status;

  QSqlCursor *c = new QSqlCursor( "tt_session", true, dbTest_->db() );
  if ( !c )
    return ;
  c->setForwardOnly( true );
  c->select( "idsession = '" + iS.setNum( currentSession_ ) + "'" );
  if ( c->next() ) {
    QSqlRecord * buffer = c->primeUpdate();
    buffer->setValue( "status", status );
    buffer->setValue( "sessionend", sessionEnd.toString( Qt::ISODate ) );
    c->update();
  }
  delete c;
}

int FLTester::getValue( QDomNode n, QVariant &v ) {
  QString tipo = n.toElement().attribute( "type" );
  int retorno;
  QVariant valor;
  if ( tipo.lower() == "query" ) {
    retorno = getQueryValue( n.firstChild(), v );
    if ( retorno != OK )
      return retorno;
  }

  if ( tipo.lower() == "valor" ) {
    v = n.toElement().text();
  }

  if ( tipo.lower() == "campo" ) {
    if ( !currentForm_ ) {
#ifdef FL_DEBUG
      qWarning( "No hay formulario" );
#endif

      return REPEAT;
    }
    if ( !currentForm_->isActiveWindow() ) {
#ifdef FL_DEBUG
      qWarning( "No activa" );
#endif

      return REPEAT;
    }
    QWidget *w = (( QWidget* ) currentForm_->child( n.toElement().text() ) );
    if ( !w ) {
#ifdef FL_DEBUG
      qWarning( "No hay campo" );
#endif

      return REPEAT;
    }
    if ( w->isA( "FLFieldDB" ) )
      v = (( FLFieldDB * ) w ) ->value();
    else if ( w->isA( "QLineEdit" ) )
      v = (( QLineEdit * ) w ) ->text();
    else if ( w->isA( "QLabel" ) )
      v = (( QLabel * ) w ) ->text();
  }
  return OK;
}

int FLTester::getQueryValue( QDomNode n, QVariant &v ) {
  QString where = n.toElement().attribute( "where" );
  QString from = n.toElement().attribute( "from" );
  where.replace( "\"", "'" );

  QSqlQuery q( QString::null, FLSqlConnections::database()->dbAux() );
  q.setForwardOnly( true );

  if ( from.find( " " ) == -1 && !FLSqlConnections::database()->manager()->existsTable( from ) ) {
    v = QVariant( "**NOT_FOUND**" );
  } else {
    if ( !q.exec( "SELECT " + n.toElement().attribute( "select" ) + " FROM " + from + " WHERE " + where ) ) {
      return ERR;
    }
    if ( !q.next() )
      v = QVariant( "**NOT_FOUND**" );
    else
      v = q.value( 0 );
  }

  QString saveIn = n.toElement().attribute( "savein" );
  if ( !saveIn.isEmpty() ) {
    setVar( saveIn, v );
  }
  return OK;
}

QString FLTester::sessionsList( const QString &bd ) {
  openTestDB( bd );
  QString res;
  QSqlQuery q( "SELECT idfuncional, codsessioncat, description FROM tt_sessioncat", dbTest_->db() );
  q.setForwardOnly( true );

  if ( !q.exec() ) {
    return "";
  }
  while ( q.next() ) {
    if ( !res.isEmpty() )
      res += "**";
    res += q.value( 0 ).toString() + "//" + q.value( 1 ).toString() + "//" + q.value( 2 ).toString();
  }
  return res;
}

bool FLTester::processParameters( QString &parameters ) {
  QString iniVar = "||";
  QString finVar = "||";

  if ( !parameters.isEmpty() ) {
    int posInicio = parameters.find( iniVar );
    int posFin = 0;
    QString var;
    QVariant valor;
    while ( posInicio > -1 ) {
      posFin = parameters.find( finVar, posInicio + 1 );
      if ( posFin == -1 )
        return false;
      var = parameters.mid( posInicio + 2, posFin - posInicio - 2 );
      if ( var == "_S" ) {
        valor = QString::number( currentSession_ );
      } else if ( var == "_T" ) {
        valor = currentTest_;
      } else if ( var == "_YEAR" ) {
        valor = QString::number(( QDate::currentDate() ).year() );
      } else if ( var == "_TODAY" ) {
        valor = ( QDate::currentDate() ).toString( Qt::ISODate );
      } else {
        bool exists;
        valor = getVar( var, &exists );
        if ( !exists )
          valor = QVariant( "**UNDEFINED**" );
      }
      parameters.replace( iniVar + var + finVar, valor.toString() );
      posInicio = parameters.find( iniVar, posInicio + 2 );
    }
  }
  return true;
}

uint FLTester::createSession( const QString &codSessionCat, const QString &idFuncional ) {
  QSqlQuery q( QString::null, dbTest_->db() );
  if ( !q.exec( "DELETE FROM tt_step" ) )
    return 0;

  if ( !q.exec( "DELETE FROM tt_test" ) )
    return 0;

  uint times;
  uint idTest;
  uint idSession;
  QString idSessionCat;
  QString idTestCat;
  QString idTCatSCat;

  QDate sessionDate = QDate::currentDate();
  QTime sessionStart = QTime::currentTime();
  if ( idFuncional.isNull() ) {
    if ( !q.exec( "SELECT id FROM tt_sessioncat WHERE codsessioncat = '" + codSessionCat + "' AND idfuncional IS NULL;" ) )
      return 0;
  } else {
    if ( !q.exec( "SELECT id FROM tt_sessioncat WHERE codsessioncat = '" + codSessionCat + "' AND idfuncional = '" + idFuncional + "';" ) )
      return 0;
  }

  if ( !q.next() )
    return 0;
  idSessionCat = q.value( 0 ).toString();
  FLSqlDriver * dr = ( FLSqlDriver * ) dbTest_->db()->driver();
  idSession = dr->nextSerialVal( "tt_session", "idsession" ).toUInt();

  QSqlCursor curSession( "tt_session", true, dbTest_->db() );
  QSqlRecord *buffer = curSession.primeInsert();
  buffer->setValue( "idsession", idSession );
  buffer->setValue( "idsessioncat", idSessionCat );
  buffer->setValue( "status", "Pendant" );
  buffer->setValue( "sessiondate", sessionDate.toString( Qt::ISODate ) );
  buffer->setValue( "sessionstart", sessionStart.toString( Qt::ISODate ) );
  if ( !curSession.insert() )
    return 0;

  if ( !q.exec( "SELECT idsession FROM tt_session WHERE 1 = 1 ORDER BY idsession DESC" ) )
    return 0;

  if ( !q.first() )
    return 0;

  idSession = q.value( 0 ).toUInt();

  if ( !q.exec( "SELECT idtestcat, repeat, idtcatscat FROM tt_tcatscat WHERE idsessioncat = '" + idSessionCat + "' ORDER BY testnumber" ) )
    return 0;

  QProgressDialog progress( tr( "Generando guión de pruebas" ), 0, q.size() );
  progress.setCaption( tr( "Inicializando" ) );
  uint step = 0;

  while ( q.next() ) {
    times = ( q.value( 1 ).toUInt() );

    QSqlCursor curTest( "tt_test", true, dbTest_->db() );
    for ( int i = 0; i < times; i++ ) {
      idTestCat = q.value( 0 ).toString();
      idTest = dr->nextSerialVal( "tt_test", "idtest" ).toUInt();
      idTCatSCat = q.value( 2 ).toString();

      QSqlRecord *buffer = curTest.primeInsert();
      buffer->setValue( "idtest", idTest );
      buffer->setValue( "idsession", idSession );
      buffer->setValue( "idtestcat", idTestCat );
      buffer->setValue( "idtcatscat", idTCatSCat );
      buffer->setValue( "status", "Pendant" );
      if ( !curTest.insert() )
        return 0;

      if ( !addTest( idTest, idTestCat, 0 ) )
        return 0;

      progress.setProgress( ++step );
    }
  }
  return idSession;
}

bool FLTester::addTest( const uint &idTest, const QString idTestCat, uint testLevel ) {
  QSqlCursor cursor( "tt_step", true, dbTest_->db() );
  QSqlQuery qryStepCat( QString::null, dbTest_->db() );

  qryStepCat.setForwardOnly( true );
  if ( !qryStepCat.exec( "SELECT idstepcat, idobjecttype, objectname FROM tt_stepcat WHERE idtestcat = '" + idTestCat + "' ORDER BY stepnumber" ) )
    return false;

  bool firstStep = true;
  FLSqlDriver * dr = ( FLSqlDriver * ) dbTest_->db()->driver();
  while ( qryStepCat.next() ) {
    QSqlRecord * buffer = cursor.primeInsert();
    buffer->setValue( "idstep", dr->nextSerialVal( "tt_step", "idstep" ).toUInt() );
    buffer->setValue( "idtest", idTest );
    buffer->setValue( "idstepcat", qryStepCat.value( 0 ).toString() );
    buffer->setValue( "status", "Pendant" );
    buffer->setValue( "testlevel", testLevel );
    buffer->setValue( "firststep", firstStep );
    if ( !cursor.insert() )
      return false;

    firstStep = false;

    if ( qryStepCat.value( 1 ) == "Test" ) {
      QDomDocument doc;
      if ( !doc.setContent( qryStepCat.value( 2 ).toString() ) )
        return false;

      QDomNode n = doc.namedItem( "FLTest:Test" );
      if ( n.isNull() )
        return false;
      QDomElement e = n.toElement();
      QSqlQuery qryTest( QString::null, dbTest_->db() );
      qryTest.setForwardOnly( true );
      if ( !qryTest.exec( "SELECT id FROM tt_testcat WHERE codtestcat = '" + e.attribute( "codtest" ) + "' AND idfuncional = '" + e.attribute( "idfuncional" ) + "'" ) )
        return false;
      if ( !qryTest.next() ) {
        qWarning( tr( "No se encontró la prueba " ) + e.attribute( "codtest" ) + " / " + e.attribute( "idfuncional" ) );
        return false;
      }
      addTest( idTest, qryTest.value( 0 ).toString(), ( testLevel + 1 ) );
    }
  }
  return true;
}

void FLTester::openTestDB( const QString &bd ) {
  FLSqlDatabase * dbCur = FLSqlConnections::database();
  QString driverAlias = FLSqlDatabase::driverNameToDriverAlias( dbCur->driverName() );

  if ( !FLSqlConnections::addDatabase( driverAlias, bd, dbCur->user(), dbCur->password(), dbCur->host(), dbCur->port(), "facturaluxTest" ) )  return;

  dbTest_ = FLSqlConnections::database( "facturaluxTest" );
}

void FLTester::enqueueTesterEvent( const int c, const QString & oC ) {
  if ( !queueTesterEvents ) {
    queueTesterEvents = new QPtrQueue < FLTesterEvent >;
    queueTesterEvents->setAutoDelete( true );
  }

  FLTesterEvent * testerEvent = new FLTesterEvent();
  testerEvent->condition = c;
  testerEvent->objectCondition = oC;

  queueTesterEvents->enqueue( testerEvent );

  QTimer::singleShot( DELAY / 4, this, SLOT( tryContinue() ) );
}

bool FLTester::setVar( const QString &name, const QVariant &value ) {
  QString idSesion = (( FLApplication * ) qApp ) ->timeUser().toString( Qt::ISODate );
  QString where = "idvar = '" + name + "' AND idsesion = '" + idSesion + "'";

  QSqlQuery qry( QString::null, dbTest_->db() );

  qry.setForwardOnly( true );
  qry.exec( "SELECT idvar FROM flvar WHERE " + where );
  if ( qry.next() ) {
    if ( !execTransaction( "UPDATE flvar SET valor = '" + value.toString() + "' WHERE " + where ) )
      return false;
  } else {
    FLSqlDriver * dr = ( FLSqlDriver * ) dbTest_->db()->driver();
    if ( dbTest_->db()->transaction() ) {
      QSqlCursor cursor( "flvar", true, dbTest_->db() );
      QSqlRecord * buffer = cursor.primeInsert();
      buffer->setValue( "id", dr->nextSerialVal( "flvar", "id" ).toUInt() );
      buffer->setValue( "idvar", name );
      buffer->setValue( "idsesion", idSesion );
      buffer->setValue( "valor", value );
      if ( cursor.insert() ) {
        dbTest_->db()->commit();
      } else {
        dbTest_->db()->rollback();
        return false;
      }
    }
  }
  return true;
}

QVariant FLTester::getVar( const QString &name, bool *exists ) {
  QString idSesion = (( FLApplication * ) qApp ) ->timeUser().toString( Qt::ISODate );
  QString where = "idvar = '" + name + "' AND idsesion = '" + idSesion + "'";

  QSqlQuery qry( QString::null, dbTest_->db() );
  QVariant res;

  qry.setForwardOnly( true );
  qry.exec( "SELECT valor FROM flvar WHERE " + where );
  if ( qry.next() ) {
    *exists = true;
    res = qry.value( 0 );
  } else
    *exists = false;

  return res;
}

bool FLTester::delVar( const QString &name ) {
  QString idSesion = (( FLApplication * ) qApp ) ->timeUser().toString( Qt::ISODate );
  QString where = "idvar = '" + name + "' AND idsesion = '" + idSesion + "'";

  return execTransaction( "DELETE FROM flvar WHERE " + where );
}

FLEventLoop::FLEventLoop() : QEventLoop() {}

bool FLEventLoop::processEvents( ProcessEventsFlags flags ) {
  bool b = QEventLoop::processEvents( flags );
#ifdef FL_TEST

  if ( aboutToEnter_ ) {
    aboutToEnter_ = false;
    FLTester *t = (( FLApplication * ) qApp ) ->tester();
    if ( t && t->testing() ) {
      QWidget * w = qApp->activeModalWidget();
      if ( w )
        t->enqueueTesterEvent( FLTester::NO_CONDITION, "VOID" );
      else
        qWarning( "No hay ventana activa" );
    }
  }
#endif
  return b;
}

int FLEventLoop::enterLoop() {
  aboutToEnter_ = true;
  return QEventLoop::enterLoop();
}

void FLEventLoop::exitLoop() {
  QEventLoop::exitLoop();
}
