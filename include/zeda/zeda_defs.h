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

#ifndef __KERNEL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* __KERNEL__ */

#include <time.h>

#include <zeda/zeda_compat.h>

__BEGIN_DECLS

/* ********************************************************** */
/* basic type & macro definitions
 * ********************************************************** */

/*! \brief default buffer size.
 *
 * BUFSIZ is defined in the standard C library. This macro is
 * validated in non-standard C environments rather as a moderate
 * value.
 */
#ifndef BUFSIZ
#define BUFSIZ 512
#endif /* BUFSIZ */

/* size-fixed integer family */
/* might be already defined in stdint.h or sys/types.h conforming to C99 */
#include <bits/wordsize.h>
typedef unsigned char      uint8_t;      /*!< 8-bit unsigned integer */
typedef unsigned short int uint16_t;     /*!< 16-bit unsigned integer */

#ifndef __uint32_t_defined
#define __uint32_t_defined
typedef unsigned int       uint32_t;     /*!< 32-bit unsigned integer */
#if __WORDSIZE == 64
typedef unsigned long int  uint64_t;     /*!< 64-bit unsigned integer */
#else
typedef unsigned long long int uint64_t; /*!< 64-bit unsigned integer */
#endif
#endif

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char        int8_t;       /*!< 8-bit signed integer */
typedef short int          int16_t;      /*!< 16-bit signed integer */
typedef int                int32_t;      /*!< 32-bit signed integer */
#if __WORDSIZE == 64
typedef long int           int64_t;      /*!< 64-bit signed integer */
#else
typedef long long int      int64_t;      /*!< 64-bit signed integer */
#endif
#endif

typedef int8_t   byte;  /*!< signed one-byte data. */
typedef uint8_t  ubyte; /*!< unsigned one-byte data. */
typedef int16_t  word;  /*!< signed two-byte data. */
typedef uint16_t uword; /*!< unsigned two-byte data. */

#if !defined( _NETBSD_SOURCE )
/* NetBSD already defines uint and ulong in sys/types.h */
#if !defined( __USE_GNU ) && !defined( __USE_MISC ) && !defined( __KERNEL__ )
/* GNU C already defines uint and ulong in sys/types.h */
typedef unsigned int   uint;  /*!< unsigned integer. */
typedef unsigned long  ulong; /*!< unsigned long integer. */
#endif
#endif

#ifndef __cplusplus
/*! \brief boolean type (for Language C). */
typedef ubyte bool;
/*! these values are available for boolean. */
enum{
  false = 0, true = 1
};
#endif /* __cplusplus */

/*! convert a boolean value to a string. */
#define zBoolExpr(b) ( (b) ? "true" : "false" )

__END_DECLS

#endif /* __ZEDA_DEFS_H__ */
