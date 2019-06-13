/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_string - string operations.
 */

#include <zeda/zeda_string.h>
#include <ctype.h>

static const char *znullstring = "";
char *zNullStr(void){ return (char *)znullstring; }

/* copy a string supposing the destination area has enough size. */
char *zStrCopyNC(char *dest, const char *src)
{
  /* length should include the terminating \0 */
  return memmove( dest, src, strlen(src)+1 );
}

/* copy a string. */
char *zStrCopy(char *dest, const char *src, size_t size)
{
  size_t len;

  if( ( len = strlen(src) ) > size - 1 ){
    len = size - 1; /* clamped if length is beyond the buffer size */
  }
  dest[len] = '\0'; /* surely terminate the string */
  return memmove( dest, src, len ); 
}

/* clone a string. */
char *zStrClone(char str[])
{
  char *buf;

  if( !str ) return NULL;
  if( !( buf = zAlloc( char, strlen(str)+1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zStrCopyNC( buf, str );
  return buf;
}

/* concatenate a string with another. */
char *zStrCat(char *dest, const char *src, size_t size)
{
  size_t ld;

  ld = strlen(dest);
  return zStrCopy( dest+ld, src, size-ld );
}

/* concatenate a string with a charactor. */
char *zStrAddChar(char *str, size_t size, char c)
{
  int len;

  if( ( len = strlen(str) ) >= size - 1 ){
    ZRUNWARN( ZEDA_WARN_BUF_EXHAUSTED );
    return NULL;
  }
  str[len] = c;
  str[len+1] = '\0';
  return str;
}

/* insert a charactor into a string. */
char *zStrInsChar(char *str, size_t size, int cur, char c)
{
  int len;

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
char *zStrOvrChar(char *str, size_t size, int cur, char c)
{
  if( cur >= strlen(str) )
    return zStrAddChar( str, size, c );
  if( cur < 0 ) cur = 0;
  *(str+cur) = c;
  return str + cur;
}

/* delete a charactor from a string. */
char *zStrDelChar(char *str, int cur)
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
char *zToUpper(char *src, char *dest)
{
  char *cp = dest;

  while( *src ) *cp++ = toupper( *src++ );
  *cp = '\0';
  return dest;
}

/* convert charactors in a string to lowercases. */
char *zToLower(char *src, char *dest)
{
  char *cp = dest;

  while( *src ) *cp++ = tolower( *src++ );
  *cp = '\0';
  return dest;
}

#ifndef __KERNEL__
static char zdelimiter_default[] = {
  EOF, '\t', '\v', '\f', '\n', '\r',
  ' ', ',', ':', ';', '|', '(', ')', '{', '}', '\0'
};
static char *zdelimiter = zdelimiter_default;

/* specify a set of delimiters. */
void zSetDelimiter(char s[]){ zdelimiter = s; }

/* reset a set of delimiters. */
void zResetDelimiter(void){ zSetDelimiter( zdelimiter_default ); }

static char zoperator_default[] = {
  '!', '%', '&', '*', '+', '-', '/', '<', '=', '>',
  '?', '@', '\\', '^', '~', '\0' };
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

/* skip whitespaces in file. */
char zFSkipWS(FILE *fp)
{
  char c;

  do{
    if( ( c = fgetc( fp ) ) == EOF ) return (char)0;
  } while( zIsWS(c) );
  ungetc( c, fp );
  return c;
}

/* skip whitespaces in string. */
char *zSSkipWS(char *str)
{
  for( ; *str && zIsWS(*str); str++ );
  return str;
}

/* skip certain charactors in file. */
char zFSkipIncludedChar(FILE *fp, char *s)
{
  char c;

  do{
    if( ( c = fgetc( fp ) ) == EOF ) return (char)0;
  } while( zIsIncludedChar( c, s ) );
  ungetc( c, fp );
  return c;
}

/* skip certain charactors in string. */
char *zSSkipIncludedChar(char *str, char *s)
{
  for( ; *str && zIsIncludedChar(*str,s); str++ );
  return str;
}

/* skip delimiters in file. */
char zFSkipDelimiter(FILE *fp)
{
  return zFSkipIncludedChar( fp, zdelimiter );
}

/* skip delimiters in string. */
char *zSSkipDelimiter(char *str)
{
  return zSSkipIncludedChar( str, zdelimiter );
}

/* skip comments in file. */
char zFSkipComment(FILE *fp, char ident)
{
  char c;
  char dummy[BUFSIZ];

  while( 1 )
    switch( ( c = fgetc( fp ) ) ){
    case ' ': case '\t': zFSkipWS( fp ); break;
    case '\n': case '\r': break;
    case EOF: return (char)0;
    default:
      if( c == ident ){
        if( !fgets( dummy, BUFSIZ, fp ) ) return (char)0;
      } else{
        ungetc( c, fp );
        return c;
      }
    }
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
static char *_zFString(FILE *fp, char *tkn, size_t size);
static char *_zSString(char *str, char *tkn, size_t size);

/* get a string from file. */
char *_zFString(FILE *fp, char *tkn, size_t size)
{
  register int i;

  if( size <= 1 ) return NULL;
  size--;
  for( i=0; !feof(fp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_STR );
      i = zMax( size, 0 );
      break;
    }
    tkn[i] = fgetc( fp );
    if( zIsQuotation(tkn[i]) && ( i == 0 || tkn[i-1] != '\\' ) )
      break;
  }
  tkn[i] = '\0';
  return tkn;
}

/* get a string from string. */
char *_zSString(char *str, char *tkn, size_t size)
{
  register int i;
  char *sp;

  size--;
  for( sp=str, i=0; *sp; sp++, i++ ){
    if( zIsQuotation(*sp) && ( i == 0 || tkn[i-1] != '\\' ) )
      break;
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_STR );
      i = zMax( size, 0 );
      break;
    }
    tkn[i] = *sp;
  }
  tkn[i] = '\0';
  zStrCopyNC( str, sp );
  return tkn;
}

/* get a token in file. */
char *zFToken(FILE *fp, char *tkn, size_t size)
{
  register int i;

  *tkn = '\0';
  if( !zFSkipDelimiter( fp ) ) return NULL;
  *tkn = fgetc( fp );
  if( zIsQuotation( *tkn ) )
    return _zFString( fp, tkn, size );
  size--;
  for( i=1; !feof(fp); i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_TKN );
      i = zMax( size, 0 );
      break;
    }
    if( zIsDelimiter( ( tkn[i] = fgetc( fp ) ) ) ) break;
  }
  tkn[i] = '\0';
  return tkn;
}

