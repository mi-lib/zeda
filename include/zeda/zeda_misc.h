﻿/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_misc.h
 * \brief miscellanies.
 * \author Zhidao
 */

#ifndef __ZEDA_MISC_H__
#define __ZEDA_MISC_H__

#include <zeda/zeda_defs.h>
#include <zeda/zeda_errmsg.h>
#include <assert.h>

__BEGIN_DECLS

/*!
 * \def zAssert( func, expr )
 * asserts \a expr and echo a message that \a func works fine;
 * if not, it aborts.
 */
#define zAssert( func, expr ) do{\
  printf( "%s ... ", #func );\
  assert( expr );\
  printf( "OK\n" );\
} while(0)\

/*!
 * \def zMax(x,y)
 * return the larger value of \a x and \a y.
 * \def zMin(x,y)
 * return the smaller value of \a x and \a y.
 * \def zLimit(x,l,u)
 * return a saturated value of \a x with the lower
 * boundary \a l and the upper boundary \a u.
 */
#define _zMax(x,y)     ( (x)>=(y) ? (x) : (y) )
__EXPORT double zMax(double x, double y);
#define _zMin(x,y)     ( (x)<=(y) ? (x) : (y) )
__EXPORT double zMin(double x, double y);
#define _zLimit(x,l,u) ( (x)<=(l) ? (l) : ( (x)>=(u) ? (u) : (x) ) )
__EXPORT double zLimit(double x, double l, double u);

/*! \brief
 * return a saturated value of \a x with two boundaries \a b1 and
 * \a b2, where the magnitude relation between \a b1 and \a b2 does
 * not matter.
 */
__EXPORT double zBound(double x, double b1, double b2);

/*! \brief
 * swap values of two data \a a and \a b.
 * \a type the data type of \a a and \a b.
 */
#define zSwap(type,a,b) do{\
  type __swap_tmp;\
  __swap_tmp = a;\
  a = b;\
  b = __swap_tmp;\
} while(0)

/* ********************************************************** */
/*! \defgroup mem_man dynamic memory allocation.
 * \{ *//* ************************************************** */

/*!
 * \def zAlloc(t,n)
 * allocate memory for \a n data of a data type \a type.
 *
 * \def zAllocZero(t,n)
 * allocate memory for \a n data of a data type \a type and clear it by zero.
 *
 * \def zFree(m)
 * free memory allocated at \a m.
 * \a m is reset to be the null pointer after freeing the memory.
 */
/* NOTE: now zAlloc is implemented with calloc but will be replaced
 * by malloc for faster operations.
 */
#define zAlloc(t,n)     ( (n) == 0 ? NULL : (t *)calloc( (n), sizeof(t) ) )
#define zAllocZero(t,n) ( (n) == 0 ? NULL : (t *)calloc( (n), sizeof(t) ) )

#define zFree(m)        do{ if( (m) ){ free( m ); (m) = NULL; } } while(0)

/*!
 * \def zCopy(t,s,d)
 * copy \a s to \a d, where both are supposed to be types of \a t.
 */
#define zCopy(t,s,d)    ( (t *)memcpy( d, s, sizeof(t) ) )

/*!
 * \def zRealloc(m,t,n)
 * reallocate memory where \a m points.
 * \a n is the number of data to be reallocated.
 * \a type is the type of the data.
 * \note zRealloc() is not available in the kernel space.
 */
#ifndef __KERNEL__
#define zRealloc(m,t,n) (t *)realloc( (void *)m, sizeof(t)*(n) )
#endif /* __KERNEL__ */

/*! \brief clone a memory space.
 *
 * zClone() clones a memory space \a src with a size \a size and
 * returns a pointer to the newly allocated memory if succeeding.
 * Otherwise, the null pointer is returned.
 */
#ifndef __KERNEL__
__EXPORT void *zClone(void *src, size_t size);
#endif /* __KERNEL__ */

/*! \} */

/*! \brief count the size of a file. */
#ifndef __KERNEL__
__EXPORT size_t zFileSize(FILE *fp);
#endif /* __KERNEL__ */

/*! \brief peek charactor.
 *
 * fpeek() picks up a charactor from the current position
 * of the file pointed by \a fp without proceeding the
 * pointer of \a fp.
 * \return the charactor picked up.
 */
#ifndef __KERNEL__
__EXPORT int fpeek(FILE *fp);
#endif /* __KERNEL__ */

/* ********************************************************** */
/*! \defgroup error error and warning messages
 * \{ *//* ************************************************** */

/*! \brief formatted output to stderr.
 *
 * eprintf() outputs a string formated by \a fmt to
 * stderr (the standard error).
 * The output format conforms to the printf() family
 * specification.
 */
