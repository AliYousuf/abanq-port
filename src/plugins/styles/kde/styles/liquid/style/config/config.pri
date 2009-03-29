INCLUDEPATH 		+= $$PWD
DEPENDPATH		+= $$PWD
#########################################################

HEADERS += liquidconf.h previewdata.h
SOURCES += liquidconf.cpp

#########################################################
include( $$KDELITE_PATH/kdelite.pri )