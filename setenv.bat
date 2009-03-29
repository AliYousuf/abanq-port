@echo off
rem ***************************************************************************
rem          REQUIERE AJUSTE MANUAL
rem MinGW variables
set MINGW=c:\MinGW

rem Qt variables
set QTDIR=%CD%\src\qt
set QMAKESPEC=win32-g++

rem Abanq variables

rem     Directorio base de codigo fuente
set ABROOT=%CD%

rem PATH variables
set PATH=%QTDIR%\bin;%MINGW%\bin;%PATH%