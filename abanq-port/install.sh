#!/bin/bash
#***************************************************************************
#                         install.sh  -  shell-script de instalación
#                             -------------------
#    begin                : Mar Abr 09 2003
#    copyright            : (C) 2003-2008 by InfoSiAL, S.L.
#    email                : mail@infosial.com
#***************************************************************************

#***************************************************************************
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#***************************************************************************

VER="2.3"

OPT_HELP=no
OPT_PREFIX=""
OPT_QMAKESPEC="linux-g++"
OPT_DEBUG=no
OPT_DEBUGGER=no
OPT_TEST=no
OPT_QUICK=no
OPT_SQLLOG=no
QT_DEBUG="-release -DQT_NO_CHECK -DQT_NO_COMPAT"
QSADIR=qsa

VERSION="$VER Build $(svnversion -n)"
BUILD_KEY="$VER-Build-$(svnversion -n)"

for a in "$@"; do
	case "$a" in
		-help|--help)
			OPT_HELP=yes
		;;
		-debug)
			OPT_DEBUG=yes
		;;
		-qtdebug)
			QT_DEBUG="-debug -DQT_NO_COMPAT"
		;;
		-sqllog)
			OPT_SQLLOG=yes
		;;
		-debugger)
			OPT_DEBUGGER=yes
		;;
		-test)
			OPT_TEST=yes
		;;
		-quick)
			OPT_QUICK=yes
			VERSION="$VER-QUICK Build $(svnversion -n)"
		;;
		-prefix|-platform)
			VAR=`echo $a | sed "s,^-\(.*\),\1,"`
			shift
			VAL="yes"
		;;
		*)
		if [ "$VAL" == "yes" ];then
			VAL=$a
		fi
		;;
	esac
	case "$VAR" in
		prefix)
		if  [ $VAL != "yes" ];then
			OPT_PREFIX=$VAL
		fi
		;;
		platform)
		if  [ $VAL != "yes" ];then
			OPT_QMAKESPEC=$VAL
			export QMAKESPEC=$OPT_QMAKESPEC
		fi
		;;
	esac
done

echo -e "\nUtilidad de compilación e instalación de AbanQ $VERSION"
echo -e "(C) 2003-2008 InfoSiAL, S.L. http://infosial.com - http://abanq.org\n"

if [ "$OPT_HELP" = "yes" ]; then
    cat <<EOF
Uso: install.sh [opciones]
Opciones:
  -help 	: Despliega esta ayuda
  -platform id	: Identifica la plataforma
  -prefix ruta  : Directorio de instalacion
  -pglib ruta	: Directorio de librerias de  postgres
  -qtdir ruta	: Diretorio QTDIR
  -debug	: Compilar con símbolos de depuración
  -quick	: Compilar en modo 'quick' optimizado para conexiones lentas via Internet.

EOF
   exit 0
fi


if [ ! -r ./COPYING ]
then
	echo -e "No se encuentra el fichero COPYING, con la licencia. Lo sentimos, no se puede continuar."
	echo -e "Debes ejecutar este script dentro del directorio del código fuente de AbanQ $VERSION.\n"
	exit 1
fi

while [ "$accept" != "si" -a "$accept" != "SI" ]
do
	echo -e "Sólo se permite el uso del software AbanQ $VERSION\nbajo la licencia GNU General Public License (GPL).\n"
	echo -e "Teclea 'G' si deseas visualizar el texto de la licencia GNU General Public License.\n"
	echo -e "¿Aceptas los términos de la licencia GNU General Public License? (si/no)"

	read accept
	echo -e "\n"
	if [ "$accept" = "G" -o "$accept" = "g" ]
	then
			if [ -r ./COPYING ]
			then
				cat ./COPYING | less
			else
				echo -e "No se encuentra el fichero COPYING, con la licencia. Lo sentimos, no se puede continuar."
				echo -e "Debes ejecutar este script dentro del directorio del código fuente de AbanQ $VERSION.\n"
				exit 1
			fi
  else
		if [ "$accept" = "no" -o "$accept" = "NO" ]
		then
			echo -e "No has aceptado la licencia, AbanQ $VERSION no se puede instalar.\n"
			exit 0
		fi
	fi
