/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_string - string operations.
 */

#include <zeda/zeda_string.h>
#include <ctype.h>
#include <stdarg.h>

static const char *znullstring = "";
char *zNullStr(void){ return (char *)znullstring; }

/* copy a string supposing the destination area has enough size. */
char *zStrCopyNC(char *dest, const char *src)
{
  /* length should include the terminating \0 */
  return (char *)memmove( dest, src, strlen(src)+1 );
}

/* copy a string. */
char *zStrCopy(char *dest, const char *src, size_t size)
{
  size_t len;

  if( ( len = strlen(src) ) > size - 1 ){
    len = size - 1; /* clamped if length is beyond the buffer size */
  }
  memmove( dest, src, len ); 
  dest[len] = '\0'; /* surely terminate the string */
  return dest;
}

/* clone a string. */
char *zStrClone(char *str)
{
  return str ? (char *)zClone( str, strlen(str)+1 ) : NULL;
}

/* concatenate a string with another. */
char *zStrCat(char *dest, const char *src, size_t size)
{
  size_t ld;

  ld = strlen(dest);
  return zStrCopy( dest+ld, src, size-ld );
}

/* concatenate a string with a formatted string. */
char *zStrCatPrint(char *str, size_t size, char *fmt, ...)
{
  va_list args;
  char buf[BUFSIZ];

  va_start( args, fmt );
  vsprintf( buf, fmt, args );
  va_end( args );
  return zStrCat( str, buf, size );
}

/* find a specified charactor in a string. */
char *zStrChar(const char *s, size_t size, int c)
{
  for( ; *s && size > 0; s++, size-- )
    if( *s == c ) return (char *)s;
  return NULL;
}

/* find a specified charactor in a string. */
char *zStrCharNul(const char *s, size_t size, int c)
{
  char *cp;
  size_t len;

  if( ( cp = zStrChar( s, size, c ) ) ) return cp;
  if( ( len = strlen( s ) ) <= size ) len = size;
  return (char *)&s[len];
}

/* find a specified charactor from the last in a string. */
char *zStrrChar(const char *s, size_t size, int c)
{
  size_t len;
  char *cp;

  if( ( len = strlen( s ) ) > size ) len = size;
  if( len == 0 ){
    ZRUNERROR( ZEDA_ERR_EMPTY_STRING );
    return NULL;
  }
  for( ; *( cp = (char*)s+len-1 ); len-- )
    if( *cp == c ) return cp;
  return NULL;
}

/* concatenate a string with a charactor. */
char *zStrAddChar(char *str, size_t size, char c)
{
  size_t len;

  if( ( len = strlen(str) ) >= size - 1 ){
    ZRUNWARN( ZEDA_WARN_BUF_EXHAUSTED );
    return NULL;
  }
  str[len] = c;
  str[len+1] = '\0';
  return str;
}

/* insert a charactor into a string. */
char *zStrInsChar(char *str, size_t size, uint cur, char c)
{
  size_t len;

  if( cur >= ( len = strlen(str) ) )
    return zStrAddChar( str, size, c );
  if( len >= size - 1 ){
    ZRUNWARN( ZEDA_WARN_BUF_EXHAUSTED );
    return NULL;
  }
  if( cur < 0 ) cur = 0;
  zStrCopy( str+cur+1, str+cur, size-cur-1 );
  *(str+cur) = c;
  return str + cur;
}

/* override a charactor in a string. */
char *zStrOvrChar(char *str, size_t size, uint cur, char c)
{
  if( cur >= strlen(str) )
    return zStrAddChar( str, size, c );
  if( cur < 0 ) cur = 0;
  *(str+cur) = c;
  return str + cur;
}

/* delete a charactor from a string. */
char *zStrDelChar(char *str, uint cur)
{
  char *cp;

  if( cur >= strlen(str) ) return NULL;
  if( cur < 0 ) cur = 0;
  cp = str + cur;
  return zStrCopyNC( cp, cp+1 );
}

/* backspace a string. */
char *zStrBSChar(char *str, int cur)
{
  char *cp;
  int len;

  if( cur <= 0 ) return NULL;
  if( cur >= ( len = strlen(str) ) ) cur = len;
  cp = str + cur - 1;
  return zStrCopyNC( cp, cp+1 );
}

