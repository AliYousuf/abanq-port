exists(../../../../../settings.pro) {
        include(../../../../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE                = lib
CONFIG                  += warn_off
win32 {
        CONFIG += qt
        DLLDESTDIR = $$PREFIX/bin
        INCLUDEPATH += ./
}
else {
        CONFIG += qt x11 create_prl link_prl
}
DESTDIR = $$PREFIX/lib

TARGET                  = kdelite

HEADERS 		+= kglobal.h \
                           klocale.h \
			   kcolorbutton.h \
			   kcolordrag.h \
			   kfiledialog.h \
			   kactivelabel.h \
			   kaboutdata.h \

SOURCES 		+= kcolorbutton.cpp \
			   kcolordrag.cpp \
			   kfiledialog.cpp \
			   kactivelabel.cpp \
			   kaboutdata.cpp