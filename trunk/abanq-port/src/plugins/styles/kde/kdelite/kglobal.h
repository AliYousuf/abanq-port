#ifndef __KGLOBAL_H__
#define __KGLOBAL_H__


#ifdef Q_WS_WIN
# define DLL_EXPORT __declspec(dllexport)
#else
# define DLL_EXPORT
#endif

#endif
