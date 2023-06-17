/* zeda_export.h */
/* This file was automatically generated. */
/* 2023年  6月  1日 木曜日 21:20:21 JST by zhidao */
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
#endif /* __ZEDA_EXPORT_H__ */