done

if [ "$OPT_PREFIX" = "" ]
then
	echo -e "AVISO : No se ha especificado directorio de instalación"
	echo -e "Uso :  $0 directorio_de_instalacion\n"
	DIRINST=$PWD/lite-instalacion
	echo -e "Utilizando por defecto el directorio $DIRINST\n"
else
	DIRINST=$OPT_PREFIX
fi

mkdir -p $DIRINST

if [ ! -w $DIRINST ]
then
	echo -e "ERROR : Actualmente no tienes permisos de escritura en el directorio de instalación ($DIRINST)."
	echo -e "Solución : Cambia los permisos o ejecuta este script como un usuario que tenga permisos de escritura en ese directorio.\n"
  exit 1
fi

ACTUAL=$PWD
PREFIX=$DIRINST

echo -e "Directorio de instalación : $PREFIX\n"

echo -e "Estableciendo configuración...\n"

rm -f $HOME/.qmake.cache

export QTDIR=$ACTUAL/src/qt

if [ ! -f $QTDIR/include/qglobal.h ]
then
	cd $QTDIR
	make -f Makefile.cvs
	cd $ACTUAL
fi

version=$(cat $QTDIR/include/qglobal.h | grep "QT_VERSION_STR" | sed "s/.*\"\(.*\)\"/\1/g")
echo -e "Versión de Qt... $version\n"
echo -e "Compilando qmake y moc...\n"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$QTDIR/lib
cd $QTDIR
cp -vf Makefile.qt Makefile
./configure -platform $OPT_QMAKESPEC -prefix $PREFIX -R$PREFIX/lib -L$PREFIX/lib $QT_DEBUG -thread -no-pch -no-exceptions -buildkey $BUILD_KEY -stl -xinerama -disable-opengl -no-cups -no-nas-sound -no-nis -qt-libjpeg -qt-gif -qt-libmng -qt-libpng -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng
	
make qmake-install || exit 1
make moc-install || exit 1

cd $ACTUAL

export PATH=$PREFIX/bin:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$PREFIX/lib

echo -e "\nComprobando qmake...\n"
$QTDIR/bin/qmake -v > /dev/null
if [ $? = 154 ]
then
	echo -e "OK : qmake encontrado\n"
else
	echo -e "ERROR : No se encuentra qmake, esta utilidad se proporciona con las Qt."
	exho -e "        Comprueba que se encuentra en la ruta de búsqueda (variable $PATH).\n"
  	exit 1
fi

cd $ACTUAL

if  [ "$OPT_QMAKESPEC" == "macx-g++" ];then
	DIRAPP="qApp->applicationDirPath() + \"/../../../../\""
	echo -e "//$(date +%d%m%Y):$PREFIX" > ./config.h
	echo -e "/* config.h  Generado automaticamente por $0, NO EDITAR. */\n" >> ./config.h
	echo -e "#define FLDATA $DIRAPP + \"share/facturalux\"\n" >> config.h
	mkdir -p $PREFIX/share/facturalux/forms
	mkdir -p $PREFIX/share/facturalux/tables
	mkdir -p $PREFIX/share/facturalux/translations
	mkdir -p $PREFIX/share/facturalux/scripts
	mkdir -p $PREFIX/share/facturalux/queries
	mkdir -p $PREFIX/share/facturalux/reports
	mkdir -p $PREFIX/share/facturalux/tmp
	echo -e "#define FLDOC $DIRAPP + \"share/facturalux/doc\"\n" >> config.h
	mkdir -p $PREFIX/share/facturalux/doc
	echo -e "#define FLLIB $DIRAPP + \"lib\"\n" >> config.h
	mkdir -p $PREFIX/lib
	echo -e "#define FLBIN $DIRAPP + \"bin\"\n" >> config.h
	mkdir -p $PREFIX/bin
	echo -e "#define FLPREFIX $DIRAPP\n" >> config.h
	echo -e "#define QTDIR $DIRAPP\n" >> config.h
	echo -e "#define VERSION \"$VERSION\"\n" >> config.h
	echo -e "#define FL_HOME QDir::homeDirPath()\n" >> config.h
	
	echo "PREFIX = $PREFIX" > settings.pro
	echo "ROOT = $ACTUAL" >> settings.pro
	echo "DEFINES *= FL_EXPORT=\"\"" >> settings.pro
	echo "INCLUDEPATH *= $PREFIX/include" >> settings.pro
	#echo "DEFINES *= QSDEBUGGER" >> settings.pro
