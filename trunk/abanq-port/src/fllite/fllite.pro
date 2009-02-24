exists(../../settings.pro) {
	include(../../settings.pro)
}
else {
	error("settings.pro missing, unable to build")
}

TEMPLATE = app 

CONFIG += warn_off link_prl qt 
unix:CONFIG += x11
CONFIG -= dll

win32:RC_FILE = fllite.rc
mac:RC_FILE = fl.icns

DESTDIR = $$PREFIX/bin

TARGET = fllite 

INCLUDEPATH += $$ROOT $$ROOT/src/flbase
DEPENDPATH += $$ROOT/src/flbase

unix:LIBS += -L$$PREFIX/lib -lhoard -ldl -lflbase
win32:LIBS += -L$$PREFIX/lib -lhoard -lflbase
mac:LIBS += -ladvance -lqui -lflmail

load(qsa)

!contains(DEBIAN, true) {
	!mac:QMAKE_RPATH = -Wl,-rpath,$$PREFIX/lib,-rpath,
}

SOURCES += main.cpp

contains(DEFINES, FL_QUICK_CLIENT) {
	IMAGES += images/splashabanq.png
} else {
	IMAGES += images/splashabanq.png
}

