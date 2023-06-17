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
# ifdef _MSC_VER /* Microsoft Visual C++ (or Clang) */
#  include <windows.h>
#  ifndef _DEFINED_BYTE
#  define _DEFINED_BYTE 1
#  endif
#  undef min /* undefine notorious min/max macros */
#  undef max
# endif
#endif

/* int8_t already defined in Windows. */
#ifdef __WINDOWS__
# ifndef _DEFINED_INT8
# define _DEFINED_INT8 1
# endif /* _DEFINED_INT8 */
#endif /* __WINDOWS__ */

/* NetBSD and GNU C already defines uint and ulong in sys/types.h */
#if defined(_NETBSD_SOURCE) || defined( __USE_GNU ) || defined( __USE_MISC )
#define _DEFINED_UINT 1
#endif

/* endian */
#ifdef __WINDOWS__
#define __BYTE_ORDER __BYTE_ORDER__
#define __BIG_ENDIAN __ORDER_BIG_ENDIAN__
#define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
#include <endian.h>
#endif /* __WINDOWS__ */

/* for symbol visibility in DLLs */
#ifdef __EXPORT
#undef __EXPORT
#endif

#if defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 303
# define __EXPORT extern __attribute__((visibility("default")))
#else
# define __EXPORT extern
#endif

#ifdef __FASTCALL
#undef __FASTCALL
#endif

#if __STATIC_BUILD
# define __EXPORT
#else
#if defined(__WINDOWS__) && !defined(__CYGWIN__)
# define __FASTCALL __fastcall
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 303
# define __FASTCALL extern __attribute__((regparm(3)))
#else
# define __FASTCALL extern
#endif
#endif

/* in order to create .dll for MS-Windows, add the following three lines:
#pragma data_seg( ".share" )
#pragma data_seg()
__DEF_WINDLL
 */
#ifdef __WINDOWS__
#include <windows.h>
#if __STATIC_BUILD
#define __DEF_WINDLL
#else
#define __DEF_WINDLL \
extern __declspec(dllexport) HINSTANCE _hInstance;\
HINSTANCE _hInstance;\
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
#endif /* __STATIC_BUILD */
#endif /* __WINDOWS__ */

#endif /* __ZEDA_COMPAT_H__ */
