exists(../../../../settings.pro) {
        include(../../../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = lib
CONFIG += warn_off plugin
DESTDIR = $$PREFIX/plugins/sqldrivers
TARGET = qsqlite
INCLUDEPATH += $$ROOT $$ROOT/src/flbase $$ROOT/src/sqlite

!mac:LIBS += -L$$PREFIX/lib -lsqlite -lflbase
mac {
        load(qsa)
        LIBS += -L$$PREFIX/lib -lsqlite -lflbase -ladvance -lqui -lflmail
}

HEADERS += qsqlite.h
SOURCES += qsqlite.cpp main.cpp
