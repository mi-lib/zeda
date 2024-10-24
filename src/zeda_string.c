/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_string - string operations.
 */

#include <zeda/zeda_string.h>
#include <ctype.h>
#include <stdarg.h>

static const char *zeda_nullstring = "";
char *zNullStr(void){ return (char *)zeda_nullstring; }

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
char *zStrClone(const char *str)
{
  return str ? zClone( str, char, strlen(str)+1 ) : NULL;
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
static char *_zStrConv(const char *src, size_t size, char *dest, int (* _method)(int))
{
  char *cp;
  size_t s;

  for( cp=dest, s=0; s<size && *src; s++ )
    *cp++ = _method( *src++ );
  *cp = '\0';
  return dest;
}

/* convert charactors in a string to uppercases. */
char *zStrToUpper(const char *src, size_t size, char *dest)
{
  return _zStrConv( src, size, dest, toupper );
}

/* convert charactors in a string to lowercases. */
char *zStrToLower(const char *src, size_t size, char *dest)
{
  return _zStrConv( src, size, dest, tolower );
}

#ifndef __KERNEL__
static const char zeda_delimiter_default[] = {
  EOF, '\t', '\v', '\f', '\n', '\r',
  ' ', ',', ';', ':', '|', '(', ')', '{', '}', '\0'
};
static const char *zeda_delimiter = zeda_delimiter_default;

/* specify a set of delimiters. */
void zSetDelimiter(const char s[]){ zeda_delimiter = s; }

/* reset a set of delimiters. */
void zResetDelimiter(void){ zSetDelimiter( zeda_delimiter_default ); }

static const char zeda_operator_default[] = {
  '!', '%', '&', '*', '+', '-', '/', '<', '=', '>',
  '?', '@', '\\', '^', '~', '\0',
};
static const char *zeda_operator = zeda_operator_default;

/* specify a set of operators. */
void zSetOperator(const char s[]){ zeda_operator = s; }

/* reset a set of operators. */
void zResetOperator(void){ zSetOperator( zeda_operator_default ); }

/* check if a charactor is included in a specified set. */
bool zIsIncludedChar(char c, const char *s)
{
  while( *s ) if( *s++ == c ) return true;
  return false;
}

/* check if a charactor is a delimiter. */
bool zIsDelimiter(char c)
{
  return zIsIncludedChar( c, zeda_delimiter );
}

/* check if a charactor is an operator. */
bool zIsOperator(char c)
{
  return zIsIncludedChar( c, zeda_operator );
}

/* check if a string represents a hexadecimal number. */
bool zStrIsHex(const char *str)
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
char zFSkipIncludedChar(FILE *fp, const char *s)
{
  char c;

  do{
    if( ( c = fgetc( fp ) ) == EOF ) return (char)0;
  } while( zIsIncludedChar( c, s ) );
  ungetc( c, fp );
  return c;
}

/* skip certain charactors in a string. */
char *zSSkipIncludedChar(char *str, const char *s)
{
  for( ; *str && zIsIncludedChar(*str,s); str++ );
  return str;
}

/* skip delimiters in a file. */
char zFSkipDelimiter(FILE *fp)
{
  return zFSkipIncludedChar( fp, zeda_delimiter );
}

/* skip delimiters in a string. */
char *zSSkipDelimiter(char *str)
{
  return zSSkipIncludedChar( str, zeda_delimiter );
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
/* get a fenced token from file. */
static char *_zFFencedToken(FILE *fp, char *tkn, size_t size, const char begin_ident, const char end_ident)
{
  uint i;

  if( size <= 1 ) return NULL;
  if( tkn[0] != begin_ident ) return NULL;
  size--; /* for the null charactor */
  for( i=1; !feof(fp); ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLONG_STRING );
      i = _zMax( size, 0 );
      break;
    }
    tkn[i++] = fgetc( fp );
    if( tkn[i-1] == end_ident && tkn[i-2] != '\\' ) break;
  }
  tkn[i] = '\0';
  return tkn;
}

/* get a fenced token from a vanilla string and return a pointer immediately after the string. */
static char *_zSFencedToken(char *str, char *tkn, size_t size, const char begin_ident, const char end_ident)
{
  uint i;
  char *sp;

  if( *str != begin_ident ) return NULL;
  tkn[0] = *str;
  for( sp=str+1, i=1; *sp; ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLONG_STRING );
      i = size - 1;
      sp--;
      break;
    }
    tkn[i++] = *sp++;
    if( tkn[i-1] == end_ident && tkn[i-2] != '\\' ) break;
  }
  tkn[i] = '\0';
  return sp;
}

/* unfence a fenced string. */
static char *_zStrUnfence(const char *src, char *dest, const char begin_ident, const char end_ident)
{
  char *cp;

  if( *src == begin_ident ) src++;
  zStrCopyNC( dest, src );
  if( ( cp = strrchr( dest, end_ident ) ) ) *cp = '\0';
  return dest;
}

static char *_zFFencedTokenAndUnfence(FILE *fp, char *tkn, size_t size, const char begin_ident, const char end_ident)
{
  if( !_zFFencedToken( fp, tkn, size, begin_ident, end_ident ) ) return NULL;
  return _zStrUnfence( tkn, tkn, begin_ident, end_ident );
}

