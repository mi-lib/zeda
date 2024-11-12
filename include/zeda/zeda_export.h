/* ../include/zeda/zeda_export.h */
/* This file was automatically generated. */
/* 2024年 11月 12日 火曜日 14:35:51 JST by zhidao */
#ifndef __ZEDA_EXPORT_H__
#define __ZEDA_EXPORT_H__
#include <zeda/zeda_compat.h>
#if defined(__WINDOWS__) && !defined(__CYGWIN__)
# if defined(__ZEDA_BUILD_DLL__)
#  define __ZEDA_EXPORT extern __declspec(dllexport)
#  define __ZEDA_CLASS_EXPORT  __declspec(dllexport)
# else
#  define __ZEDA_EXPORT extern __declspec(dllimport)
#  define __ZEDA_CLASS_EXPORT  __declspec(dllimport)
# endif
#else
# define __ZEDA_EXPORT __EXPORT
# define __ZEDA_CLASS_EXPORT
#endif
#define __ZEDA_VERSION "1.9.16"
#endif /* __ZEDA_EXPORT_H__ */