else
	echo -e "//$(date +%d%m%Y):$PREFIX" > ./config.h
	echo -e "/* config.h  Generado automáticamente por $0, NO EDITAR. */\n" >> ./config.h
	echo -e "#define FLDATA \"$PREFIX/share/facturalux\"\n" >> config.h
	mkdir -p $PREFIX/share/facturalux/forms
	mkdir -p $PREFIX/share/facturalux/tables
	mkdir -p $PREFIX/share/facturalux/translations
	mkdir -p $PREFIX/share/facturalux/scripts
	mkdir -p $PREFIX/share/facturalux/queries
	mkdir -p $PREFIX/share/facturalux/reports
	mkdir -p $PREFIX/share/facturalux/tmp
	echo -e "#define FLDOC \"$PREFIX/share/facturalux/doc\"\n" >> config.h
	mkdir -p $PREFIX/share/facturalux/doc
	echo -e "#define FLLIB \"$PREFIX/lib\"\n" >> config.h
	mkdir -p $PREFIX/lib
	echo -e "#define FLBIN \"$PREFIX/bin\"\n" >> config.h
	mkdir -p $PREFIX/bin
	echo -e "#define FLPREFIX \"$PREFIX\"\n" >> config.h
	echo -e "#define QTDIR \"$PREFIX\"\n" >> config.h
	echo -e "#define VERSION \"$VERSION\"\n" >> config.h
	echo -e "#define FL_HOME QDir::home().absPath()\n" >> config.h
	
	echo "PREFIX = $PREFIX" > settings.pro
	echo "ROOT = $ACTUAL" >> settings.pro
	echo "DEFINES *= FL_EXPORT=\"\"" >> settings.pro
	echo "INCLUDEPATH *= $PREFIX/include" >> settings.pro
	#echo "DEFINES *= QSDEBUGGER" >> settings.pro
fi


echo "QMAKE_MKDIR           = @echo AbanQ ::   [MKDIR] $< && \$\$QMAKE_MKDIR" >> settings.pro
echo "QMAKE_COPY            = @echo AbanQ ::   [CP___] $< && \$\$QMAKE_COPY" >> settings.pro
echo "QMAKE_CC              = @echo AbanQ ::   [CC___] $< && \$\$QMAKE_CC" >> settings.pro
echo "QMAKE_CXX             = @echo AbanQ ::   [C++__] $< && \$\$QMAKE_CXX" >> settings.pro
#echo "QMAKE_MOC             = @echo AbanQ ::   [MOC__] $< && \$\$QMAKE_MOC" >> settings.pro
#echo "QMAKE_UIC             = @echo AbanQ ::   [UIC__] $< && \$\$QMAKE_UIC" >> settings.pro
echo "QMAKE_AR              = @echo AbanQ ::   [AR___] $< && \$\$QMAKE_AR" >> settings.pro
echo "QMAKE_LINK            = @echo AbanQ ::   [LK___] \$@ && \$\$QMAKE_LINK" >> settings.pro
echo "QMAKE_LINK_SHLIB      = @echo AbanQ ::   [LK___] \$@ && \$\$QMAKE_LINK_SHLIB" >> settings.pro
echo "QMAKE_MOVE            = @echo AbanQ ::   [MV___] \$@ && \$\$QMAKE_MOVE" >> settings.pro
echo "QMAKE_DEL_FILE        = @echo AbanQ ::   [RM___] \$@ && \$\$QMAKE_DEL_FILE" >> settings.pro
echo "QMAKE_DEL_DIR         = @echo AbanQ ::   [RMDIR] \$@ && \$\$QMAKE_DEL_DIR" >> settings.pro
echo "QMAKE_STRIP           = @echo AbanQ ::   [STRIP] \$@ && \$\$QMAKE_STRIP" >> settings.pro
echo "QMAKE_CHK_DIR_EXISTS  = @echo AbanQ ::   [CHK__] \$@ && \$\$QMAKE_CHK_DIR_EXISTS" >> settings.pro
echo "QMAKE_INSTALL_FILE    = @echo AbanQ ::   [INS__] \$@ && \$\$QMAKE_INSTALL_FILE" >> settings.pro
echo "QMAKE_INSTALL_PROGRAM = @echo AbanQ ::   [INS__] \$@ && \$\$QMAKE_INSTALL_PROGRAM" >> settings.pro