/* cut a newline charactor at the end of a string. */
char *zCutNL(char *str)
{
  if( *( str += strlen(str)-1 ) == '\n' )
    *str = '\0';
  return str;
}

/* convert charactors in a string to uppercases. */
static char *_zStrConv(char *src, size_t size, char *dest, int (* _method)(int))
{
  char *cp;
  size_t s;

  for( cp=dest, s=0; s<size && *src; s++ )
    *cp++ = _method( *src++ );
  *cp = '\0';
  return dest;
}

/* convert charactors in a string to uppercases. */
char *zStrToUpper(char *src, size_t size, char *dest)
{
  return _zStrConv( src, size, dest, toupper );
}

/* convert charactors in a string to lowercases. */
char *zStrToLower(char *src, size_t size, char *dest)
{
  return _zStrConv( src, size, dest, tolower );
}

#ifndef __KERNEL__
static char zdelimiter_default[] = {
  EOF, '\t', '\v', '\f', '\n', '\r',
  ' ', ',', ';', ':', '|', '(', ')', '{', '}', '\0'
};
static char *zdelimiter = zdelimiter_default;

/* specify a set of delimiters. */
void zSetDelimiter(char s[]){ zdelimiter = s; }

/* reset a set of delimiters. */
void zResetDelimiter(void){ zSetDelimiter( zdelimiter_default ); }

static char zoperator_default[] = {
  '!', '%', '&', '*', '+', '-', '/', '<', '=', '>',
  '?', '@', '\\', '^', '~', '\0',
};
static char *zoperator = zoperator_default;

/* specify a set of operators. */
void zSetOperator(char s[]){ zoperator = s; }

/* reset a set of operators. */
void zResetOperator(void){ zSetOperator( zoperator_default ); }

/* check if a charactor is included in a specified set. */
bool zIsIncludedChar(char c, char *s)
{
  while( *s ) if( *s++ == c ) return true;
  return false;
}

/* check if a charactor is a delimiter. */
bool zIsDelimiter(char c)
{
  return zIsIncludedChar( c, zdelimiter );
}

/* check if a charactor is an operator. */
bool zIsOperator(char c)
{
  return zIsIncludedChar( c, zoperator );
}

/* check if a string represents a hexadecimal number. */
bool zStrIsHex(char *str)
{
  for( ; *str; str++ )
    if( !isxdigit( *str ) ) return false;
  return true;
}

/* skip whitespaces in a file. */
char zFSkipWS(FILE *fp)
{
  char c;

  do{
    if( ( c = fgetc( fp ) ) == EOF ) return (char)0;
  } while( zIsWS(c) );
  ungetc( c, fp );
  return c;
}

/* skip whitespaces in a string. */
char *zSSkipWS(char *str)
{
  for( ; *str && zIsWS(*str); str++ );
  return str;
}

/* skip certain charactors in a file. */
char zFSkipIncludedChar(FILE *fp, char *s)
{
  char c;

  do{
    if( ( c = fgetc( fp ) ) == EOF ) return (char)0;
  } while( zIsIncludedChar( c, s ) );
  ungetc( c, fp );
  return c;
}

/* skip certain charactors in a string. */
char *zSSkipIncludedChar(char *str, char *s)
{
  for( ; *str && zIsIncludedChar(*str,s); str++ );
  return str;
}

/* skip delimiters in a file. */
char zFSkipDelimiter(FILE *fp)
{
  return zFSkipIncludedChar( fp, zdelimiter );
}

/* skip delimiters in a string. */
char *zSSkipDelimiter(char *str)
{
  return zSSkipIncludedChar( str, zdelimiter );
}

static char zcommentident = ZDEFAULT_COMMENT_IDENT;

/* specify the comment identifier. */
void zSetCommentIdent(char ident){ zcommentident = ident; }

/* reset the comment identifier. */
void zResetCommentIdent(void){ zSetCommentIdent( ZDEFAULT_COMMENT_IDENT ); }