static char *_zSFencedTokenAndUnfence(char *str, char *tkn, size_t size, const char begin_ident, const char end_ident)
{
  char *ret;

  if( !( ret = _zSFencedToken( str, tkn, size, begin_ident, end_ident ) ) ) return NULL;
  _zStrUnfence( tkn, tkn, begin_ident, end_ident );
  return ret;
}

/* get a quoted string from file. */
#define _zFString(fp,tkn,size) _zFFencedTokenAndUnfence( fp, tkn, size, '\"', '\"' )
#define _zFQuote(fp,tkn,size)  _zFFencedTokenAndUnfence( fp, tkn, size, '\'', '\'' )

/* get a quoted string from a vanilla string and return a pointer immediately after the string. */
#define _zSString(str,tkn,size) _zSFencedTokenAndUnfence( str, tkn, size, '\"', '\"' )
#define _zSQuote(str,tkn,size)  _zSFencedTokenAndUnfence( str, tkn, size, '\'', '\'' )

/* for ZTK (Z's tag-and-key) format */

static char zeda_tag_begin_ident = ZDEFAULT_TAG_BEGIN_IDENT;
static char zeda_tag_end_ident   = ZDEFAULT_TAG_END_IDENT;

/* specify the tag identifiers. */
void zSetTagIdent(char begin_ident, char end_ident){
  zeda_tag_begin_ident = begin_ident;
  zeda_tag_end_ident   = end_ident;
}

/* reset the tag identifiers to default values. */
void zResetTagIdent(void){ zSetTagIdent( ZDEFAULT_TAG_BEGIN_IDENT, ZDEFAULT_TAG_END_IDENT ); }

/* get a tagged token. */
#define _zFTag(fp,tkn,size) _zFFencedToken( fp, tkn, size, zeda_tag_begin_ident, zeda_tag_end_ident )
#define _zSTag(fp,tkn,size) _zSFencedToken( fp, tkn, size, zeda_tag_begin_ident, zeda_tag_end_ident )

/* check if a string is a tag. */
bool zStrIsTag(const char *str)
{
  return str && ( str[0] == zeda_tag_begin_ident && str[strlen(str)-1] == zeda_tag_end_ident )
    ? true : false;
}

/* extract a tagged part from string. */
char *zExtractTag(char *tag, char *notag)
{
  return _zStrUnfence( tag, notag, zeda_tag_begin_ident, zeda_tag_end_ident );
}

static char zeda_key_ident = ZDEFAULT_KEY_IDENT;

/* specify the key identifier. */
void zSetKeyIdent(char ident){ zeda_key_ident = ident; }

/* reset the key identifier. */
void zResetKeyIdent(void){ zSetKeyIdent( ZDEFAULT_KEY_IDENT ); }

/* check if the last token is a key. */
bool zFPostCheckKey(FILE *fp)
{
  char c;

  while( ( c = fgetc( fp ) ) != EOF ){
    if( c == zeda_key_ident ) return true;
    if( !zIsDelimiter( c ) ){
      ungetc( c, fp );
      break;
    }
  }
  return false;
}

/* get a token in a file. */
char *zFToken(FILE *fp, char *tkn, size_t size)
{
  uint i;

  *tkn = '\0'; /* initialize buffer */
  if( !zFSkipComment( fp ) ) return NULL;
  *tkn = fgetc( fp );
  /* check if token is fenced */
  if( _zFString( fp, tkn, size ) || _zFQuote( fp, tkn, size ) || _zFTag( fp, tkn, size ) )
    return tkn;

  size--; /* token already has one charactor. */
  for( i=1; !feof(fp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLONG_TOKEN );
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
  char *sp, *sp_next;

  if( !*( sp = zSSkipDelimiter( str ) ) ){ /* empty string case */
    *tkn = '\0';
    return sp;
  }
  /* check if token is fenced */
  if( ( sp_next = _zSString( sp, tkn, size ) ) ) return sp_next;
  if( ( sp_next = _zSQuote( sp, tkn, size ) ) ) return sp_next;
  if( ( sp_next = _zSTag( sp, tkn, size ) ) ) return sp_next;

  for( i=0; *sp && !zIsDelimiter(*sp); sp++, i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLONG_TOKEN );
      i = size - 1;
      sp--;
      break;
    }
    tkn[i] = *sp;
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
      ZRUNWARN( ZEDA_WARN_TOOLONG_NUM );
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
      ZRUNWARN( ZEDA_WARN_TOOLONG_NUM );
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
  char buf[BUFSIZ];

  if( !*zSToken( str, buf, BUFSIZ ) ) return NULL;
  *val = atoi( buf );
  return str;
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
  char buf[BUFSIZ];

  if( !*zSToken( str, buf, BUFSIZ ) ) return NULL;
  *val = atof( buf );
  return str;
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

#ifndef __KERNEL__
/* indent. */
void zFIndent(FILE *fp, int n)
{
  fprintf( fp, "%*s", n, "" );
}
#else
/* indent (for kernel module). */
void zIndent(int n)
{
  int i;

  for( i=0; i<n; i++ ) printk( " " );
}
#endif /* __KERNEL__ */

/* get suffix of a pathname. */
char *zGetSuffix(const char *name)
{
  char *cp;

  return ( cp = (char *)strrchr( name, '.' ) ) ? cp+1 : NULL;
}

/* concatenate suffix to a pathname. */
char *zAddSuffix(const char *org, const char *suffix, char *dest, size_t size)
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
FILE *zOpenFile(const char filename[], const char *suffix, const char *mode)
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