if [ "$OPT_DEBUG" = "yes" ]
then
	echo "CONFIG *= debug" >> settings.pro
	echo "DEFINES *= FL_DEBUG QSDEBUGGER" >> settings.pro

fi

if [ "$OPT_DEBUGGER" = "yes" ]
then
	if [ "$OPT_DEBUG" = "no" ]
	then
		echo "DEFINES *= FL_DEBUG" >> settings.pro
	fi
	echo "DEFINES *= QSDEBUGGER FL_DEBUGGER" >> settings.pro
	QSADIR=qsa-debugger
fi

if [ "$OPT_SQLLOG" = "yes" ]
then
	echo "DEFINES *= FL_SQL_LOG" >> settings.pro

fi

if [ "$OPT_TEST" = "yes" ]
then
	echo "DEFINES *= FL_TEST" >> settings.pro
fi

if [ "$OPT_QUICK" = "yes" ]
then
	echo "DEFINES *= FL_QUICK_CLIENT" >> settings.pro
fi

echo -e "\nCompilando Hoard...\n"
cd $ACTUAL/src/hoard
$QTDIR/bin/qmake hoard.pro
make || exit 1
cd $ACTUAL

echo -e "\nCompilando Qt ($QTDIR) ...\n"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib:$PREFIX/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$QTDIR/lib:$PREFIX/lib
cd $QTDIR
make install || exit 1
export QTDIR=$PREFIX

echo "Compilando QSA..."
cd $ACTUAL/src/$QSADIR
cp -fv ../qt/.qmake.cache .qmake.cache
cp -fv ../qt/.qmake.cache src/$QSADIR/
cp -fv ../qt/.qmake.cache src/plugin/
./configure
make || exit 1
make install

cd $ACTUAL

echo -e "Creando Makefiles con qmake...\n"

cp -fv src/qt/.qmake.cache .qmake.cache
cp -fv src/qt/.qmake.cache src/advance/
cp -fv src/qt/.qmake.cache src/qwt/
cp -fv src/qt/.qmake.cache src/qwt/designer
cp -fv src/qt/.qmake.cache src/barcode/
cp -fv src/qt/.qmake.cache src/flbase/
cp -fv src/qt/.qmake.cache src/fllite/
cp -fv src/qt/.qmake.cache src/flbase/
cp -fv src/qt/.qmake.cache src/kugar/
cp -fv src/qt/.qmake.cache src/sqlite/
cp -fv src/qt/.qmake.cache src/libpq/
cp -fv src/qt/.qmake.cache src/dbf/
cp -fv src/qt/.qmake.cache src/kudesigner/
cp -fv src/qt/.qmake.cache src/teddy/
cp -fv src/qt/.qmake.cache src/plugins/designer/flfielddb/
cp -fv src/qt/.qmake.cache src/plugins/designer/fltabledb/
cp -fv src/qt/.qmake.cache src/plugins/sqldrivers/sqlite/
cp -fv src/qt/.qmake.cache src/plugins/sqldrivers/psql/
cp -fv src/qt/.qmake.cache src/plugins/styles/bluecurve/
cp -fv src/qt/.qmake.cache src/plugins/styles/kde/

$QTDIR/bin/qmake lite.pro

echo -e "Compilando...\n"
cd src/flbase
$QTDIR/bin/qmake flbase.pro
make uicables || exit 1
cd $ACTUAL
make || exit 1
make install

