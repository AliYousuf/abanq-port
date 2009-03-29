exists(../../../../../settings.pro) {
        include(../../../../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE 		= lib
CONFIG 			+= warn_off
win32 {
        CONFIG += qt
        DLLDESTDIR = $$PREFIX/bin
}
else {
        CONFIG += qt x11 create_prl link_prl
}
DESTDIR = $$PREFIX/lib

TARGET 			= kdefxx

INCLUDEPATH 		+= .
INCLUDEPATH     	+= include

DEFINES 		+= MAKE_KDEFX_LIB 

QMAKE_CXXFLAGS 		+= -DKDE_MAKE_LIB=1 

HEADERS 		+= kcpuinfo.h \
			   kdrawutil.h \
			   kimageeffect.h \
			   kpixmap.h \
			   kpixmapeffect.h \
			   kpixmapsplitter.h \
			   kstyle.h
           
SOURCES 		+= kcpuinfo.cpp \
			   kdrawutil.cpp \
			   kimageeffect.cpp \
			   kpixmap.cpp \
			   kpixmapeffect.cpp \
			   kpixmapsplitter.cpp \
			   kstyle.cpp