/* skip comments in a file. */
char zFSkipComment(FILE *fp)
{
  char c;
  char dummy[BUFSIZ];

  while( 1 ){
    if( !zFSkipDelimiter( fp ) ) return (char)0;
    if( ( c = fgetc( fp ) ) == zcommentident ){
      if( !fgets( dummy, BUFSIZ, fp ) ) return (char)0;
    } else{
      ungetc( c, fp );
      return c;
    }
  }
  return 0; /* never reaches this statement */
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* get a string from file. */
static char *_zFString(FILE *fp, char *tkn, size_t size)
{
  uint i;

  if( size <= 1 ) return NULL;
  size--; /* for the null charactor */
  for( i=0; !feof(fp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_STR );
      i = _zMax( size, 0 );
      break;
    }
    tkn[i] = fgetc( fp );
    if( zIsQuotation( tkn[i] ) && ( i == 0 || tkn[i-1] != '\\' ) )
      break;
  }
  tkn[i] = '\0';
  return tkn;
}

/* get a string from string and return a pointer immediately after the string. */
static char *_zSString(char *str, char *tkn, size_t size)
{
  uint i;
  char *sp;

  size--; /* for the null charactor */
  for( sp=str, i=0; *sp; sp++, i++ ){
    if( zIsQuotation( *sp ) && ( i == 0 || tkn[i-1] != '\\' ) )
      break;
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_STR );
      i = _zMax( size, 0 );
      break;
    }
    tkn[i] = *sp;
  }
  tkn[i] = '\0';
  return sp+1;
}

/* get a token in a file. */
char *zFToken(FILE *fp, char *tkn, size_t size)
{
  uint i;

  *tkn = '\0'; /* initialize buffer */
  if( !zFSkipComment( fp ) ) return NULL;
  *tkn = fgetc( fp );
  if( zIsQuotation( *tkn ) )
    return _zFString( fp, tkn, size );
  size--;
  for( i=1; !feof(fp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_TKN );
      i = _zMax( size, 0 );
      break;
    }
    if( zIsDelimiter( ( tkn[i] = fgetc( fp ) ) ) ){
      ungetc( tkn[i], fp );
      break;
    }
  }
  tkn[i] = '\0';
  return tkn;
}

/* skim a token in a string. */
char *zSTokenSkim(char *str, char *tkn, size_t size)
{
  uint i;
  char *sp;

  if( !( *tkn = *( sp = zSSkipDelimiter( str ) ) ) ) return sp;
  if( zIsQuotation( *sp ) ){
    zStrCopyNC( str, sp+1 );
    return _zSString( str, tkn, size );
  }
  if( *sp++ == '\0' ){
    *str = '\0';
    return NULL;
  }
  size--;
  for( i=1; *sp && !zIsDelimiter(*sp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_TKN );
      i = _zMax( size, 0 );
      break;
    }
    tkn[i] = *sp++;
  }
  tkn[i] = '\0';
  return sp;
}

/* get a token in a string. */
char *zSToken(char *str, char *tkn, size_t size)
{
  zStrCopyNC( str, zSTokenSkim( str, tkn, size ) );
  return tkn;
}

/* get a token that represents an integer number from file. */
char *zFIntToken(FILE *fp, char *tkn, size_t size)
{
  uint i;

  size--;
  for( i=0; ; i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_NUM );
      i = _zMax( size, 0 );
      break;
    }
    if( ( tkn[i] = fgetc(fp) ) == EOF ) break;
    if( !isdigit(tkn[i]) ){
      ungetc( tkn[i], fp );
      break;
    }
  }
  tkn[i] = '\0';
  return tkn;
}

/* get a token that represents a fraction part from file. */
static char *_zFFractToken(FILE *fp, char *tkn, size_t size)
{
  if( size <= 1 )
    *tkn = '\0';
  else{
    if( ( *tkn = fgetc( fp ) ) == '.' ){
      zFIntToken( fp, ++tkn, size-1 );
    } else{
      ungetc( *tkn, fp );
      *tkn = '\0';
    }
  }
  return tkn;
}

/* get a token that represents an unsigned real number from file. */
static char *_zFUnsignedToken(FILE *fp, char *tkn, size_t size)
{
  size_t len;

  if( ( len = strlen( zFIntToken( fp, tkn, size ) ) ) < size - 1 )
    _zFFractToken( fp, tkn+len, size-len );
  return tkn;
}

