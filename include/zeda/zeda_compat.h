/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_compat.h
 * \brief for OS compatibility.
 * \author Zhidao
 */

#ifndef __ZEDA_COMPAT_H__
#define __ZEDA_COMPAT_H__

/* for Windows */
#if defined(__Windows) || defined(_WIN16) || defined(_WIN32) || defined(__WIN32__)
# ifndef __WINDOWS__
#  define __WINDOWS__ /* unify compiler-dependent macros */
# endif
# ifdef __BORLANDC__ /* Borland C++ */
#  include <vcl.h>
# endif
# ifdef __MSC_VER /* Microsoft Visual C++ */
#  include <windows.h>
#  undef min /* undefine notorious min/max macros */
#  undef max
# endif
#endif

/* int8_t already defined in Windows. */
#ifdef __WINDOWS__
#define _DEFINED_INT8
#endif

/* NetBSD and GNU C already defines uint and ulong in sys/types.h */
#if defined(_NETBSD_SOURCE) || defined( __USE_GNU ) || defined( __USE_MISC )
#define _DEFINED_UINT
#endif

/* for symbol visibility in DLLs */
#ifdef __EXPORT
#undef __EXPORT
#endif

#ifdef __FASTCALL
#undef __FASTCALL
#endif

#if defined(__WINDOWS__) && !defined(__CYGWIN__)
# if defined(__BUILD_DLL__)
#  define __EXPORT __declspec(dllexport)
# else
#  define __EXPORT __declspec(dllimport)
# endif
# define __FASTCALL __fastcall
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 303
# define __EXPORT __attribute__((visibility("default")))
# define __FASTCALL __attribute__((regparm(3)))
#else
# define __EXPORT
# define __FASTCALL
#endif

/* in order to create .dll for MS-Windows, define __WINDOWS__ and
 add the following three lines:
 #pragma data_seg( ".share" )
 #pragma data_seg()
 __DEF_WINDLL
 */
#ifdef __WINDOWS__
#define __DEF_WINDLL \
extern HINSTANCE _hInstance;\
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void *reserved)\
{\
  return 1;\
}\
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)\
{\
  switch( reason ){\
  case DLL_PROCESS_ATTACH:\
    _hInstance = hinst;\
    break;\
  case DLL_PROCESS_DETACH:\
  default: ;\
  }\
  return TRUE;\
}
#else
#define __DEF_WINDLL
#endif /* __WINDOWS__ */

#endif /* __ZEDA_COMPAT_H__ */