/* get a token in a string. */
char *zSToken(char *str, char *tkn, size_t size)
{
  register int i;
  char *sp;

  *tkn = *( sp = zSSkipDelimiter( str ) );
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
      i = zMax( size, 0 );
      break;
    }
    tkn[i] = *sp++;
  }
  tkn[i] = '\0';
  zStrCopyNC( str, sp );
  return tkn;
}

/* get a token that represents an integer number from file. */
char *zFIntToken(FILE *fp, char *tkn, size_t size)
{
  register int i;

  size--;
  for( i=0; ; i++ ){
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_NUM );
      i = zMax( size, 0 );
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

static char *_zFFractToken(FILE *fp, char *tkn, size_t size);
static char *_zFUnsignedToken(FILE *fp, char *tkn, size_t size);
static char *_zFSignedToken(FILE *fp, char *tkn, size_t size);

/* get a token that represents a fraction part from file. */
char *_zFFractToken(FILE *fp, char *tkn, size_t size)
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
char *_zFUnsignedToken(FILE *fp, char *tkn, size_t size)
{
  int l;

  if( ( l = strlen( zFIntToken( fp, tkn, size ) ) ) < size - 1 )
    _zFFractToken( fp, tkn+l, size-l );
  return tkn;
}

/* get a token that represents a signed real number from file. */
char *_zFSignedToken(FILE *fp, char *tkn, size_t size)
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
  int l;
  long pos;

  if( !*_zFSignedToken( fp, tkn, size ) ) goto END;
  if( ( l = strlen( tkn ) + 1 ) >= size ) goto END;
  pos = ftell( fp );
  c = fgetc( fp );
  if( c == 'e' || c == 'E' ){
    if( !*_zFSignedToken( fp, tkn+l, size-l ) ){
      fseek( fp, pos, SEEK_SET );
      goto END;
    }
    tkn[l-1] = c;
  } else
    ungetc( c, fp );
 END:
  return tkn;
}

/* get a token that represents an integer number from string. */
char *zSIntToken(char *str, char *tkn, size_t size)
{
  register int i;
  char *sp;

  size--;
  for( sp=str, i=0; isdigit(*sp); tkn[i++]=*sp++ )
    if( i >= size ){
      ZRUNWARN( ZEDA_WARN_TOOLNG_NUM );
      i = zMax( size, 0 );
      break;
    }
  tkn[i] = '\0';
  zStrCopyNC( str, sp );
  return tkn;
}

static char *_zSFractToken(char *str, char *tkn, size_t size);
static char *_zSUnsignedToken(char *str, char *tkn, size_t size);
static char *_zSSignedToken(char *str, char *tkn, size_t size);

/* get a token that represents a fraction part from string. */
char *_zSFractToken(char *str, char *tkn, size_t size)
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
char *_zSUnsignedToken(char *str, char *tkn, size_t size)
{
  int l;

  if( ( l = strlen( zSIntToken( str, tkn, size ) ) ) < size - 1 )
    _zSFractToken( str, tkn+l, size-l );
  return tkn;
}