if  [ "$OPT_QMAKESPEC" == "macx-g++" ];then
	echo -e "\nConfigurando packete app ...\n"

	install_name_tool -change libqsa.1.dylib @executable_path/../../../../lib/libqsa.1.dylib $PREFIX/bin/fllite.app/Contents/MacOS/fllite
	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/fllite.app/Contents/MacOS/fllite
	install_name_tool -change libflbase.2.dylib @executable_path/../../../../lib/libflbase.2.dylib $PREFIX/bin/fllite.app/Contents/MacOS/fllite
	install_name_tool -change libadvance.0.dylib @executable_path/../../../../lib/libadvance.0.dylib $PREFIX/bin/fllite.app/Contents/MacOS/fllite
	install_name_tool -change libflmail.1.dylib @executable_path/../../../../lib/libflmail.1.dylib $PREFIX/bin/fllite.app/Contents/MacOS/fllite

	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/kudesigner.app/Contents/MacOS/kudesigner

	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/teddy.app/Contents/MacOS/teddy

	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/designer.app/Contents/MacOS/designer
	
	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/linguist.app/Contents/MacOS/linguist
	install_name_tool -change libqt-mt.3.dylib @executable_path/../lib/libqt-mt.3.dylib $PREFIX/bin/lupdate

	install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $PREFIX/bin/assistant.app/Contents/MacOS/assistant
	
	for i in $(find $PREFIX -type f -name "*.dylib" -print)
	do
		install_name_tool -change libqsa.1.dylib @executable_path/../../../../lib/libqsa.1.dylib $i
		install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../lib/libqt-mt.3.dylib $i
		install_name_tool -change libflbase.2.dylib @executable_path/../../../../lib/libflbase.2.dylib $i
		install_name_tool -change libadvance.0.dylib @executable_path/../../../../lib/libadvance.0.dylib $i
		install_name_tool -change libkdefxx.1.dylib @executable_path/../../../../lib/libkdefxx.1.dylib $i
		install_name_tool -change libpq.3.dylib @executable_path/../../../../lib/libpq.3.dylib $i
		install_name_tool -change libsqlite.2.dylib @executable_path/../../../../lib/libsqlite.2.dylib $i
		install_name_tool -change libmysqlclient.14.dylib @executable_path/../../../../lib/libmysqlclient.14.dylib $i
		install_name_tool -change libflmail.1.dylib @executable_path/../../../../lib/libflmail.1.dylib $i
	done
fi

if [ "$OPT_DEBUG" = "no" ]
then
	strip --strip-unneeded $PREFIX/bin/* 2> /dev/null
	strip --strip-unneeded $PREFIX/lib/* 2> /dev/null
	strip --strip-unneeded $PREFIX/plugins/designer/* 2> /dev/null
	strip --strip-unneeded $PREFIX/plugins/sqldrivers/* 2> /dev/null
	strip --strip-unneeded $PREFIX/plugins/styles/* 2> /dev/null
fi

echo -e "\nTerminando instalación...\n"
cd src/translations
./update.sh 2> /dev/null
cd ../..
cp -fv ./src/forms/*.ui $PREFIX/share/facturalux/forms
cp -fv ./src/tables/*.mtd $PREFIX/share/facturalux/tables
cp -fv ./src/translations/*.ts $PREFIX/share/facturalux/translations
cp -fv ./src/scripts/*.qs $PREFIX/share/facturalux/scripts
cp -fv ./src/docs/*.html $PREFIX/share/facturalux/doc
cp -fv ./src/*.xml $PREFIX/share/facturalux
cp -fv ./src/*.xpm $PREFIX/share/facturalux

if [ "$OPT_DEBUGGER" = "yes" ]
then
	cp -fv $PREFIX/plugins/designer/* $PREFIX/plugins/qsa
fi

echo -e "\nAbanQ $VERSION\n(C) 2003-2008 InfoSiAL, S.L. http://infosial.com - http://abanq.org\n"
echo -e "Instalación terminada.\nPara iniciar la aplicación ejecuta $PREFIX/bin/fllite\n"
