/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_ztk - ZTK (Z's tag-and-key) file format.
 */

#include <zeda/zeda_ztk.h>

/* ********************************************************** */
/* file stack.
 *//* ******************************************************* */

/* initialize a file stack. */
void zFileStackInit(zFileStack *stack)
{
  stack->pathname = NULL;
  stack->fp = NULL;
  stack->prev = NULL;
}

/* open a new file to be pushed to a file stack. */
static zFileStack *_zFileStackNew(const char *pathname)
{
  zFileStack *cp;

  if( !( cp = zAlloc( zFileStack, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cp->fp = zOpenZTKFile( pathname, "r" ) ) ){
    free( cp );
    return NULL;
  }
  if( !( cp->pathname = zStrClone( pathname ) ) ){
    fclose( cp->fp );
    free( cp );
    return NULL;
  }
  cp->prev = NULL;
  return cp;
}

/* check if the given file is already in a file stack, and if not,
 * open the file and push it. */
zFileStack *zFileStackPush(zFileStack *head, const char *pathname)
{
  zFileStack *cp;

  if( !pathname ) return NULL;
  for( cp=head->prev; cp; cp=cp->prev )
    if( strncmp( pathname, cp->pathname, BUFSIZ ) == 0 ){
      ZRUNWARN( ZEDA_WARN_ZTK_INCLUDE_DUP, pathname );
      return NULL;
    }
  if( !( cp = _zFileStackNew( pathname ) ) ) return NULL;
  cp->prev = head->prev;
  head->prev = cp;
  return cp;
}

/* pop the latest file from a file stack. */
zFileStack *zFileStackPop(zFileStack *head)
{
  zFileStack *cp;

  if( !( cp = head->prev ) ) return NULL;
  head->prev = cp->prev;
  zFree( cp->pathname );
  fclose( cp->fp );
  free( cp );
  return head->prev;
}

/* destroy a file stack. */
void zFileStackDestroy(zFileStack *head)
{
  while( zFileStackPop( head ) );
}

/* ********************************************************** */
/* key field of ZTK format.
 *//* ******************************************************* */

/* add a value to a key field of ZTK format. */
#define ZTKKeyFieldAddVal(kf,val) zStrListAdd( &(kf)->vallist, val )

/* print out a key field of ZTK format. */
void ZTKKeyFieldFPrint(FILE *fp, ZTKKeyField *kf)
{
  zStrListCell *cp;

  fprintf( fp, "%s:", kf->key );
  zListForEach( &kf->vallist, cp ){
    fprintf( fp, " %s", cp->data );
    if( zListCellNext(cp) != zListRoot(&kf->vallist) )
      fprintf( fp, "," );
  }
  fprintf( fp, "\n" );
}

/* ********************************************************** */
/* a list of key fields of ZTK format.
 *//* ******************************************************* */

/* insert a new key field of ZTK format to a list. */
ZTKKeyFieldListCell *ZTKKeyFieldListNew(ZTKKeyFieldList *list, const char *key)
{
  ZTKKeyFieldListCell *cp;

  if( !( cp = zAlloc( ZTKKeyFieldListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cp->data.key = zStrClone( key ) ) ){
    ZALLOCERROR();
    free( cp );
    return NULL;
  }
  zListInit( &cp->data.vallist );
  zListInsertHead( list, cp );
  return cp;
}

/* destroy the head of a list of key fields of ZTK format. */
void ZTKKeyFieldListHeadDestroy(ZTKKeyFieldList *list)
{
  ZTKKeyFieldListCell *cp;

  zListDeleteHead( list, &cp );
  free( cp->data.key );
  zStrListDestroy( &cp->data.vallist );
  free( cp );
}

/* destroy a list of key fields of ZTK format. */
void ZTKKeyFieldListDestroy(ZTKKeyFieldList *list)
{
  while( !zListIsEmpty( list ) )
    ZTKKeyFieldListHeadDestroy( list );
}

/* ********************************************************** */
/* tagged field of ZTK format.
 *//* ******************************************************* */

/* print out a tagged field of ZTK format (for debug). */
void ZTKTagFieldFPrint(FILE *fp, ZTKTagField *tf)
{
  ZTKKeyFieldListCell *cp;

  fprintf( fp, "[%s]\n", tf->tag );
  zListForEach( &tf->kflist, cp )
    ZTKKeyFieldFPrint( fp, &cp->data );
}

/* ********************************************************** */
/* a list of tagged fields of ZTK format.
 *//* ******************************************************* */

/* insert a new tagged field of ZTK format to a list. */
ZTKTagFieldListCell *ZTKTagFieldListNew(const char *buf)
{
  ZTKTagFieldListCell *cp;

  if( !( cp = zAlloc( ZTKTagFieldListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cp->data.tag = zStrClone( buf ) ) ){
    ZALLOCERROR();
    free( cp );
    return NULL;
  }
  zListInit( &cp->data.kflist );
  return cp;
}

/* destroy a list of tagged fields of ZTK format. */
void ZTKTagFieldListDestroy(ZTKTagFieldList *list)
{
  ZTKTagFieldListCell *cp;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &cp );
    free( cp->data.tag );
    ZTKKeyFieldListDestroy( &cp->data.kflist );
    free( cp );
  }
}

/* print out a list of tagged fields of ZTK format (for debug). */
void ZTKTagFieldListFPrint(FILE *fp, ZTKTagFieldList *list)
{
  ZTKTagFieldListCell *cp;

  zListForEach( list, cp )
    ZTKTagFieldFPrint( fp, &cp->data );
}

/* ********************************************************** */
/* ZTK format processor.
 *//* ******************************************************* */

/* a initialize ZTK format processor. */
ZTK *ZTKInit(ZTK *ztk)
{
  zFileStackInit( &ztk->fs );
  zListInit( &ztk->tflist );
  ztk->tf_cp = NULL;
  ztk->kf_cp = NULL;
  ztk->val_cp = NULL;
  return ztk;
}

/* destroy a ZTK format processor. */
void ZTKDestroy(ZTK *ztk)
{
  zFileStackDestroy( &ztk->fs );
  ZTKTagFieldListDestroy( &ztk->tflist );
}

/* add a new tag to a ZTK format processor. */
bool ZTKAddTag(ZTK *ztk, const char *tag)
{
  if( !( ztk->tf_cp = ZTKTagFieldListNew( tag ) ) ) /* allocate a new tagged field */
    return false;
  zListInsertHead( &ztk->tflist, ztk->tf_cp );
  ztk->kf_cp = NULL; /* unactivate the key field */
  return true;
}

/* add a new key to the current tag field of a ZTK format processor. */
bool ZTKAddKey(ZTK *ztk, const char *key)
{
  return ( ztk->kf_cp = ZTKKeyFieldListNew( &ztk->tf_cp->data.kflist, key ) ) ? true : false;
}

/* delete a key at the head of the current tag field of a ZTK format processor. */
void ZTKDelKey(ZTK *ztk)
{
  ZTKKeyFieldListHeadDestroy( &ztk->tf_cp->data.kflist );
  ztk->kf_cp = zListIsEmpty( &ztk->tf_cp->data.kflist ) ? NULL : zListHead( &ztk->tf_cp->data.kflist );
}

/* add a new value to the current key field of a ZTK format processor. */
bool ZTKAddVal(ZTK *ztk, const char *val)
{
  return ZTKKeyFieldAddVal( &ztk->kf_cp->data, val ) ? true : false;
}

/* add a new integer value to the current key field of a ZTK format processor. */
bool ZTKAddInt(ZTK *ztk, const int val)
{
  char buf[BUFSIZ];
  sprintf( buf, "%d", val );
  return ZTKAddVal( ztk, buf );
}

/* add a new double-precision floating-point value value to the current key field of a ZTK format processor. */
bool ZTKAddDouble(ZTK *ztk, const double val)
{
  char buf[BUFSIZ];
  sprintf( buf, "%.10g", val );
  return ZTKAddVal( ztk, buf );
}

/* internally scan and parse a file into a tag-and-key list of a ZTK format processor. */
bool _ZTKParse(ZTK *ztk, const char *path)
{
  bool ret = true;
  zFileStack *fs;

  if( !( fs = zFileStackPush( &ztk->fs, path ) ) ) return false;
  ret = ZTKParseFP( ztk, fs->fp );
  zFileStackPop( &ztk->fs );
  return ret;
}

/* scan and parse a file stream into a tag-and-key list of a ZTK format processor. */
bool ZTKParseFP(ZTK *ztk, FILE *fp)
{
  char buf[BUFSIZ];
  bool ret = true;

  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    if( zStrIsTag( buf ) ){
      zExtractTag( buf, buf );
      if( !ZTKAddTag( ztk, buf ) ){
        ret = false;
        break;
      }
    } else{ /* might be a key or a value */
      if( strcmp( buf, "include" ) == 0 ){ /* include a file */
        _ZTKParse( ztk, zFToken(fp,buf,BUFSIZ) );
        continue;
      }
      if( !ztk->tf_cp )
        if( !ZTKAddTag( ztk, "" ) ) continue; /* tagged field unactivated. */
      if( zFPostCheckKey( fp ) ){ /* token is a key. */
        if( !ZTKAddKey( ztk, buf ) ){
          ret = false;
          break;
        }
      } else{
        if( !ztk->kf_cp ){ /* add and activate a null key field */
          if( !ZTKAddKey( ztk, "" ) ){
            ret = false;
            break;
          }
        }
        if( !ZTKAddVal( ztk, buf ) ){ /* token is a value. */
          ret = false;
          break;
        }
      }
    }
  }
  return ret;
}

/* scan and parse a file into a tag-and-key list of a ZTK format processor. */
bool ZTKParse(ZTK *ztk, const char *path)
{
  ZTKInit( ztk );
  return _ZTKParse( ztk, path );
}

/* count the number of tagged fields with a specified tag in a tag-and-key list of a ZTK format processor. */
int ZTKCountTag(ZTK *ztk, const char *tag)
{
  ZTKTagFieldListCell *cp;
  int count = 0;

  zListForEach( &ztk->tflist, cp )
    if( strcmp( cp->data.tag, tag ) == 0 ) count++;
  return count;
}

/* count the number of key fields with a specified key of the current tagged field in a tag-and-key list of a ZTK format processor. */
int ZTKCountKey(ZTK *ztk, const char *key)
{
  ZTKKeyFieldListCell *cp;
  int count = 0;

  if( !ztk->tf_cp ) return 0;
  zListForEach( &ztk->tf_cp->data.kflist, cp )
    if( strcmp( cp->data.key, key ) == 0 ) count++;
  return count;
}

/* move to the next value string in the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
zStrListCell *ZTKValNext(ZTK *ztk)
{
  if( !ztk->kf_cp ) return ztk->val_cp = NULL;
  do{
    if( ztk->val_cp == zListHead(&ztk->kf_cp->data.vallist) ) return ztk->val_cp = NULL;
    ztk->val_cp = zListCellNext(ztk->val_cp);
  } while( !ztk->val_cp->data );
  return ztk->val_cp;
}

/* rewind the list of value strings of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
zStrListCell *ZTKValRewind(ZTK *ztk)
{
  if( !ztk->kf_cp ) return ztk->val_cp = NULL;
  ztk->val_cp = zListRoot(&ztk->kf_cp->data.vallist);
  return ZTKValNext( ztk );
}

/* move to the next key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
ZTKKeyFieldListCell *ZTKKeyNext(ZTK *ztk)
{
  if( !ztk->tf_cp ) return ztk->kf_cp = NULL;
  do{
    if( ztk->kf_cp == zListHead(&ztk->tf_cp->data.kflist) ) return ztk->kf_cp = NULL;
    ztk->kf_cp = zListCellNext(ztk->kf_cp);
  } while( !ZTKValRewind( ztk ) );
  return ztk->kf_cp;
}

/* rewind the list of key fields of the current tagged field in a tag-and-key list of a ZTK format processor. */
ZTKKeyFieldListCell *ZTKKeyRewind(ZTK *ztk)
{
  if( !ztk->tf_cp ) return ztk->kf_cp = NULL;
  ztk->kf_cp = zListRoot(&ztk->tf_cp->data.kflist);
  return ZTKKeyNext( ztk );
}

/* move to the next tagged field in a tag-and-key list of a ZTK format processor. */
ZTKTagFieldListCell *ZTKTagNext(ZTK *ztk)
{
  do{
    if( ztk->tf_cp == zListHead(&ztk->tflist) ) return ztk->tf_cp = NULL;
    ztk->tf_cp = zListCellNext(ztk->tf_cp);
  } while( !ZTKKeyRewind( ztk ) );
  return ztk->tf_cp;
}

/* rewind the list of tagged field in a tag-and-key list of a ZTK format processor. */
ZTKTagFieldListCell *ZTKTagRewind(ZTK *ztk)
{
  ztk->tf_cp = zListRoot(&ztk->tflist);
  return ZTKTagNext( ztk );
}

/* retrieve an integer value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
int ZTKInt(ZTK *ztk)
{
  int retval;
  char buf[BUFSIZ];

  if( !ZTKVal(ztk) ) return 0;
  strncpy( buf, ZTKVal(ztk), BUFSIZ );
  zSInt( buf, &retval );
  ZTKValNext( ztk );
  return retval;
}

/* retrieve a real value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
double ZTKDouble(ZTK *ztk)
{
  double retval;
  char buf[BUFSIZ];

  if( !ZTKVal(ztk) ) return 0;
  strncpy( buf, ZTKVal(ztk), BUFSIZ );
  zSDouble( buf, &retval );
  ZTKValNext( ztk );
  return retval;
}

/* print out ZTK to a file.
 * This function could be referred as an example of how the information in ZTK is retrieved.
 */
void ZTKFPrint(FILE *fp, ZTK *ztk)
{
  if( !ZTKTagRewind( ztk ) ){
    fprintf( fp, "(empty)\n" );
    return; /* no tag registerred */
  }
  do{
    fprintf( fp, "[%s]\n", ZTKTag(ztk) );
    do{
      fprintf( fp, "%s:", ZTKKey(ztk) );
      while( 1 ){
        fprintf( fp, " %s", ZTKVal(ztk) );
        if( ZTKValNext(ztk) )
          fprintf( fp, "," );
        else{
          fprintf( fp, "\n" );
          break;
        }
      }
    } while( ZTKKeyNext(ztk) );
    fprintf( fp, "\n" );
  } while( ZTKTagNext(ztk) );
}

/* ********************************************************** */
/* ZTKPrp
 * ********************************************************** */

/* print out information of a list of ZTK property (for debug). */
void ZTKPrpFPrint(FILE *fp, const ZTKPrp *prp, size_t prpnum)
{
  uint i;

  for( i=0; i<prpnum; i++ ){
    fprintf( fp, "property of field %s (%d)\tevaluator: %p\tgenerator: %p\n", prp[i].str, prp[i].num, prp[i]._eval, prp[i]._fprint );
  }
}

/* duplicate an array of ZTKPrp. */
ZTKPrp *ZTKPrpDup(const ZTKPrp *src, size_t prpnum)
{
  ZTKPrp *dest;

  if( !( dest = zAlloc( ZTKPrp, prpnum ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  memcpy( dest, src, sizeof(ZTKPrp)*prpnum );
  return dest;
}

/* set number of a ZTK property with the specified string. */
bool ZTKPrpSetNum(ZTKPrp *prp, size_t prpnum, const char *str, int num)
{
  uint i;

  for( i=0; i<prpnum; i++ ){
    if( strcmp( prp[i].str, str ) == 0 ){
      if( prp[i].num != -1 ){
        ZRUNERROR( ZEDA_ERR_ZTK_PRP_UNMODIFIABLE, str );
        return false;
      }
      prp[i].num = num;
      return true;
    }
  }
  ZRUNWARN( ZEDA_WARN_ZTK_PRP_NOTFOUND, str );
  return false;
}

/* evaluate a key field of a ZTK format processor based on a ZTK property. */
void *ZTKEvalKey(void *obj, void *arg, ZTK *ztk, const ZTKPrp prp[], size_t prpnum)
{
  uint i;
  int *count;

  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( !( count = zAlloc( int, prpnum ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  do{
    for( i=0; i<prpnum; i++ )
      if( ZTKKeyCmp( ztk, prp[i].str ) && prp[i]._eval ){
        if( prp[i].num > 0 && count[i] >= prp[i].num ){
          ZRUNWARN( ZEDA_WARN_ZTK_TOOMANY_KEYS, prp[i].str );
        } else{
          if( !prp[i]._eval( obj, count[i]++, arg, ztk ) ){
            ZECHO( "error while evaluating key: %s", prp[i].str );
            obj = NULL;
            goto TERMINATE;
          }
        }
        break;
      }
  } while( ZTKKeyNext(ztk) );
 TERMINATE:
  free( count );
  return obj;
}

/* print out a key field of a ZTK format processor based on a ZTK property. */
void ZTKPrpKeyFPrint(FILE *fp, void *obj, const ZTKPrp prp[], size_t prpnum)
{
  uint i;
  int j, k;
  fpos_t pos;

  for( i=0; i<prpnum; i++ )
    if( prp[i]._fprint )
      for( j=0; j<prp[i].num; j++ ){
        if( fp != stdout && fp != stderr )
          fgetpos( fp, &pos );
        fprintf( fp, "%s: ", prp[i].str );
        if( !prp[i]._fprint( fp, j, obj ) ){
          if( fp != stdout && fp != stderr )
            fsetpos( fp, &pos );
          else{
            for( k=strlen(prp[i].str)+2; k>0; k-- )
              fputc( '\b', fp );
            fprintf( fp, "\033[0K" );
          }
        }
      }
}

/* evaluate a tag field of a ZTK format processor based on a ZTK property. */
void *ZTKEvalTag(void *obj, void *arg, ZTK *ztk, const ZTKPrp prp[], size_t prpnum)
{
  uint i;
  int *count;

  if( !ZTKTagRewind( ztk ) ) return NULL;
  if( !( count = zAlloc( int, prpnum ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  for( i=0; i<prpnum; i++ ){
    ZTKTagRewind( ztk );
    if( prp[i]._eval ) do{
      if( ZTKTagCmp( ztk, prp[i].str ) ){
        if( prp[i].num > 0 && count[i] >= prp[i].num ){
          ZRUNWARN( ZEDA_WARN_ZTK_TOOMANY_TAGS, prp[i].str );
        } else{
          if( !prp[i]._eval( obj, count[i]++, arg, ztk ) ){
            ZECHO( "error while evaluating tag [%s]", prp[i].str );
            obj = NULL;
            goto TERMINATE;
          }
        }
      }
    } while( ZTKTagNext(ztk) );
  }
 TERMINATE:
  free( count );
  return obj;
}

/* print out a tag field of a ZTK format processor based on a ZTK property. */
void ZTKPrpTagFPrint(FILE *fp, void *obj, const ZTKPrp prp[], size_t prpnum)
{
  uint i;
  int j;

  for( i=0; i<prpnum; i++ )
    if( prp[i]._fprint ){
      for( j=0; j<prp[i].num; j++ ){
        fprintf( fp, "[%s]\n", prp[i].str );
        prp[i]._fprint( fp, j, obj );
      }
    }
}
