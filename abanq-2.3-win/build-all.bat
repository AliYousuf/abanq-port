@exho off
echo "Compilando abanq para windows"

call setenv.bat

call build-config.bat

call build-pthreads.bat

cd %ABROOT%

rem ***************************************************************************
rem           acepta opciones extra para configurar Qt
call build-Qt.bat -release 

cd %ABROOT%

call build-qsa.bat

cd %ABROOT%

call build-abanq.bat

