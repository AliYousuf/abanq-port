exists(../../settings.pro) {
	include(../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = lib

CONFIG += warn_off create_prl link_prl qt
unix:CONFIG += x11

win32 {
	CONFIG += dll
	DLLDESTDIR = $$PREFIX/bin
}
DESTDIR = $$PREFIX/lib

TARGET = flbase

INCLUDEPATH += $$ROOT $$ROOT/src/flbase $$ROOT/src/lrelease $$ROOT/src/barcode \
	$$ROOT/src/kugar $$ROOT/src/advance $$ROOT/src/posprinter $$ROOT/src/flmail \
	$$ROOT/src/serialport $$ROOT/src/qwt/src
DEPENDPATH += $$ROOT/src/lrelease $$ROOT/src/barcode $$ROOT/src/kugar $$ROOT/src/advance $$ROOT/src/flmail

LIBS += -L$$PREFIX/lib -lqui -lbarcode -lkugar -lqwt -ladvance -lflmail -llrelease
load(qsa)

!contains(DEBIAN, true) {
	QMAKE_RPATH = -Wl,-rpath,$$PREFIX/lib,-rpath,
}

VERSION = 2.3

include(../serialport/serialport.pri)

SOURCES += FLAbout.cpp \
           FLApplication.cpp \
           FLCompoundKey.cpp \
           FLDataTable.cpp \
           FLFieldMetaData.cpp \
           FLFormDB.cpp \
           FLFormRecordDB.cpp \
           FLFormSearchDB.cpp \
           FLGroupByQuery.cpp \
           FLManager.cpp \
           FLParameterQuery.cpp \
           FLRelationMetaData.cpp \
           FLReportEngine.cpp \
           FLReportViewer.cpp \
           FLConnectDBDialog.cpp \
           FLSqlCursor.cpp \
           FLSqlQuery.cpp \
           FLTableMetaData.cpp \
           FLUtil.cpp \
           FLObjectFactory.cpp \
           FLFieldDB.cpp \
           FLTableDB.cpp \
           FLHelpWindow.cpp \
           FLWidgetAction.cpp \
           FLCodBar.cpp \
           FLMemCache.cpp \
           FLTranslations.cpp \
           FLTranslator.cpp \
           FLManagerModules.cpp \
           FLScriptEditor.cpp \
           FLTester.cpp \
           FLVar.cpp \
           FLSha1.cpp \
           FLDiskCache.cpp \
           FLSqlSavePoint.cpp \
	   FLAccessControl.cpp \
	   FLAccessControlLists.cpp \
	   FLAccessControlFactory.cpp \
	   FLSqlDatabase.cpp \
	   FLSqlDriver.cpp \
	   FLSqlConnections.cpp \
	   FLSmtpClient.cpp \
	   FLImageViewer.cpp \
	   FLPicture.cpp \
	   FLNetwork.cpp \
           FLStylePainter.cpp \
           FLFiscalBixolon.cpp

HEADERS += FLAbout.h \
           FLAction.h \
           FLApplication.h \
           FLCompoundKey.h \
           FLDataTable.h \
           FLFieldMetaData.h \
           FLFormDB.h \
           FLFormRecordDB.h \
           FLFormSearchDB.h \
           FLGroupByQuery.h \
           FLManager.h \
           FLParameterQuery.h \
           FLRelationMetaData.h \
           FLReportEngine.h \
           FLReportViewer.h \
           FLConnectDBDialog.h \
           FLSqlCursor.h \
           FLSqlQuery.h \
           FLTableMetaData.h \
           FLUtil.h \
           FLObjectFactory.h \
           FLFieldDB.h \
           FLTableDB.h \
           FLHelpWindow.h \
           FLWidgetAction.h \
           FLCodBar.h \
           FLMemCache.h \
           FLTranslations.h \
           FLTranslator.h \
           FLManagerModules.h \
           FLScriptEditor.h \
           FLTester.h \
           FLVar.h \
           FLSha1.h \
           FLDiskCache.h \
           FLSqlSavePoint.h \
	   FLAccessControl.h \
	   FLAccessControlLists.h \
	   FLAccessControlFactory.h \
	   FLSqlDatabase.h \
	   FLSqlDriver.h \
	   FLSqlConnections.h \
	   FLSmtpClient.h \
	   FLImageViewer.h \
	   FLPicture.h \
	   FLNetwork.h \
           FLStylePainter.h \
           FLFiscalBixolon.h

FORMS += FLWidgetAbout.ui \
         FLWidgetConnectDB.ui \
         FLWidgetReportViewer.ui \
         FLWidgetFieldDB.ui \
         FLWidgetTableDB.ui \
	 FLWidgetScriptEditor.ui

IMAGES += images/infosial.png \ 
        images/about.png \
	images/cascada.png \
	images/exit.png \
	images/font.png \
	images/last.png \
	images/ok.png \
	images/remove.png \
	images/aboutqt.png \
	images/cerrar.png \
	images/first.png \
	images/help_index.png \
	images/mosaico.png \
	images/previous.png \
	images/button_ok.png \
	images/button_cancel.png \
	images/estilo.png \
	images/home.png \
	images/next.png \
	images/reload.png \
	images/date.png \
	images/bomba.png \
	images/filesaveas.png \
	images/logo_abanq.png \
	images/mini_logo_abanq.png \
	images/icono_abanq.png \
	images/bug.png \
	images/unlock.png \
	images/lock.png \
	images/file_open.png \
	images/eraser.png \
	images/locked.png

