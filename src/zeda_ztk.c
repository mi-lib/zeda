/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_ztk - ZTK (Z's tag-and-key) file format.
 */

#include <zeda/zeda_ztk.h>

/* ********************************************************** */
/* file stack.
 *//* ******************************************************* */

static zFileStack *_zFileStackNew(char *pathname);

/* initialize a file stack. */
void zFileStackInit(zFileStack *stack)
{
  stack->fp = NULL;
  stack->prev = NULL;
}

/* open a new file to be pushed to a file stack. */
zFileStack *_zFileStackNew(char *pathname)
{
  zFileStack *cp;

  if( !( cp = zAlloc( zFileStack, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cp->fp = fopen( pathname, "r" ) ) ){
    ZOPENERROR( pathname );
    free( cp );
    return NULL;
  }
  strncpy( cp->pathname, pathname, BUFSIZ );
  cp->prev = NULL;
  return cp;
}

/* check if the given file is already in a file stack, and if not,
 * open the file and push it. */
zFileStack *zFileStackPush(zFileStack *head, char *pathname)
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
/* definition of ZTK (a set of tag and keys).
 *//* ******************************************************* */

/* destroy a definition of ZTK. */
void ZTKDefDestroy(ZTKDef *def)
{
  free( def->tag );
  zStrListDestroy( &def->keylist );
}

/* print out a definition of ZTK (for debug). */
void ZTKDefFPrint(FILE *fp, ZTKDef *def)
{
  fprintf( fp, "[%s]\n", def->tag );
  fprintf( fp, "(keys)\n" );
  zStrListFPrint( fp, &def->keylist );
}

/* find a key in a definition of ZTK. */
#define ZTKDefFindKey(d,key) ( zStrListFindStr( &(d)->keylist, key ) ? true : false )

/* ********************************************************** */
/* a definition list of ZTK.
 *//* ******************************************************* */

/* destroy a definition list of ZTK. */
void ZTKDefListDestroy(ZTKDefList *list)
{
  ZTKDefListCell *cp;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &cp );
    free( cp->data.tag );
    zStrListDestroy( &cp->data.keylist );
    free( cp );
  }
}

