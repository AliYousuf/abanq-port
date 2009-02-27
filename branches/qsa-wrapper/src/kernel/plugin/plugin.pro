TEMPLATE	= lib
CONFIG		+= qt warn_on plugin
TARGET 		= quick
VERSION  	= 1.0.0
DESTDIR		= $(QTDIR)/plugins/designer
INCLUDEPATH 	+= .. $(QTDIR)/tools/designer/interfaces ../../3rdparty ../../sdk
SOURCES 	+= quickcore.cpp
HEADERS 	+= quickcore.h

unix:LIBS	+= -L$(QTDIR)/lib -lquickkernel
win32:LIBS	+= $(QTDIR)\lib\quickkernel.lib $(QTDIR)\lib\quicksdk.lib
#The following line was inserted by qt3to4
QT +=  qt3support 
