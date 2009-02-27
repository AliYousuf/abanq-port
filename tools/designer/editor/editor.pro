TEMPLATE	= lib
CONFIG		+= qt warn_on staticlib
CONFIG		-= dll
HEADERS		= editor.h  \
		  parenmatcher.h  \
		  completion.h \
		  viewmanager.h \
		  markerwidget.h\
		  conf.h \
		  browser.h \
		  arghintwidget.h \
		  cindent.h

SOURCES		= editor.cpp \
		  parenmatcher.cpp  \
		  completion.cpp \
		  viewmanager.cpp \
		  markerwidget.cpp \
		  conf.cpp \
		  browser.cpp \
		  arghintwidget.cpp \
		  cindent.cpp \
		  yyindent.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3		= preferences.ui

TARGET		= qsaeditor
DESTDIR		= ../../../lib
VERSION		= 1.0.0

INCLUDEPATH	+= $$QT_SOURCE_TREE/tools/designer/interfaces

target.path=$$libs.path

INSTALLS += target

unix {
	QMAKE_CFLAGS += $$QMAKE_CFLAGS_SHLIB
	QMAKE_CXXFLAGS += $$QMAKE_CXXFLAGS_SHLIB
}
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

