@echo off



echo "compilando Qt"

copy qconfig.h src\qt\include
copy qmodules.h src\qt\include
copy qtwinconfig src\qt\.qtwinconfig

cd src\qt

mkdir bin

if not exist misc\findpath\findpath.exe (
	cd misc\findpath && mingw32-make
	cd ..\..
	)
	
::./configure -prefix $PREFIX -L$PREFIX/lib -release -DQT_NO_CHECK -DQT_NO_COMPAT -thread -no-pch -no-exceptions -platform linux-g++ -xplatform win32-g++-cross -buildkey $BUILD_KEY -stl -disable-opengl -qt-libjpeg -qt-gif -qt-libmng -qt-libpng -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng
::bin\configure -prefix %PREFIX% -libdir %PREFIX%/lib -release -thread -no-pch -no-exceptions -stl -disable-opengl -gif -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng
::configure -prefix %PREFIX% -libdir %PREFIX%/lib -release -thread -no-pch -no-exceptions -stl -disable-opengl -gif -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng -v
configure  -thread -no-pch -no-exceptions -stl -disable-opengl -gif -qt-imgfmt-png -qt-imgfmt-jpeg -qt-imgfmt-mng %*

cd %ABROOT%