#ifndef __KERNEL__
#define eprintf(fmt,...) fprintf( stderr, fmt, ##__VA_ARGS__ )
#else
#define eprintf printk
#endif /* __KERNEL__ */

/*!
 * \def zEchoOn()
 * make echo on.
 * \def zEchoOff()
 * make echo off.
 */
/*! \cond */
extern bool __zeda_echo;
/*! \endcond */
#define zEchoOn()  ( __zeda_echo = true )
#define zEchoOff() ( __zeda_echo = false )

/*!
 * \def ZRUNERROR(msg,...)
 * output a run-time error message.
 * \def ZRUNWARN(msg,...)
 * output a run-time warning message.
 * \def ZECHO(m)
 * output a run-time echo.
 * \def ZOPENERROR(m)
 * output an error message when failing to open file \a m.
 * \def ZALLOCERROR()
 * output an error message whan failing to allocate memory.
 */
#define ZECHO(msg,...)     ( __zeda_echo ? eprintf( msg " (%s).\n", ##__VA_ARGS__, __FUNCTION__ ) : 0 )
#define ZRUNERROR(msg,...) ZECHO( "run-time error: " msg, ##__VA_ARGS__ )
#define ZRUNWARN(msg,...)  ZECHO( "warning: " msg, ##__VA_ARGS__ )

#define ZOPENERROR(m) ZRUNERROR( "cannot open file: %s", (m) )
#define ZALLOCERROR() ZRUNERROR( "cannot allocate memory" )

/*! \} */

/* ********************************************************** */
/*! \defgroup val_str convert value from/to string.
 * \{ *//* ************************************************** */

/*! \brief convert a hexadecimal note to a value.
 *
 * atox_c() converts a charactor \a c which denotes a
 * hexadecimal value to an integer value.
 * \return an integer value converted.
 */
__EXPORT int atox_c(char c);

/*! \brief convert hexadecimal string to value.
 *
 * atox() converts a string \a str which denotes hexadecimal
 * value to an integer value.
 *
 * if \a c is neither 0-9 nor a-f, it warns and returns zero.
 * if \a str includes non-hexadecimal charactors, it warns and
 * skips them.
 * Ex. a string "1g2h3i" is converted to 102030.
 * \return an integer value converted.
 */
__EXPORT int atox(char *str);

/*! \brief convert an integer to a string.
 *
 * itoa() converts an integer \a val to an ASCII string
 * and copies it to an array pointed by \a buf.
 * \return a pointer \a buf.
 * \note
 * itoa() does not check the size of \a buf.
 * It assumes that \a 'buf' has an enough size.
 */
#ifndef __WINDOWS__
__EXPORT char *itoa(int val, char *buf);
#endif

/*! \brief convert a d-float value to a string.
 *
 * ftoa() converts a double-precision floating point
 * value \a val to an ASCII string and copies it to an
 * array pointed by \a buf.
 * \return a pointer \a buf.
 * \note
 * itoa() does not check the size of \a buf.
 * It assumes that \a 'buf' has an enough size.
 */
__EXPORT char *ftoa(double val, char *buf);

#ifndef __KERNEL__
/*! \brief convert an integer to a string with a blank
 * filled by a charactor.
 *
 * itoa_fill() converts a given integer \a val to a string with
 * its blank filled by \a pat.
 * \a size is the length of the string. If the digit of \a val is
 * more than \a size, \a size is ignored.
 *
 *  Examples:
 *
 *  1.when calling itoa_fill( 12, 5, '*', buf );,
 *    the string will be "***12".
 *
 *  2.when calling 'itoa_fill( 123, 2, '?', buf );',
 *    the string will be "123".
 * \return a pointer \a buf.
 * \note
 * itoa_fill() is not available in ther kernel space.
 */
__EXPORT char *itoa_fill(int val, int size, char pat, char *buf);

/*! \def itoa_zerofill
 * \brief convert an integer to a string with blank
 * filled by a charactor-zero.
 * \return a pointer \a buf.
 * \sa itoa_fill
 */
#define itoa_zerofill(v,s,b) itoa_fill( (v), (s), '0', (b) )

/*! \brief convert an integer number to a string that represents an ordinal.
 *
 * itoa_ordinal() converts an integer number \a val into a string
 * that represents an ordinal, and stores it into \a buf.
 * \a size is the size of \a buf.
 * \return
 * itoa_ordinal() returns a pointer \a buf.
 */
__EXPORT char *itoa_ordinal(int val, char *buf, size_t size);
#endif /* __KERNEL__ */

/*! \} */

__END_DECLS

#endif /* __ZEDA_MISC_H__ */
