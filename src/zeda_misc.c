/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_misc - miscellanies.
 */

#include <zeda/zeda_misc.h>

/* entry point for MS-Windows .dll */
#ifdef __WINDOWS__
#pragma data_seg( ".share" )
#pragma data_seg()
__DEF_WINDLL
#endif /* __WINDOWS__ */

#include <math.h>
#include <stdarg.h>
#include <ctype.h>

/* zBound
 * - a saturated value of a segment.
 */
double zBound(double x, double b1, double b2)
{
  if( b1 > b2 ) zSwap( double, b1, b2 );
  return zLimit( x, b1, b2 );
}

/* ********************************************************** */
/* stream manipulation
 * ********************************************************** */

#ifndef __KERNEL__
/* fpeek
 * - peek charactor.
 */
int fpeek(FILE *fp)
{
  int c;

  c = fgetc( fp );
  ungetc( c, fp );
  return c;
}
#endif /* __KERNEL__ */

/* ********************************************************** */
/* error and warning messages
 * ********************************************************** */

/* echo-on/off flag */
bool __zeda_echo = true;

/* ********************************************************** */
/* value from/to string conversion
 * ********************************************************** */

/* atox_c
 * - convert hexadecimal charactor to value.
 */
int atox_c(char c)
{
  if( !isxdigit(c) ){
    ZRUNERROR( "invalid hexadecimal notation: %c", c );
    return 0;
  }
  return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
}

/* atox
 * - convert hexadecimal string to value.
 */
int atox(char *str)
{
  int ret = 0;

  while( *str ){
    ret <<= 4;
    ret |= atox_c( *str++ );
  }
  return ret;
}

#ifdef __KERNEL__
#define _ftoa_fig1_val(val) \
 ( (int)floor(val) % 10 )
#define _ftoa_fig1(val,val_ten) \
 ( ( *(val_ten) = _zftoa_fig1_val((val)) ) + '0' )

static void _ftoa_advance(double val, char *fig);

/* (static)
 * _ftoa_advance
 * - pick up the bottom figure of the value.
 */
void _ftoa_advance(double val, char *fig)
{
  if( _ftoa_fig1_val(val) <= 5 ) return;
  for( ; *fig=='9' || *fig=='.'; fig-- )
    if( *fig != '.' ) *fig = '0';
  (*fig)++;
}
#endif /* __KERNEL__ */

/* itoa
 * - integer to ASCII translator.
 */
char *itoa(int val, char *buf)
{
#ifdef __KERNEL__
  int fig, base;
  char *cp;

  cp = buf;
  if( val < 0 ){
    *cp++ = '-';
    val = -val;
  }
  if( ( base = (int)pow( 10, floor(log10(val))+1 ) ) == 0 )
    strcat( cp, "0" );
  else{
    for( fig=base/10; base>1; base=fig, fig/=10 )
      *cp++ = val % base / fig + '0';
    *cp = '\0';
  }
#else
  sprintf( buf, "%d", val );
#endif /* __KERNEL__ */
  return buf;
}

/* ftoa
 * - floating point number to ASCII translator.
 */
char *ftoa(double val, char *buf)
{
#ifdef __KERNEL__
#define Z_FTOA_FLOATSIZE 10
  register int i;
  int fig;
  double val10 = 0;
  char *cp;

  cp = buf;
  if( val < 0 ){
    *cp++ = '-';
    val = -val;
  }
  if( val == 0 )
    strcpy( buf, "0" );
  else if( val >= 1.0 ){
    if( ( fig = (int)floor(log10(val)) + 1 ) <= Z_FTOA_FLOATSIZE ){
      val *= pow( 0.1, fig-1 );
      for( i=0; i<Z_FTOA_FLOATSIZE+fig; val*=10, i++ ){
        val -= val10 * 10;
        if( i == fig ) *cp++ = '.';
        *cp++ = _ftoa_fig1( val, &val10 );
      }
      _ftoa_advance( val, cp-1 );
      *cp = '\0';
    } else{
      val *= pow( 0.1, fig-1 );
      *cp++ = _ftoa_fig1( val, &val10 );
      *cp++ = '.';
      for( val*=10, i=0; i<Z_FTOA_FLOATSIZE; val*=10, i++ ){
        val -= val10 * 10;
        *cp++ = _ftoa_fig1( val, &val10 );
      }
      _ftoa_advance( val, cp-1 );
      *cp++ = 'e';
      itoa( fig-1, cp );
    }
  } else{
    if( ( fig = -(int)floor(log10(val)) ) < Z_FTOA_FLOATSIZE ){
      *cp++ = '0';
      *cp++ = '.';
      for( i=1; i<fig; val*=10, i++ ) *cp++='0';
      for( i=0; i<=Z_FTOA_FLOATSIZE-fig; i++ ){
        val = ( val - val10 ) * 10;
        *cp++ = _ftoa_fig1( val, &val10 );
      }
      _ftoa_advance( val, cp-1 );
      *cp = '\0';
    } else{
      val *= pow( 10, fig );
      *cp++ = _ftoa_fig1( val, &val10 );
      *cp++ = '.';
      for( i=0; i<Z_FTOA_FLOATSIZE; i++ ){
        val = ( val - val10 ) * 10;
        *cp++ = _ftoa_fig1( val, &val10 );
      }
      _ftoa_advance( val, cp-1 );
      *cp++ = 'e';
      *cp++ = '-';
      itoa( fig, cp );
    }
  }
#else
  sprintf( buf, "%.10g", val );
#endif /* __KERNEL__ */
  return buf;
}

#ifndef __KERNEL__
/* itoa_fill
 * - convert integer to string with blank filled by a charactor.
 */
#define ULONG_MAX_BUFSIZ 25
char *itoa_fill(int val, int size, char pat, char *buf)
{
  static char _itoa_fill_buf[ULONG_MAX_BUFSIZ];
  register char *cp;
  int len;

  if( ( len = strlen( itoa( val, _itoa_fill_buf ) ) ) > size ){
    strcpy( buf, _itoa_fill_buf );
    return buf;
  }
  for( cp=buf; size>len; size--, cp++ ) *cp = pat;
  strcpy( cp, _itoa_fill_buf );
  return buf;
}
#endif /* __KERNEL__ */
