@echo off
echo "compiling qsa..."

cd src\qsa

echo building configure2...
cd configure2
qmake -spec %QMAKESPEC% -o Makefile configure2.pro
%MAKE%
  
echo executing configure2...
cd ..
"configure2\configure2.exe"

echo coping QT cache...
copy /Y %QTDIR%\.qmake.cache .qmake.cache
copy /Y %QTDIR%\.qmake.cache src\qsa\
copy /Y %QTDIR%\.qmake.cache src\plugin\

echo generating Makefile...
qmake
%MAKE%
