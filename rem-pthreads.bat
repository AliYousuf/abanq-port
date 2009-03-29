rem @echo off
echo "eliminando pthreads"
cd cross-mingw\pthreads
mingw32-make clean
del *.o
del *.i
del *.exe
del *.dll
del pthread.def

del %MINGW%\include\pthread.h
del %MINGW%\include\semaphore.h
del %MINGW%\include\sched.h
del %MINGW%\lib\libpthreadGC2.a 

cd %ABROOT%