/* get a token that represents a signed real number from string. */
char *_zSSignedToken(char *str, char *tkn, size_t size)
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
  int l;
  char *sp;

  sp = str;
  if( !*_zSSignedToken( sp, tkn, size ) ) goto END;
  if( ( l = strlen( tkn ) + 1 ) >= size ) goto END;
  if( *sp && ( *sp == 'e' || *sp == 'E' ) ){
    if( !*_zSSignedToken( sp+1, tkn+l, size-l ) ) goto END;
    tkn[l-1] = *sp++;
  }
 END:
  zStrCopyNC( str, sp );
  return tkn;
}

/* get an integer value from file. */
int zFInt(FILE *fp)
{
  char buf[BUFSIZ];
  return zFToken( fp, buf, BUFSIZ ) ? atoi( buf ) : 0;
}

/* get an integer value from string. */
int zSInt(char *str)
{
  char buf[BUFSIZ];
  return zSToken( str, buf, BUFSIZ ) ? atoi( buf ) : 0;
}

/* get a double-precision floating-point value from file. */
double zFDouble(FILE *fp)
{
  char buf[BUFSIZ];
  return zFToken( fp, buf, BUFSIZ ) ? atof( buf ) : 0;
}

/* get a double-precision floating-point value from string. */
double zSDouble(char *str)
{
  char buf[BUFSIZ];
  return zSToken( str, buf, BUFSIZ ) ? atof( buf ) : 0;
}

/* check if a token is a tag. */
bool zTokenIsTag(char *tkn)
{
  return tkn && ( tkn[0] == '[' && tkn[strlen(tkn)-1] == ']' )
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

/* count specified tags in file. */
int zFCountTag(FILE *fp, char *tag)
{
  char buf[BUFSIZ];
  int count = 0;

  while( !feof( fp ) )
    if( zFToken( fp, buf, BUFSIZ ) && zTokenIsTag( buf ) ){
      zExtractTag( buf, buf );
      if( strcmp( buf, tag ) == 0 ) count++;
    }
  rewind( fp );
  return count;
}

/* count specified keys in file. */
int zFCountKey(FILE *fp, char *key)
{
  char buf[BUFSIZ];
  int cur, count = 0;

  cur = ftell( fp );
  while( !feof( fp ) ){
    if( zFToken( fp, buf, BUFSIZ ) && zTokenIsTag( buf ) )
      break;
    if( strcmp( buf, key ) == 0 ) count++;
  }
  fseek( fp, cur, SEEK_SET );
  return count;
}

/* scan tagged fields from a file. */
bool zTagFScan(FILE *fp, bool (* fscan_tag)(FILE*,void*,char*,bool*), void *instance)
{
  char buf[BUFSIZ];
  bool success = true;

  while( !feof( fp ) ){
    if( !zFSkipDefaultComment( fp ) ) break;
    if( zTokenIsTag( zFToken(fp,buf,BUFSIZ) ) ){
      zExtractTag( buf, buf );
      fscan_tag( fp, instance, buf, &success );
    } else
      if( !fgets( buf, BUFSIZ, fp ) ) break;
  }
  return success;
}

/* scan a field from a file. */
bool zFieldFScan(FILE *fp, bool (* fscan_field)(FILE*,void*,char*,bool*), void *instance)
{
  char buf[BUFSIZ];
  int cur = 0;
  bool success = true;

  while( !feof(fp) ){
    if( !zFSkipDefaultComment( fp ) ) break;
    cur = ftell( fp );
    if( !zFToken(fp,buf,BUFSIZ) ) break;
    if( zTokenIsTag( buf ) ){
      fseek( fp, cur, SEEK_SET );
      break;
    }
    if( !fscan_field( fp, instance, buf, &success ) )
      if( !fgets( buf, BUFSIZ, fp ) ) break;
  }
  return success;
}

#ifndef __KERNEL__
/* indent. */
void zIndentF(FILE *fp, int n)
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
char *zAddSuffix(char *org, char *suffix, char *dest, size_t size)
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
char *zGetBasename(char *org, char *name, size_t size)
{
  char *cp;

  for( cp=org+strlen(org); cp>org; cp-- )
    if( *(cp-1) == '/' || *(cp-1) == '\\' ) break;
  zStrCopy( name, cp, size );
  zCutSuffix( name );
  return name;
}

/* open file with specified suffix. */
FILE *zOpenFile(char filename[], char *suffix, char *mode)
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
static int *_zStrSearchKMPTable(char *pat, int lp);
int *_zStrSearchKMPTable(char *pat, int lp)
{
  register int i=2, j=0;
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
  register int m=0, i=0, lt, lp;
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
  register int i, j, n, m;
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
