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

/* return the larger of two values. */
double zMax(double x, double y){ return _zMax( x, y ); }
/* return the smaller of two values. */
double zMin(double x, double y){ return _zMin( x, y ); }
/* return as close value in between the given lower and upper bounds. */
double zLimit(double x, double l, double u){ return _zLimit( x, l, u ); }

/* return the largest of three values. */
double zMax3(double x, double y, double z){ return _zMax3( x, y, z ); }
/* return the smallest of three values. */
double zMin3(double x, double y, double z){ return _zMin3( x, y, z ); }

/* saturate a value within a segment. */
double zBound(double x, double b1, double b2)
{
  if( b1 > b2 ) zSwap( double, b1, b2 );
  return _zLimit( x, b1, b2 );
}

/* ********************************************************** */
/* dynamic memory allocation.
 * ********************************************************** */

/* clone a memory space. */
void *zCloneMem(const void *src, size_t size)
{
  void *dest;

  if( !src ) return NULL;
  if( !( dest = malloc( size ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return memcpy( dest, src, size );
}

/* ********************************************************** */
/* file stream operations.
 * ********************************************************** */

#ifndef __KERNEL__
/* peek a charactor from file. */
int fpeek(FILE *fp)
{
  int c;

  c = fgetc( fp );
  ungetc( c, fp );
  return c;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* count the size of a file. */
size_t zFileSize(FILE *fp)
{
  size_t size;
  fpos_t pos;

  fgetpos( fp, &pos );
  fseek( fp, 0, SEEK_END );
  size = ftell( fp );
  fsetpos( fp, &pos );
  return size;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* check if two files are identical. */
bool zFileIsIdent(FILE *fp1, FILE *fp2)
{
#ifndef __WINDOWS__
  struct stat stat1, stat2;

  if( fstat( fileno( fp1 ), &stat1 ) != 0 ||
      fstat( fileno( fp2 ), &stat2 ) != 0 ){
    ZRUNERROR( ZEDA_ERR_CANNOTGETFILESTAT );
    return false;
  }
  return ( stat1.st_ino == stat2.st_ino ) && ( stat1.st_dev == stat2.st_dev );
#else /* __WINDOWS__ */
  BY_HANDLE_FILE_INFORMATION info1, info2;

  HANDLE h1 = (HANDLE)_get_osfhandle( _fileno(fp1) );
  HANDLE h2 = (HANDLE)_get_osfhandle( _fileno(fp2) );
  if( h1 == INVALID_HANDLE_VALUE || h2 == INVALID_HANDLE_VALUE ){
    ZRUNERROR( ZEDA_ERR_CANNOTGETFILEHANDLE );
    return false;
  }
  if( !GetFileInformationByHandle( h1, &info1 ) ||
      !GetFileInformationByHandle( h2, &info2 ) ){
    ZRUNERROR( ZEDA_ERR_CANNOTGETFILEINFO );
    return false;
  }
  return ( info1.dwVolumeSerialNumber == info2.dwVolumeSerialNumber ) &&
         ( info1.nFileIndexHigh == info2.nFileIndexHigh ) &&
         ( info1.nFileIndexLow == info2.nFileIndexLow );
#endif /* __WINDOWS__ */
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* compare two files. */
long zFileCompare(const char *filename1, const char *filename2)
{
  FILE *fp1, *fp2;
  char data1, data2;
  long ret = 0;

  if( !( fp1 = fopen( filename1, "rb" ) ) ){
    ZOPENERROR( filename1 );
    return -1;
  }
  if( !( fp2 = fopen( filename2, "rb" ) ) ){
    ZOPENERROR( filename2 );
    fclose( fp1 );
    return -1;
  }
  while( !feof( fp1 ) && !feof( fp2 ) ){
    data1 = fgetc( fp1 );
    data2 = fgetc( fp2 );
    if( data1 != data2 ){
      ret = ftell( fp1 ) + 1;
      break;
    }
  }
  fclose( fp1 );
  fclose( fp2 );
  return ret;
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

/* convert a hexadecimal charactor to a value. */
int zA2X_c(char c)
{
  if( !isxdigit(c) ){
    ZRUNERROR( ZEDA_WARN_INVALID_HEX, c );
    return 0;
  }
  return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
}

/* convert a hexadecimal string to a value. */
int zA2X(char *str)
{
  int ret = 0;

  while( *str ){
    ret <<= 4;
    ret |= zA2X_c( *str++ );
  }
  return ret;
}

/* translate an integer value to an ASCII string. */
char *zI2A(int val, char *buf)
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

#ifdef __KERNEL__
#define _zF2AFig1Val(val) \
 ( (int)floor(val) % 10 )
#define _zF2AFig1(val,val_ten) \
 ( ( *(val_ten) = _zF2AFig1Val((val)) ) + '0' )

static void _zF2AAdvance(double val, char *fig);

/* pick up the bottom figure of the value. */
void _zF2AAdvance(double val, char *fig)
{
  if( _zF2AFig1Val(val) <= 5 ) return;
  for( ; *fig=='9' || *fig=='.'; fig-- )
    if( *fig != '.' ) *fig = '0';
  (*fig)++;
}
#endif /* __KERNEL__ */

/* translate a floating point number to an ASCII string. */
char *zF2A(double val, char *buf)
{
#ifdef __KERNEL__
#define ZF2A_FLOATSIZE 10
  int i, fig;
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
    if( ( fig = (int)floor(log10(val)) + 1 ) <= ZF2A_FLOATSIZE ){
      val *= pow( 0.1, fig-1 );
      for( i=0; i<ZF2A_FLOATSIZE+fig; val*=10, i++ ){
        val -= val10 * 10;
        if( i == fig ) *cp++ = '.';
        *cp++ = _zF2AFig1( val, &val10 );
      }
      _zF2AAdvance( val, cp-1 );
      *cp = '\0';
    } else{
      val *= pow( 0.1, fig-1 );
      *cp++ = _zF2AFig1( val, &val10 );
      *cp++ = '.';
      for( val*=10, i=0; i<ZF2A_FLOATSIZE; val*=10, i++ ){
        val -= val10 * 10;
        *cp++ = _zF2AFig1( val, &val10 );
      }
      _zF2AAdvance( val, cp-1 );
      *cp++ = 'e';
      zI2A( fig-1, cp );
    }
  } else{
    if( ( fig = -(int)floor(log10(val)) ) < ZF2A_FLOATSIZE ){
      *cp++ = '0';
      *cp++ = '.';
      for( i=1; i<fig; val*=10, i++ ) *cp++='0';
      for( i=0; i<=ZF2A_FLOATSIZE-fig; i++ ){
        val = ( val - val10 ) * 10;
        *cp++ = _zF2AFig1( val, &val10 );
      }
      _zF2AAdvance( val, cp-1 );
      *cp = '\0';
    } else{
      val *= pow( 10, fig );
      *cp++ = _zF2AFig1( val, &val10 );
      *cp++ = '.';
      for( i=0; i<ZF2A_FLOATSIZE; i++ ){
        val = ( val - val10 ) * 10;
        *cp++ = _zF2AFig1( val, &val10 );
      }
      _zF2AAdvance( val, cp-1 );
      *cp++ = 'e';
      *cp++ = '-';
      zI2A( fig, cp );
    }
  }
#else
  sprintf( buf, "%.10g", val );
#endif /* __KERNEL__ */
  return buf;
}

#ifndef __KERNEL__
/* convert an integer number to a string with blank filled by a specified charactor. */
#define ULONG_MAX_BUFSIZ 25
char *zI2AFill(int val, int size, char pat, char *buf)
{
  char _fill_buf[ULONG_MAX_BUFSIZ];
  char *cp;
  int len;

  if( ( len = strlen( zI2A( val, _fill_buf ) ) ) > size ){
    strcpy( buf, _fill_buf );
    return buf;
  }
  for( cp=buf; size>len; size--, cp++ ) *cp = pat;
  strcpy( cp, _fill_buf );
  return buf;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* convert an integer number to a string that represents an ordinal. */
char *zI2AOrdinal(int val, char *buf, size_t size)
{
  zI2A( val, buf );
  if( val % 10 == 1 && val % 100 != 11 )
    strcat( buf, "st" );
  else
  if( val % 10 == 2 && val % 100 != 12 )
    strcat( buf, "nd" );
  else
  if( val % 10 == 3 && val % 100 != 13 )
    strcat( buf, "rd" );
  else
    strcat( buf, "th" );
  return buf;
}
#endif /* __KERNEL__ */
