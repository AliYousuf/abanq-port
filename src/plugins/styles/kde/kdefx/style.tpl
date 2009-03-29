exists(../../../../../settings.pro) {
        include($$PWD/../../../../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = lib
CONFIG += warn_off qt plugin
DESTDIR = $$PREFIX/plugins/styles
TARGET 			= k$${STYLE_NAME}style

#message ( ******************************** )
#message ( BUILDING $${STYLE_NAME} style    )
#message ( ******************************** )

#!rtti:error( This style need QT to be built with RTTI! )

#contains( KDEFX_BUILT_IN , yes ) {

#message( KDEFX built it )

#include($$PWD/kdefx.pri)

#} else {

#message( KDEFX as dll )

INCLUDEPATH 		+= $$PWD
INCLUDEPATH 		+= $$PWD/include

#QMAKE_LIBDIR 		+= $$PWD/lib

win32-msvc:LIBS	 	+= kdefxx.lib
win32-g++:LIBS		+= -lkdefxx
unix:LIBS		+= -L$$PREFIX/lib -lkdefxx

#}

#message( Attempt to include the config tool... )
#include( config/config.pri )

INCLUDEPATH 		+= .
INCLUDEPATH 		+= ../src

DEFINES 		+= MAKE_KSTYLE_LIB

MOC_DIR           	= ../src/moc
OBJECTS_DIR       	= ../src/obj
UI_HEADERS_DIR 		= ../src/ui
UI_SOURCES_DIR 		= ../src/ui

#DESTDIR    		= ../lib
#DLLDESTDIR		= $(QTDIR)/plugins/styles