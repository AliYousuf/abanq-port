#!/bin/bash

##############################################################
# Inicializaciones
##############################################################
export VER="2.3"
export BASEDIR=$PWD
export VERSION="$VER Build $(svnversion -n)"
export BUILD_MINGW=yes
export BUILD_FLUX=yes
export BUILD_QT=yes
export BUILD_QSA=yes
export BUILD_KEY="$VER-win32-g++-cross"

##############################################################
# MinGW compilador cruzado Linux -> Win32
##############################################################
if [ "$BUILD_MINGW" == "yes" ]
then

cd $BASEDIR
cd cross-mingw
mkdir build
cp -f orig/* build/
cd build
make -i -f xmingw32.mak all test
cp -fv xmingw32.sh ../xmingw32/bin/

cd $BASEDIR
cd src/qt
source myconfigure

cd $BASEDIR
cd cross-mingw/pthreads
make CROSS=mingw32- clean GC
cp pthread.h ../xmingw32/mingw32/include/ 
cp semaphore.h ../xmingw32/mingw32/include/
cp sched.h ../xmingw32/mingw32/include/
cp libpthreadGC2.a ../xmingw32/mingw32/lib/

fi

cd $BASEDIR
cd src/qt
source myconfigure
cd $BASEDIR
export DIRINST=$QTDIR
export PREFI="C:/facturalux"
export ACTUAL=$PWD
export PREFIX=$DIRINST
export PTHREADS="$ACTUAL/cross-mingw/pthreads"

echo "#define FLDATA \"$PREFI/share/facturalux\"" > config.h
mkdir -p $PREFIX/share/facturalux/forms
mkdir -p $PREFIX/share/facturalux/tables
mkdir -p $PREFIX/share/facturalux/translations
mkdir -p $PREFIX/share/facturalux/scripts
mkdir -p $PREFIX/share/facturalux/queries
mkdir -p $PREFIX/share/facturalux/reports
mkdir -p $PREFIX/share/facturalux/tmp
echo "#define FLDOC \"$PREFI/share/facturalux/doc\"" >> config.h
mkdir -p $PREFIX/share/facturalux/doc
echo "#define FLLIB \"$PREFI/lib\"" >> config.h
mkdir -p $PREFIX/lib
echo "#define FLBIN \"$PREFI/bin\"" >> config.h
mkdir -p $PREFIX/bin
echo "#define FLPREFIX \"$PREFI\"" >> config.h
echo "#define QTDIR \"$QTDIR\"" >> config.h
echo "#define VERSION \"$VERSION\"" >> config.h
echo "#define FL_HOME QDir::homeDirPath()" >> config.h

echo "PREFIX = $PREFIX" > settings.pro
echo "ROOT = $ACTUAL" >> settings.pro
echo "PTHREADS = $PTHREADS" >> settings.pro
echo "DEFINES *= FL_EXPORT=\"\"" >> settings.pro
echo "OBJECTS_DIR=.o" >> settings.pro
echo "LIBS *= -L$PREFIX/lib" >> settings.pro
#echo "DEFINES *= FL_QUICK_CLIENT" >> settings.pro
#echo "DEFINES *= FL_TEST" >> settings.pro


##############################################################
# Qt
##############################################################
if [ "$BUILD_QT" == "yes" ]
then

cd $BASEDIR
cd src/qt
source myconfigure
sh limpiar
if [ ! -f ./include/qglobal.h ]
then
	make -f Makefile.cvs
fi
cd $BASEDIR
cat qconfig.h.in | sed "s/@BKEY@/$BUILD_KEY/" > qconfig.h
cp -fv qconfig.h src/qt/include
cp -fv qmodules.h src/qt/include
cp -fv qtwinconfig src/qt/.qtwinconfig
cd src/qt
./configure -prefix $PREFIX -L$PREFIX/lib -release -DQT_NO_CHECK -DQT_NO_COMPAT -thread -no-pch -no-exceptions -platform linux-g++ -xplatform win32-g++-cross -buildkey $BUILD_KEY -stl -disable-opengl -qt-libjpeg -qt-gif -qt-libmng -qt-libpng -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng

make qmake-install || exit 1
make moc-install || exit 1

cd $BASEDIR
cd src/hoard     
$BASEDIR/src/qt/bin/qmake hoard.pro
make || exit 1

cd $BASEDIR
cd src/qt
if [ ! -f ./bin/uic ]
then
	echo "Falta el compilador de interfaces 'uic'. Hay que compilarlo para linux y copiarlo en $BASEDIR/src/qt/bin."
	exit 1
fi
make 
make || exit 1

fi

##############################################################
# Qt QSA
##############################################################
if [ "$BUILD_QSA" == "yes" ]
then

cd $BASEDIR
cd src/qsa
configure2/configure2
sh cpqmkcache
qmake
make || exit 1

fi

##############################################################
# FacturaLUX Lite
##############################################################
if [ "$BUILD_FLUX" == "yes" ]
then

cd $BASEDIR

cp -v src/forms/*.ui $PREFIX/share/facturalux/forms
cp -v src/tables/*.mtd $PREFIX/share/facturalux/tables
cp -v src/translations/*.ts $PREFIX/share/facturalux/translations
cp -v src/scripts/*.qs $PREFIX/share/facturalux/scripts
cp -v src/docs/*.html $PREFIX/share/facturalux/doc
cp -v src/*.xml $PREFIX/share/facturalux
cp -v src/*.xpm $PREFIX/share/facturalux

cd src
sh cpqmkcache
cd flbase
qmake
make mocables
cd ..
qmake
make

fi
