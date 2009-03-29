exists(../../../../settings.pro) {
        include(../../../../settings.pro)
}
else {
	error("settings.pro missing, unable to build")
}

TEMPLATE = lib 
CONFIG += warn_off plugin 
DESTDIR = $$PREFIX/plugins/designer
TARGET = fltabledb 
INCLUDEPATH += $$ROOT $$ROOT/src/flbase
SOURCES += main.cpp 
LIBS += -L$$PREFIX/lib -lflbase
IMAGES += images/datatabledb.png
