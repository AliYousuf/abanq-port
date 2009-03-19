@echo off
echo "compilando pthreads"
cd cross-mingw\pthreads
mingw32-make clean GC
copy pthread.h %MINGW%\include 
copy semaphore.h %MINGW%\include
copy sched.h %MINGW%\include
copy libpthreadGC2.a %MINGW%\lib

cd %ABROOT%