SOURCES += $$ROOT/src/posprinter/posprinter.cpp

win32:SOURCES += $$ROOT/src/posprinter/posprinter_win32.cpp
unix:SOURCES += $$ROOT/src/posprinter/posprinter_unix.cpp

HEADERS += $$ROOT/src/posprinter/posprinter.h
