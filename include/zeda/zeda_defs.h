/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_defs.h
 * \brief base definitions.
 * \author Zhidao
 */

#ifndef __ZEDA_DEFS_H__
#define __ZEDA_DEFS_H__

/* to tell C++ compiler that types and declarations are in Language C */
#ifdef __cplusplus
# ifdef __BEGIN_DECLS
# undef __BEGIN_DECLS
# endif
# define __BEGIN_DECLS extern "C"{
# ifdef __END_DECLS
# undef __END_DECLS
# endif
# define __END_DECLS }
#else /* nothing done for C compiler */
# ifdef __BEGIN_DECLS
# undef __BEGIN_DECLS
# endif
# define __BEGIN_DECLS
# ifdef __END_DECLS
# undef __END_DECLS
# endif
# define __END_DECLS
#endif

#include <zeda/zeda_compat.h>

#ifndef __KERNEL__
# ifdef __cplusplus
#  include <iostream>
#  include <cstdio>
#  include <cstdlib>
#  include <cstring>
# else
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
# endif /* __cplusplus */
#include <sys/stat.h>
#endif /* __KERNEL__ */

#include <time.h>

/*!
 * \def ZDECL_STRUCT, ZDECL_UNION, ZDEF_STRUCT, ZDEF_UNION
 * defines a struct/union in a way that is compatible with C++.
 */
#ifdef __cplusplus
#define ZDECL_STRUCT( __struct_name ) struct __struct_name
#define ZDECL_UNION( __struct_name ) union __struct_name
#define ZDEF_STRUCT( __export, __struct_name ) struct __export __struct_name
#define ZDEF_UNION( __export, __struct_name )  union __export __struct_name
#else
#define ZDECL_STRUCT( __struct_name ) \
struct _##__struct_name; \
typedef struct _##__struct_name __struct_name
#define ZDECL_UNION( __struct_name ) \
union _##__struct_name; \
typedef union _##__struct_name __struct_name
#define ZDEF_STRUCT( __export, __struct_name ) \
struct _##__struct_name; \
typedef struct _##__struct_name __struct_name; \
struct _##__struct_name
#define ZDEF_UNION( __export, __struct_name ) \
union _##__struct_name; \
typedef union _##__struct_name __struct_name; \
union _##__struct_name
#endif /* __cplusplus */

__BEGIN_DECLS

/* ********************************************************** */
/* basic type & macro definitions
 * ********************************************************** */

/*! \brief default buffer size.
 *
 * BUFSIZ is defined in the standard C library. This macro is
 * validated in non-standard C environments as a moderate value.
 */
#ifndef BUFSIZ
#define BUFSIZ 512
#endif /* BUFSIZ */

#if ( defined( __cplusplus ) && __cplusplus < 201103L ) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L )
/* size-fixed integer family */
/* might be already defined in stdint.h or sys/types.h conforming to C99 */
typedef unsigned char      uint8_t;      /*!< 8-bit unsigned integer */
typedef unsigned short int uint16_t;     /*!< 16-bit unsigned integer */
typedef unsigned int       uint32_t;     /*!< 32-bit unsigned integer */
# ifdef __LP64__
typedef unsigned long int  uint64_t;     /*!< 64-bit unsigned integer */
# else
typedef unsigned long long int uint64_t; /*!< 64-bit unsigned integer */
# endif /* __LP64__ */
# ifndef _DEFINED_INT8
# define _DEFINED_INT8 1
typedef signed char        int8_t;       /*!< 8-bit signed integer */
# endif /* _DEFINED_INT8 */
typedef short int          int16_t;      /*!< 16-bit signed integer */
typedef int                int32_t;      /*!< 32-bit signed integer */
# ifdef __LP64__
typedef long int           int64_t;      /*!< 64-bit signed integer */
# else
typedef long long int      int64_t;      /*!< 64-bit signed integer */
# endif /* __LP64__ */
#else
# ifdef __cplusplus
# include <cstdint>
# else
# include <stdint.h>
# endif
#endif /* __STDC_VERSION__ */

#ifdef __SIZEOF_INT__
#define INT_BIT ( __SIZEOF_INT__ << 3 )
#else
#define INT_BIT ( sizeof(int) << 3 )
#endif /* __SIZEOF_INT__ */

#ifndef INT_MAX
#define INT_MAX ( ( ( ( 1 << ( ( INT_BIT >> 1 ) - 1 ) ) - 1 ) << ( INT_BIT >> 1 ) ) | ( ( 1 << ( INT_BIT >> 1 ) ) - 1 ) )
#endif

#ifndef UINT_MAX
#define UINT_MAX ( (uint)( ( ( ( 1 << ( INT_BIT >> 1 ) ) - 1 ) << ( INT_BIT >> 1 ) ) | ( ( 1 << ( INT_BIT >> 1 ) ) - 1 ) ) )
#endif

#if !defined(_DEFINED_BYTE) || _DEFINED_BYTE != 1
# if defined(_DEFINED_INT8) && _DEFINED_INT8 == 1
typedef int8_t         byte;   /*!< signed one-byte data. */
# else
typedef signed char    byte;   /*!< signed one-byte data. */
# endif /* _DEFINED_INT8 */
# define _DEFINED_BYTE 1
#endif /* _DEFINED_BYTE */
typedef int16_t        word;   /*!< signed two-byte data. */
typedef int32_t        dword;  /*!< signed four-byte data. */

typedef uint8_t        ubyte;  /*!< unsigned one-byte data. */
typedef uint16_t       uword;  /*!< unsigned two-byte data. */
typedef uint32_t       udword; /*!< unsigned four-byte data. */

#ifndef _DEFINED_UINT
#define _DEFINED_UINT 1
typedef unsigned int   uint;   /*!< unsigned integer. */
typedef unsigned long  ulong;  /*!< unsigned long integer. */
#endif /* _DEFINED_UINT */

#if defined( __STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* C99 */
#include <stdbool.h>
#else
# ifndef __cplusplus
/* C89 */
/*! \brief boolean type (for Language C). */
typedef ubyte bool;
/* _Bool should be overridden by bool. */
#define _Bool bool
/*! these values are available for boolean. */
enum{
  false = 0, true = 1
};
#else
/* C++ */
# endif /* __cplusplus */
#endif /* __STDC_VERSION__ */

/*! convert a boolean value to a string. */
#define zBoolStr(b) ( (b) ? "true" : "false" )

__END_DECLS

#endif /* __ZEDA_DEFS_H__ */
