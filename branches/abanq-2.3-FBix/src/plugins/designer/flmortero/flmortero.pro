exists(../../../../settings.pro) {
        include(../../../../settings.pro)
}
else {
	error("settings.pro missing, unable to build")
}

TEMPLATE = lib 
CONFIG += warn_off plugin
DESTDIR = $$PREFIX/plugins/designer
TARGET = flmortero
SOURCES += main.cpp
HEADERS += flmortero.h
LIBS += -L$$PREFIX/lib
IMAGES += images/mortero.png