/* get a token that represents a signed real number from file. */
static char *_zFSignedToken(FILE *fp, char *tkn, size_t size)
{
  char c;
  long pos;

  pos = ftell( fp );
  c = fgetc( fp );
  if( c != '+' && c != '-' ){
    ungetc( c, fp );
    return _zFUnsignedToken( fp, tkn, size );
  }
  if( size > 1 && *_zFUnsignedToken( fp, tkn+1, size-1 ) )
    *tkn = c;
  else{
    fseek( fp, pos, SEEK_SET );
    *tkn = '\0';
  }
  return tkn;
}

/* get a token that represents a number from file. */
char *zFNumToken(FILE *fp, char *tkn, size_t size)
{
  char c;
  size_t len;
  long pos;

  if( !*_zFSignedToken( fp, tkn, size ) ) goto END;
  if( ( len = strlen( tkn ) + 1 ) >= size ) goto END;
  pos = ftell( fp );
  c = fgetc( fp );
  if( c == 'e' || c == 'E' ){
    if( !*_zFSignedToken( fp, tkn+len, size-len ) ){
      fseek( fp, pos, SEEK_SET );
      goto END;
    }
    tkn[len-1] = c;
  } else
    ungetc( c, fp );
 END:
  return tkn;
}

/* get a token that represents an integer number from string. */
char *zSIntToken(char *str, char *tkn, size_t size)
{
  uint i;
  char *sp;

  size--;
  for( sp=str, i=0; isdigit(*sp); tkn[i++]=*sp++ )
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_NUM );
      i = _zMax( size, 0 );
      break;
    }
  tkn[i] = '\0';
  zStrCopyNC( str, sp );
  return tkn;
}

/* get a token that represents a fraction part from string. */
static char *_zSFractToken(char *str, char *tkn, size_t size)
{
  if( *str == '.' ){
    *tkn++ = *str;
    zStrCopyNC( str, str+1 );
    zSIntToken( str, tkn, size-1 );
  } else
    *tkn = '\0';
  return tkn;
}

/* get a token that represents an unsigned real number from string. */
static char *_zSUnsignedToken(char *str, char *tkn, size_t size)
{
  size_t len;

  if( ( len = strlen( zSIntToken( str, tkn, size ) ) ) < size - 1 )
    _zSFractToken( str, tkn+len, size-len );
  return tkn;
}

/* get a token that represents a signed real number from string. */
static char *_zSSignedToken(char *str, char *tkn, size_t size)
{
  if( *str != '+' && *str != '-' )
    return _zSUnsignedToken( str, tkn, size );
  if( size > 1 && *_zSUnsignedToken( str+1, tkn+1, size-1 ) ){
    *tkn = *str;
    zStrCopyNC( str, str+1 );
  } else
    *tkn = '\0';
  return tkn;
}

/* get a token that represents a number from string. */
char *zSNumToken(char *str, char *tkn, size_t size)
{
  size_t len;
  char *sp;

  sp = str;
  if( !*_zSSignedToken( sp, tkn, size ) ) goto END;
  if( ( len = strlen( tkn ) + 1 ) >= size ) goto END;
  if( *sp && ( *sp == 'e' || *sp == 'E' ) ){
    if( !*_zSSignedToken( sp+1, tkn+len, size-len ) ) goto END;
    tkn[len-1] = *sp++;
  }
 END:
  zStrCopyNC( str, sp );
  return tkn;
}

/* get an integer value from file. */
char *zFInt(FILE *fp, int *val)
{
  char buf[BUFSIZ], *ret;
  if( ( ret = zFToken( fp, buf, BUFSIZ ) ) ) *val = atoi( buf );
  return ret;
}

/* get an integer value from string. */
char *zSInt(char *str, int *val)
{
  char buf[BUFSIZ], *ret;
  if( ( ret = zSToken( str, buf, BUFSIZ ) ) ) *val = atoi( buf );
  return ret;
}

/* get a double-precision floating-point value from file. */
char *zFDouble(FILE *fp, double *val)
{
  char buf[BUFSIZ], *ret;
  if( ( ret = zFToken( fp, buf, BUFSIZ ) ) ) *val = atof( buf );
  return ret;
}

