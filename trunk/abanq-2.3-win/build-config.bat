@echo off

rem ***************************************************************************
rem          REQUIERE AJUSTE MANUAL
rem directorio de instalacion de aabanq -> usado por la configuracion de Qt y requiere
rem diagonales "/" para separar carpetas
set PREFIX=C:/facturalux

rem ***************************************************************************
rem          REQUIERE AJUSTE MANUAL
rem directorio de instalacion de aabanq -> usado pora crear directorios en windows y requiere
rem diagonales invertidas "\" para separar carpetas
set PREFI=C:\facturalux

rem ***************************************************************************
rem          REQUIERE AJUSTE MANUAL
rem directorio donde se localiza abanq -> usado para la configuracion de Qt y requiere
rem diagonales "/" para separar carpetas
set ROOT=C:/Qt/abanq-2.3-win

rem version que mostrara abanq
set VER=2.3
set VERSION="%VER% Build %date%-%time%"

rem directorio donde se localiza Qt -> usado para la configuracion de Qt y requiere
rem diagonales "/" para separar carpetas
set QTDIR_QT=%ROOT%/src/qt

rem directorio donde se encuentra pthreads -> usado para la configuracion de Qt y requiere
rem diagonales "/" para separar carpetas
set PTHREADS=%ROOT%/cross-mingw/pthreads

rem build key
set BUILD_KEY="%VER%-win32-g++"

echo "creando config.h"
del /Q config.h
echo #define FLDATA "%PREFIX%/share/facturalux" > config.h
mkdir %PREFI%\share\facturalux\forms
mkdir %PREFI%\share\facturalux\tables
mkdir %PREFI%\share\facturalux\translations
mkdir %PREFI%\share\facturalux\scripts
mkdir %PREFI%\share\facturalux\queries
mkdir %PREFI%\share\facturalux\reports
mkdir %PREFI%\share\facturalux\tmp
echo #define FLDOC "%PREFIX%/share/facturalux/doc" >> config.h
mkdir %PREFI%\share\facturalux\doc
echo #define FLLIB "%PREFIX%/lib" >> config.h
mkdir %PREFI%\lib
echo #define FLBIN "%PREFIX%/bin" >> config.h
mkdir %PREFI%\bin
mkdir %PREFI%\plugins\designer
echo #define FLPREFIX "%PREFIX%" >> config.h
echo #define QTDIR "%QTDIR_QT%" >> config.h
echo #define VERSION %VERSION% >> config.h
rem echo #define FL_HOME QString("%PREFIX%") >> config.h
echo #define FL_HOME QDir::homeDirPath() >> config.h

echo "creando settings.pro"
del /Q settings.pro
echo PREFIX = %PREFIX% >> settings.pro
echo ROOT = %ROOT% >> settings.pro
echo PTHREADS = %PTHREADS% >> settings.pro

echo DEFINES += FL_EXPORT="" >> settings.pro

echo OBJECTS_DIR=.o >> settings.pro
echo LIBS += -L%PREFIX%/lib >> settings.pro
rem echo DEFINES *= FL_QUICK_CLIENT >> settings.pro
rem echo DEFINES *= FL_TEST >> settings.pro
rem echo DEFINES += FL_DEBUG >> settings.pro

echo QTDIR = %QTDIR_QT% >> settings.pro

echo #define QT_BUILD_KEY %BUILD_KEY% >> qconfig.h
