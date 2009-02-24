exists(../../settings.pro) {
        include(../../settings.pro)
}
else {
        error("settings.pro missing, unable to build")
}

TEMPLATE = lib
CONFIG += warn_off
CONFIG -= qt x11
DEFINES +=  SYSCONFDIR='"$$PREFIX"' _GNU_SOURCE FRONTEND ENABLE_THREAD_SAFETY _THREAD_SAFE _POSIX_PTHREAD_SEMANTICS
DESTDIR = $$PREFIX/lib
TARGET = pq
INCLUDEPATH = . $$ROOT/src/libpq/include
LIBS += -lcrypt -lresolv -lnsl
VERSION = 4.1

SOURCES += encnames.c \
           fe-auth.c \
           fe-connect.c \
           fe-exec.c \
           fe-lobj.c \
           fe-misc.c \
           fe-print.c \
           fe-protocol2.c \
           fe-protocol3.c \
           fe-secure.c \
           ip.c \
           md5.c \
           noblock.c \
           pgstrcasecmp.c \
           pqexpbuffer.c \
           pqsignal.c \
           thread.c \
           wchar.c

HEADERS += libpq-fe.h \
           libpq-int.h \
           fe-auth.h \
           pqexpbuffer.h \
           pqsignal.h



