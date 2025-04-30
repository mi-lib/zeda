/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_pathname - pathname manipulations.
 */

#include <zeda/zeda_pathname.h>

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
char *zReplaceSuffix(const char *org, const char *suffix, char *dest, size_t size)
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