/* register a new definition of ZTK to a list. */
ZTKDefListCell *ZTKDefListReg(ZTKDefList *list, char *tag, char **key, int keynum)
{
  ZTKDefListCell *cp;
  register int i;

  zListForEach( list, cp ) /* check if duplicate tag does not exist */
    if( strcmp( cp->data.tag, tag ) == 0 ){
      ZRUNWARN( ZEDA_WARN_ZTK_DUPDEF, tag );
      return NULL;
    }
  if( !( cp = zAlloc( ZTKDefListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cp->data.tag = zStrClone( tag ) ) ){
    ZALLOCERROR();
    free( cp );
    return NULL;
  }
  zListInit( &cp->data.keylist );
  for( i=0; i<keynum; i++ )
    if( !zStrListInsert( &cp->data.keylist, key[i] ) ){
      ZTKDefDestroy( &cp->data );
      free( cp );
      return NULL;
    }
  zListInsertHead( list, cp );
  return cp;
}

/* find tag in a definition list of ZTK. */
ZTKDefListCell *ZTKDefListFindTag(ZTKDefList *list, char *tag)
{
  ZTKDefListCell *cp;

  zListForEach( list, cp )
    if( strcmp( cp->data.tag, tag ) == 0 ) return cp;
  return NULL;
}

/* print out a definition list of ZTK (for debug). */
void ZTKDefListFPrint(FILE *fp, ZTKDefList *list)
{
  ZTKDefListCell *cp;

  zListForEach( list, cp )
    ZTKDefFPrint( fp, &cp->data );
}

/* ********************************************************** */
/* key field of ZTK format.
 *//* ******************************************************* */

/* add a value to a key field of ZTK format. */
#define ZTKKeyFieldAddVal(kf,val) zStrListInsert( &(kf)->vallist, val )

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
ZTKKeyFieldListCell *ZTKKeyFieldListNew(ZTKKeyFieldList *list, char *key)
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

/* destroy a list of key fields of ZTK format. */
void ZTKKeyFieldListDestroy(ZTKKeyFieldList *list)
{
  ZTKKeyFieldListCell *cp;

  while( !zListIsEmpty( list ) ){
    zListDeleteHead( list, &cp );
    free( cp->data.key );
    zStrListDestroy( &cp->data.vallist );
    free( cp );
  }
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
ZTKTagFieldListCell *ZTKTagFieldListNew(char buf[])
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
/* parser class of ZTK format.
 *//* ******************************************************* */

/* initialize ZTK. */
ZTK *ZTKInit(ZTK *ztk)
{
  zFileStackInit( &ztk->fs );
  zListInit( &ztk->deflist );
  ztk->def = NULL;
  zListInit( &ztk->tflist );
  ztk->tf_cp = NULL;
  ztk->kf_cp = NULL;
  ztk->val_cp = NULL;
  return ztk;
}

/* destroy ZTK. */
void ZTKDestroy(ZTK *ztk)
{
  zFileStackDestroy( &ztk->fs );
  ZTKDefListDestroy( &ztk->deflist );
  ZTKTagFieldListDestroy( &ztk->tflist );
}

/* scan a file and parse ZTK format into a list of tagged fields. */
bool ZTKParse(ZTK *ztk, char *path)
{
  char buf[BUFSIZ];
  bool ret = true;
  zFileStack *fs;

  if( !( fs = zFileStackPush( &ztk->fs, path ) ) ) return false;
  while( !feof( fs->fp ) ){
    if( !zFSkipDefaultComment( fs->fp ) ) break;
    if( !zFToken( fs->fp, buf, BUFSIZ ) ) break;
    if( zTokenIsTag( buf ) ){
      zExtractTag( buf, buf );
      if( !( ztk->def = ZTKDefListFindTag( &ztk->deflist, buf ) ) ){
        /* unregisterred tagged field, skipped */
        ztk->tf_cp = NULL;
      } else{
        if( !( ztk->tf_cp = ZTKTagFieldListNew( buf ) ) ){ /* allocate a new tagged field */
          ret = false;
          break;
        }
        zListInsertHead( &ztk->tflist, ztk->tf_cp );
        ztk->kf_cp = NULL; /* unactivate the key field */
      }
    } else{ /* might be a key or a value */
      if( strcmp( buf, "include" ) == 0 ){ /* include a file */
        if( !zFSkipDefaultComment( fs->fp ) ) break;
        ZTKParse( ztk, zFToken(fs->fp,buf,BUFSIZ) );
        continue;
      }
      if( !ztk->def ) continue; /* tagged field unactivated */
      if( !ztk->tf_cp ){ /* the corresponding tagged field must be activated. */
        ZRUNERROR( ZEDA_ERR_FATAL );
        ret = false;
        break;
      }
      if( ZTKDefFindKey( &ztk->def->data, buf ) ){ /* token is a key. */
        if( !( ztk->kf_cp = ZTKKeyFieldListNew( &ztk->tf_cp->data.kflist, buf ) ) ){
          ret = false;
          break;
        }
      } else{
        if( !ztk->kf_cp ) continue; /* skipped if the key field is unactivated */
        if( !ZTKKeyFieldAddVal( &ztk->kf_cp->data, buf ) ){ /* token is a value. */
          ret = false;
          break;
        }
      }
    }
  }
  zFileStackPop( &ztk->fs );
  return ret;
}

/* count the number of tagged fields with specified tag */
int ZTKCountTag(ZTK *ztk, const char *tag)
{
  ZTKTagFieldListCell *cp;
  int count = 0;

  zListForEach( &ztk->tflist, cp )
    if( strcmp( cp->data.tag, tag ) == 0 ) count++;
  return count;
}

/* count the number of key fields with specified key in the current tagged field */
int ZTKCountKey(ZTK *ztk, const char *key)
{
  ZTKKeyFieldListCell *cp;
  int count = 0;

  if( !ztk->tf_cp ) return 0;
  zListForEach( &ztk->tf_cp->data.kflist, cp )
    if( strcmp( cp->data.key, key ) == 0 ) count++;
  return count;
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
    ZTKKeyRewind( ztk );
    do{
      fprintf( fp, "%s:", ZTKKey(ztk) );
      ZTKValRewind( ztk );
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
  } while( ZTKTagNext(ztk) );
}