/* get a double-precision floating-point value from string. */
char *zSDouble(char *str, double *val)
{
  char buf[BUFSIZ], *ret;
  if( ( ret = zSToken( str, buf, BUFSIZ ) ) ) *val = atof( buf );
  return ret;
}

/* check byte order marker of UTF-8/16 encode files. */
#define _c_eq(c,v) ( ( 0xff & (c) ) == (v) )
zUTFType zFCheckUTFBOM(FILE *fp)
{
  char bom[3];
  zUTFType result = ZUTF_TYPE_NONE;

  rewind( fp );
  if( fread( bom, sizeof(char), 3, fp ) != 3 ) return result;
  if( _c_eq( bom[0], 0xef ) && _c_eq( bom[1], 0xbb ) && _c_eq( bom[2], 0xbf ) ){
    result = ZUTF_TYPE_UTF8;
  } else
  if( _c_eq( bom[0], 0xfe ) && _c_eq( bom[1], 0xff ) ){
    result = ZUTF_TYPE_UTF16BE;
    fseek( fp, 2, SEEK_SET );
  } else
  if( _c_eq( bom[0], 0xff ) && _c_eq( bom[1], 0xfe ) ){
    result = ZUTF_TYPE_UTF16LE;
    fseek( fp, 2, SEEK_SET );
  } else
    rewind( fp ); /* rewind if BOM is not found. */
  return result;
}
#undef _c_eq

/* for tag-and-key format */

static char ztagbeginident = ZDEFAULT_TAG_BEGIN_IDENT;
static char ztagendident   = ZDEFAULT_TAG_END_IDENT;

/* specify the tag identifiers. */
void zSetTagIdent(char begin_ident, char end_ident){
  ztagbeginident = begin_ident;
  ztagendident   = end_ident;
}

/* reset the tag identifiers. */
void zResetTagIdent(void){ zSetTagIdent( ZDEFAULT_TAG_BEGIN_IDENT, ZDEFAULT_TAG_END_IDENT ); }

/* check if a token is a tag. */
bool zTokenIsTag(char *tkn)
{
  return tkn && ( tkn[0] == ztagbeginident && tkn[strlen(tkn)-1] == ztagendident )
    ? true : false;
}

/* extract a tagged part from string. */
char *zExtractTag(char *tag, char *notag)
{
  char *cp;

  zStrCopyNC( notag, tag+1 );
  if( ( cp = strrchr( notag, ']' ) ) ) *cp = '\0';
  return notag;
}

static char zkeyident = ZDEFAULT_KEY_IDENT;

/* specify the key identifier. */
void zSetKeyIdent(char ident){ zkeyident = ident; }

/* reset the key identifier. */
void zResetKeyIdent(void){ zSetKeyIdent( ZDEFAULT_KEY_IDENT ); }

/* check if the last token is a key. */
bool zFPostCheckKey(FILE *fp)
{
  char c;

  while( ( c = fgetc( fp ) ) != EOF ){
    if( c == zkeyident ) return true;
    if( !zIsDelimiter( c ) ){
      ungetc( c, fp );
      break;
    }
  }
  return false;
}

#ifndef __KERNEL__
/* indent. */
void zFIndent(FILE *fp, int n)
{
  fprintf( fp, "%*s", n, "" );
}
#else
/* indent(for kernel module). */
void zIndent(int n)
{
  int i;

  for( i=0; i<n; i++ ) printk( " " );
}
#endif /* __KERNEL__ */

/* get suffix of a pathname. */
char *zGetSuffix(char *name)
{
  char *cp;

  return ( cp = strrchr( name, '.' ) ) ? cp+1 : NULL;
}

/* concatenate suffix to a pathname. */
char *zAddSuffix(char *org, const char *suffix, char *dest, size_t size)
{
  char *sfx;

  zStrCopy( dest, org, size );
  if( !( sfx = zGetSuffix(dest) ) || strcmp( sfx, suffix ) ){
    zStrAddChar( dest, size, '.' );
    strcat( dest, suffix );
  }
  return dest;
}

