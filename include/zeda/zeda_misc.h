/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_misc.h
 * \brief miscellanies.
 * \author Zhidao
 */

#ifndef __ZEDA_MISC_H__
#define __ZEDA_MISC_H__

#include <zeda/zeda_export.h> /* zeda_export.h is automatically generated. */

#include <zeda/zeda_defs.h>
#include <zeda/zeda_errmsg.h>

__BEGIN_DECLS

/*!
 * \def zAssert( func, expr )
 * asserts \a expr and echo a message that \a func works fine;
 * if not, it aborts.
 */
#define zAssert( func, expr ) do{\
  eprintf( "%s ... ", #func );\
  fflush( stderr );\
  if( !( expr ) ){\
    eprintf( "\x1b[31mFailed\x1b[0m\n" );\
    abort();\
  }\
  eprintf( "\x1b[32mOK\x1b[0m\n" );\
  fflush( stderr );\
} while(0)

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
__ZEDA_EXPORT double zMax(double x, double y);
#define _zMin(x,y)     ( (x)<=(y) ? (x) : (y) )
__ZEDA_EXPORT double zMin(double x, double y);
#define _zLimit(x,l,u) ( (x)<=(l) ? (l) : ( (x)>=(u) ? (u) : (x) ) )
__ZEDA_EXPORT double zLimit(double x, double l, double u);

/*! \brief
 * return a saturated value of \a x with two boundaries \a b1 and
 * \a b2, where the magnitude relation between \a b1 and \a b2 does
 * not matter.
 */
__ZEDA_EXPORT double zBound(double x, double b1, double b2);

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
 * \def zRealloc(m,t,n)
 * reallocate memory where \a m points.
 * \a n is the number of data to be reallocated.
 * \a type is the type of the data.
 * \note zRealloc() is not available in the kernel space.
 */
#ifndef __KERNEL__
#define zRealloc(m,t,n) (t *)realloc( (void *)m, sizeof(t)*(n) )
#endif /* __KERNEL__ */

#define ZDEF_ALLOC_FUNCTION_PROTOTYPE(type) type *type##Alloc(void)
#define ZDEF_ALLOC_FUNCTION(type) \
ZDEF_ALLOC_FUNCTION_PROTOTYPE( type ){ \
  type *instance; \
  if( !( instance = zAlloc( type, 1 ) ) ){ \
    ZALLOCERROR(); \
    return NULL; \
  } \
  return instance; \
}

/*!
 * \def zCopy(t,s,d)
 * copy \a s to \a d, where both are supposed to be types of \a t.
 */
#define zCopy(t,s,d) ( (t *)memcpy( d, s, sizeof(t) ) )

/*! \brief clone a memory space.
 *
 * zClone() clones a memory space \a src with a size \a size and
 * returns a pointer to the newly allocated memory if succeeding.
 * Otherwise, the null pointer is returned.
 */
#ifndef __KERNEL__
__ZEDA_EXPORT void *zClone(void *src, size_t size);
#endif /* __KERNEL__ */

/*! \} */

/*! \brief peek charactor.
 *
 * fpeek() picks up a charactor from the current position
 * of the file pointed by \a fp without proceeding the
 * pointer of \a fp.
 * \return the charactor picked up.
 */
#ifndef __KERNEL__
__ZEDA_EXPORT int fpeek(FILE *fp);
#endif /* __KERNEL__ */

/*! \brief count the size of a file. */
#ifndef __KERNEL__
__ZEDA_EXPORT size_t zFileSize(FILE *fp);
#endif /* __KERNEL__ */

/*! \brief compare two files.
 *
 * zFileSize() compares two files \a filename1 and \a filename2.
 * \return
 * zFileSize() returns the position at which the first pair of different data in
 * the files is found. If the files are the same, 0 is returned.
 * If it fails to open \a filename1 or \a filename2, -1 is returned.
 */
#ifndef __KERNEL__
__ZEDA_EXPORT long zFileCompare(const char *filename1, const char *filename2);
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
__ZEDA_EXPORT bool __zeda_echo;
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
 * zA2X_c() converts a charactor \a c which denotes a
 * hexadecimal value to an integer value.
 * \return an integer value converted.
 */
__ZEDA_EXPORT int zA2X_c(char c);

/*! \brief convert hexadecimal string to value.
 *
 * zA2X() converts a string \a str which denotes hexadecimal
 * value to an integer value.
 *
 * if \a c is neither 0-9 nor a-f, it warns and returns zero.
 * if \a str includes non-hexadecimal charactors, it warns and
 * skips them.
 * Ex. a string "1g2h3i" is converted to 102030.
 * \return an integer value converted.
 */
__ZEDA_EXPORT int zA2X(char *str);

/*! \brief convert an integer to a string.
 *
 * zI2A() converts an integer \a val to an ASCII string
 * and copies it to an array pointed by \a buf.
 * \return a pointer \a buf.
 * \note
 * zI2A() does not check the size of \a buf.
 * It assumes that \a 'buf' has an enough size.
 */
__ZEDA_EXPORT char *zI2A(int val, char *buf);

/*! \brief convert a d-float value to a string.
 *
 * zF2A() converts a double-precision floating point
 * value \a val to an ASCII string and copies it to an
 * array pointed by \a buf.
 * \return a pointer \a buf.
 * \note
 * zI2A() does not check the size of \a buf.
 * It assumes that \a 'buf' has an enough size.
 */
__ZEDA_EXPORT char *zF2A(double val, char *buf);

#ifndef __KERNEL__
/*! \brief convert an integer to a string with a blank
 * filled by a charactor.
 *
 * zI2AFill() converts a given integer \a val to a string with
 * its blank filled by \a pat.
 * \a size is the length of the string. If the digit of \a val is
 * more than \a size, \a size is ignored.
 *
 *  Examples:
 *
 *  1.when calling zI2AFill( 12, 5, '*', buf );,
 *    the string will be "***12".
 *
 *  2.when calling 'zI2AFill( 123, 2, '?', buf );',
 *    the string will be "123".
 * \return a pointer \a buf.
 * \note
 * zI2AFill() is not available in ther kernel space.
 */
__ZEDA_EXPORT char *zI2AFill(int val, int size, char pat, char *buf);

/*! \def zI2AZeroFill
 * \brief convert an integer to a string with blank
 * filled by a charactor-zero.
 * \return a pointer \a buf.
 * \sa zI2AFill
 */
#define zI2AZeroFill(v,s,b) zI2AFill( (v), (s), '0', (b) )

/*! \brief convert an integer number to a string that represents an ordinal.
 *
 * zI2AOrdinal() converts an integer number \a val into a string
 * that represents an ordinal, and stores it into \a buf.
 * \a size is the size of \a buf.
 * \return
 * zI2AOrdinal() returns a pointer \a buf.
 */
__ZEDA_EXPORT char *zI2AOrdinal(int val, char *buf, size_t size);
#endif /* __KERNEL__ */

/*! \} */

__END_DECLS

#endif /* __ZEDA_MISC_H__ */
