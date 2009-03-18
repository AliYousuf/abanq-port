INCLUDEPATH 		+= $$PWD 
INCLUDEPATH 		+= $$PWD/include
DEPENDPATH 		+= $$PWD

DEFINES 		+= MAKE_KDEFX_LIB

HEADERS 		+= $$PWD/kcpuinfo.h \
			   $$PWD/kdrawutil.h \
			   $$PWD/kimageeffect.h \
			   $$PWD/kpixmap.h \
			   $$PWD/kpixmapeffect.h \
			   $$PWD/kpixmapsplitter.h \
			   $$PWD/kstyle.h
           
SOURCES 		+= $$PWD/kcpuinfo.cpp \
			   $$PWD/kdrawutil.cpp \
			   $$PWD/kimageeffect.cpp \
			   $$PWD/kpixmap.cpp \
			   $$PWD/kpixmapeffect.cpp \
			   $$PWD/kpixmapsplitter.cpp \
			   $$PWD/kstyle.cpp
