@echo off 

rem if "%A%"=="si" (
echo.
echo installing QT and QSA...
copy /Y %QTDIR%\bin\*.dll %PREFI%\bin
copy /Y %QTDIR%\bin\designer.exe %PREFI%\bin\
copy /Y %QTDIR%\bin\assistant.exe %PREFI%\bin\
copy /Y %QTDIR%\bin\linguist.exe %PREFI%\bin\
copy /Y %QTDIR%\lib\*.* %PREFI%\lib\
rem )

echo.
echo "copiando qt cache"
cd src
copy %QTDIR%\.qmake.cache .qmake.cache 

copy %QTDIR%\.qmake.cache barcode\.qmake.cache
copy %QTDIR%\.qmake.cache flbase\.qmake.cache
copy %QTDIR%\.qmake.cache fllite\.qmake.cache
copy %QTDIR%\.qmake.cache flbase\.qmake.cache
copy %QTDIR%\.qmake.cache kugar\.qmake.cache
copy %QTDIR%\.qmake.cache sqlite\.qmake.cache
copy %QTDIR%\.qmake.cache libpq\.qmake.cache
copy %QTDIR%\.qmake.cache libmysql\.qmake.cache
copy %QTDIR%\.qmake.cache advance\.qmake.cache
copy %QTDIR%\.qmake.cache flmail\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\designer\flfielddb\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\designer\fltabledb\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\sqldrivers\sqlite\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\sqldrivers\psql\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\sqldrivers\mysql\.qmake.cache
copy %QTDIR%\.qmake.cache plugins\styles\bluecurve\.qmake.cache

echo.
echo copying sys module
copy /Y %CD%\sys.* %PREFI%\share\facturalux\
copy /Y %CD%\forms\*.* %PREFI%\share\facturalux\forms\
copy /Y %CD%\scripts\*.* %PREFI%\share\facturalux\scripts\
copy /Y %CD%\tables\*.* %PREFI%\share\facturalux\tables\
copy /Y %CD%\translations\*.* %PREFI%\share\facturalux\translations\

copy /Y %ABROOT%\cross-mingw\pthreads\*.dll %PREFI%\bin
copy /Y %MINGW%\bin\mingwm10.dll %PREFI%\bin

echo.
echo "creando flbase..."
cd flbase
qmake 
%MAKE%  mocables
cd ..
echo "compilando abanq..."
qmake 
%MAKE%
cd ..