/* replace suffix of a pathname. */
char *zReplaceSuffix(char *org, char *suffix, char *dest, size_t size)
{
  char *sfx;

  zStrCopy( dest, org, size );
  if( !( sfx = zGetSuffix(dest) ) ){
    zStrAddChar( dest, size, '.' );
    strcat( dest, suffix );
  } else if( strcmp( sfx, suffix ) ){
    zStrCopy( sfx, suffix, size-(size_t)(sfx-dest) );
  }
  return dest;
}

/* cut suffix of a pathname. */
char *zCutSuffix(char *name)
{
  char *cp;

  if( ( cp = zGetSuffix( name ) ) ) *(cp-1) = '\0';
  return cp;
}

/* simplify a pathname. */
char *zGetBasename(char *path, char *name, size_t size)
{
  char *cp;

  if( !name ) return NULL;
  for( cp=path+strlen(path); cp>path; cp-- )
    if( *(cp-1) == '/' || *(cp-1) == '\\' ) break;
  zStrCopy( name, cp, size );
  zCutSuffix( name );
  return name;
}

/* directly simplify a pathname. */
char *zGetBasenameDRC(char *name)
{
  return zGetBasename( name, name, strlen(name) );
}

/* get a directory name and a file name from a path. */
int zGetDirFilename(char *path, char *dirname, char *filename, size_t size)
{
  char *cp;
  int retval = 0;

  if( !dirname || !filename ) return 0;
  for( cp=path+strlen(path); cp>path; cp-- )
    if( *(cp-1) == '/' || *(cp-1) == '\\' ) break;
  zStrCopy( filename, cp, size );
  zStrCopy( dirname, path, cp - path + 1 );
  retval |= ( *filename ? 0x1 : 0 ) | ( *dirname ? 0x2 : 0 );
  return retval;
}

/* open file with specified suffix. */
FILE *zOpenFile(char filename[], const char *suffix, const char *mode)
{
  char fullname[BUFSIZ];
  FILE *fp;

  if( ( fp = fopen( filename, mode ) ) ) return fp;
  if( suffix ){
    zAddSuffix( filename, suffix, fullname, BUFSIZ );
    if( ( fp = fopen( fullname, mode ) ) ) return fp;
  }
  ZOPENERROR( fullname );
  return NULL;
}

/* generate a table for string search by Knuth-Morris-Pratt algorithm. */
static int *_zStrSearchKMPTable(char *pat, int lp)
{
  int i=2, j=0;
  int *table;

  if( !( table = zAlloc( int, lp ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  table[0] = -1;
  if( lp == 1 ) return table;
  table[1] = 0;
  if( lp == 2 ) return table;
  while( i < lp ){
    if( pat[i-1] == pat[j] )
      table[++i] = ++j + 1;
    else
    if( j > 0 )
      j = table[j];
    else
      table[++i] = 0;
  }
  return table;
}

/* search a string by Knuth-Morris-Pratt algorithm. */
char *zStrSearchKMP(char *text, char *pat)
{
  int m=0, i=0, lt, lp;
  int *table;

  lt = strlen( text );
  lp = strlen( pat );
  if( !( table = _zStrSearchKMPTable( pat, lp ) ) ) return NULL;
  while( m + i < lt ){
    if( pat[i] == text[m+i] ){
      if( ++i == lp ) return text + m;
    } else{
      m = m + i - table[i];
      if( i > 0 ) i = table[i];
    }
  }
  return NULL;
}

/* search a string by Boyer-Moore algorithm. */
char *zStrSearchBM(char *text, char *pat)
{
  int i, j, n, m;
  ubyte skip[0x80];

  m = strlen( pat ) - 1;
  memset( skip, m+1, 0x80 );
  for( i=0; i<m; i++ )
    skip[ (int)pat[i] ] = m - i;
  n = strlen( text ) - m;
  for( i=0; i<n; i+= skip[(int)text[i+m]] )
    if( text[i+m] == pat[m] ){
      for( j=m-1; j>=0; j-- )
        if( text[i+j] != pat[j] ) break;
      if( j < 0 ) return text + i;
    }
  return NULL;
}

#endif /* __KERNEL__ */
