/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_option - command line option manipulation.
 */

#include <zeda/zeda_option.h>
#include <stdarg.h>

/* ********************************************************** */
/* CLASS: zOption
 * command line option
 * ********************************************************** */

/* zOptionHelp
 * - display command line help.
 */
void zOptionHelp(zOption *opts)
{
  zOption *op;

  for( op=opts; op->key || op->longkey; op++ ){
    if( op->key && op->longkey )
      printf( "\t-%s, --%s", op->key, op->longkey );
    else
    if( op->key )
      printf( "\t-%s\t", op->key );
    else
      printf( "\t--%s\t", op->longkey );
    if( op->arg_guide )
      printf( "\t%s", op->arg_guide );
    if( op->desc )
      printf( "\t%s", op->desc );
    printf( "\n" );
  }
}

/* zOptionDiag
 * - diagnosis option flags (for debugging).
 */
void zOptionDiag(zOption *opts)
{
  zOption *op;

  for( op=opts; op->key || op->longkey; op++ ){
    if( op->key )
      eprintf( "-%s", op->key );
    else
      eprintf( "--%s", op->longkey );
    eprintf( "-> %s [%s]\n", op->arg ? op->arg : "NULL", zBoolExpr(op->flag) );
  }
}

/* (static)
 * _zOptionFindKey, _zOptionFindLongKey
 * - find option structure by a key.
 */
static bool _zOptionFindKey(zOption *opts, char *key, zOption **cur);
static bool _zOptionFindLongKey(zOption *opts, char *key, zOption **cur);

bool _zOptionFindKey(zOption *opts, char *key, zOption **cur)
{
  zOption *op;

  for( op=opts; op->key || op->longkey; op++ )
    if( op->key && strcmp( op->key, key ) == 0 ){
      op->flag = true;
      *cur = op->arg_guide ? op : NULL;
      return true;
    }
  ZRUNWARN( "unknown option: %s", key );
  return false;
}

bool _zOptionFindLongKey(zOption *opts, char *key, zOption **cur)
{
  zOption *op;

  for( op=opts; op->key || op->longkey; op++ )
    if( op->longkey && !strcmp( op->longkey, key ) ){
      op->flag = true;
      *cur = op->arg_guide ? op : NULL;
      return true;
    }
  ZRUNWARN( "unknown option: %s", key );
  return false;
}

/* zOptionRead
 * - read command line options.
 */
bool zOptionRead(zOption *opts, char **argv, zStrList *arglist)
{
  zOption *cur;
  bool cnt;
  bool ret = true;

  zListInit( arglist );
  for( cur=NULL, cnt=false; *argv; argv++ ){
    if( (*argv)[0] != '-' || cnt ){
      if( cur ){
        cur->arg = *argv;
        cnt = false;
        cur = NULL;
      } else{
        if( arglist && !zStrListInsert( arglist, *argv, false ) ){
          ret = false;
        }
      }
      continue;
    }
    if( (*argv)[1] != '-' ){
      ret = _zOptionFindKey( opts, *argv+1, &cur );
    } else
    if( (*argv)[2] ){
      ret = _zOptionFindLongKey( opts, *argv+2, &cur );
    } else
      cnt = true;
  }
  return ret;
}

/* ********************************************************** */
/* integer range
 * ********************************************************** */

/* zParseIntRange
 * - parse integer range notation.
 */
void zParseIntRange(char *expr, int *from, int *to)
{
  char *cp;

  *from = *to = Z_INTRANGE_BORDER;
  if( !( cp = strchr( expr, '-' ) ) )
    *from = *to = atoi( expr );
  else{
    if( cp != expr )    *from = atoi( expr );
    if( *++cp != '\0' ) *to = atoi( cp );
  }
}

/* ********************************************************** */
/* commandline argument
 * ********************************************************** */

/* zGetArg
 * - get next command line argument.
 */
char *zGetArg(char ***argv)
{
  char *arg;

  if( **argv != NULL ){
    arg = **argv;
    ( *argv )++;
    return arg;
  }
  return NULL;
}
