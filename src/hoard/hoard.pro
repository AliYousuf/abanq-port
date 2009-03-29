exists(../../settings.pro) {
        include(../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = lib
CONFIG += warn_off exceptions
CONFIG -= x11 qt
DEFINES +=  NDEBUG _REENTRANT=1 __GNUC__ unix
DESTDIR = $$QTDIR/lib
TARGET = hoard
INCLUDEPATH += /usr/include/nptl . $$ROOT/src/hoard/src $$ROOT/src/hoard/src/heaplayers $$ROOT/src/hoard/src/heaplayers/util
unix:LIBS = -ldl -lpthread

win32 {
        CONFIG += dll
        DLLDESTDIR = $$PREFIX/bin
	LIBS = -lpthreadGC2 -lm
}

VERSION = 3.7

linux-g++:QMAKE_CXXFLAGS = -pthread -malign-double -pipe -O3 -finline-limit=20000 -fomit-frame-pointer -finline-functions
linux-g++-64:QMAKE_CXXFLAGS = -pthread -pipe -O3 -finline-limit=20000 -fomit-frame-pointer -finline-functions
win32:QMAKE_CXXFLAGS = -malign-double -pipe -O3 -finline-limit=20000 -fomit-frame-pointer -finline-functions
QMAKE_LFLAGS += -Bsymbolic

SOURCES = ./src/libhoard.cpp
