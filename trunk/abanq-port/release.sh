#!/bin/sh
#***************************************************************************
#                                 release.sh
#                             -------------------
#    begin                : Mar Abr 09 2003
#    copyright            : (C) 2003 by InfoSiAL, S.L.
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

rm -f install
export QTDIR=./lite-instalacion
make distclean
cd src/qt
make distclean
rm -f include/*.h
rm -f include/private/*.h
rm -f $HOME/.qmake.cache
rm -vf .qmake.cache
rm -vfR ./lib/lib*
cd qmake
make distclean
cd ../bin
rm -vf *
cd ../lib
rm -vf
cd ../../qsa
qmake
cd configure2
qmake
cd ../src/qsa
make distclean
cd ../../qsa-debugger
make distclean
cd ../..
make distclean
cd ../..
rm -vf ./config.h
rm -vf ./src/translations/translations.pro
rm -vf *.kdevses
rm -vf *.pcs
rm -vf settings.pro
rm -vfR ./bin ./lib ./share
find . -name "lite-instalacion" -exec rm -vfR \{\} \;
find . -name "*~" -exec rm -vf \{\} \;
find . -name "*.bak" -exec rm -vf \{\} \;
find . -name "*.old" -exec rm -vf \{\} \;
for a in $(find . -name "*.pro" -print) 
do
	cat $a | sed "s/PREFIX\ =\ .*/PREFIX=/" | sed "s/ROOT\ =\ .*/ROOT=/" | sed "s/INCLUDE_PGSQL\ =\ .*/INCLUDE_PGSQL=/" | sed "s/LIB_PGSQL\ =\ .*/LIB_PGSQL=/" > ./temp
	mv -f temp $a
done
find . -name "Makefile" -exec rm -vf \{\} \;
find . -name ".moc" -type d -exec rm -vfR \{\} \;
find . -name ".obj" -type d -exec rm -vfR \{\} \;
find . -name ".uic" -type d -exec rm -vfR \{\} \;
find . -name ".tmp" -type d -exec rm -vfR \{\} \;
find . -name "*.qm" -exec rm -vf \{\} \;
find . -name "*.o" -exec rm -vf \{\} \;
find . -name "*.moc" -exec rm -vf \{\} \;
find . -name ".*.cache" -exec rm -vf \{\} \;
find . -name "*.prl" -exec rm -vf \{\} \;
find . -name "*.*.orig" -exec rm -vf \{\} \;

rm -f      src/qsa/tools/designer/editor/preferences.cpp
rm -f      src/qsa/tools/designer/editor/preferences.h
rm -f      src/qsa/src/ide/idewindow.h
rm -f      src/qsa/src/ide/outputcontainer.cpp
rm -f      src/qsa/src/ide/preferencescontainer.cpp
rm -f      src/qsa/src/ide/outputcontainer.h
rm -f      src/qsa/src/ide/preferencescontainer.h
rm -f      src/qsa/src/ide/projectcontainer.cpp
rm -f      src/qsa/src/ide/projectcontainer.h
rm -f      src/qsa/src/ide/gotoline.cpp
rm -f      src/qsa/src/ide/replacetext.cpp
rm -f      src/qsa/src/ide/gotoline.h
rm -f      src/qsa/src/ide/replacetext.h
rm -f      src/qsa/src/ide/findtext.cpp
rm -f      src/qsa/src/ide/idewindow.cpp
rm -f      src/qsa/src/ide/findtext.h
rm -f      src/qsa/src/qsa/qmake_image_collection.cpp
rm -f      src/qt/lib/qt-mt.pc
rm -f      src/qt/lib/qt.pc
rm -f      src/qt/config.status
rm -f      src/qsa/configure2/configure2
rm -f	   src/qt/src/tools/qconfig.cpp


rm -f      src/qsa-debugger/tools/designer/editor/preferences.cpp
rm -f      src/qsa-debugger/tools/designer/editor/preferences.h
rm -f      src/qsa-debugger/src/ide/idewindow.h
rm -f      src/qsa-debugger/src/ide/outputcontainer.cpp
rm -f      src/qsa-debugger/src/ide/preferencescontainer.cpp
rm -f      src/qsa-debugger/src/ide/outputcontainer.h
rm -f      src/qsa-debugger/src/ide/preferencescontainer.h
rm -f      src/qsa-debugger/src/ide/projectcontainer.cpp
rm -f      src/qsa-debugger/src/ide/projectcontainer.h
rm -f      src/qsa-debugger/src/ide/gotoline.cpp
rm -f      src/qsa-debugger/src/ide/replacetext.cpp
rm -f      src/qsa-debugger/src/ide/gotoline.h
rm -f      src/qsa-debugger/src/ide/replacetext.h
rm -f      src/qsa-debugger/src/ide/findtext.cpp
rm -f      src/qsa-debugger/src/ide/idewindow.cpp
rm -f      src/qsa-debugger/src/ide/findtext.h
rm -f      src/qsa-debugger/src/qsa/qmake_image_collection.cpp
rm -f      src/qsa-debugger/configure2/configure2
rm -f      src/qsa-debugger/src/fullide/quickbreakpointsettings.h
rm -f      src/qsa-debugger/src/fullide/quickbreakpointsettings.cpp
rm -f      src/qsa-debugger/src/fullide/quickprojectsettings.h
rm -f      src/qsa-debugger/src/fullide/quickprojectsettings.cpp
rm -f      src/qsa-debugger/src/fullide/qmake_image_collection.cpp

rm -f      src/qt/config.tests/unix/largefile/largefiletest
rm -f      src/qt/config.tests/unix/endian/endiantest
rm -f      src/qt/config.tests/unix/ptrsize/ptrsizetest
rm -f	   src/qt/config.tests/unix/ipv6/ipv6test

rm -f	   src/qt/mkspecs/default
