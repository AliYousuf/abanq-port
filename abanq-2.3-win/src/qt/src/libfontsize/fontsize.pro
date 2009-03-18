exists(../../../../settings.pro) {
	include(../../../../settings.pro)
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

TARGET = fontsize

VERSION = 1.0

SOURCES += fontsize.cpp

HEADERS += fontsize.h