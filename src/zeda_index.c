/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_index - integer value array.
 */

#include <zeda/zeda_index.h>
#include <zeda/zeda_string.h>

/* ********************************************************** */
/* CLASS: zIndex
 * integer value array class
 * ********************************************************** */

/* allocate internal array of integer vector. */
zIndex zIndexAlloc(int size)
{
  zIndex idx;

  if( !( idx = zAlloc( zIndexStruct, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zArrayAlloc( idx, int, size );
  if( !zArrayBuf(idx) ){
    free( idx );
    return NULL;
  }
  return idx;
}

/* create an integer vector. */
zIndex zIndexCreate(int size)
{
  int i;
  zIndex idx;

  if( !( idx = zIndexAlloc( size ) ) ) return NULL;
  for( i=0; i<size; i++ )
    zIndexSetElemNC( idx, i, i ); /* ordered index as default */
  return idx;
}

#ifndef __KERNEL__
/* create an integer vector from argument list. */
zIndex zIndexCreateList(int size, ...)
{
  int i;
  zIndex idx;
  va_list args;

  if( !( idx = zIndexCreate( size ) ) ) return NULL;
  va_start( args, size );
  for( i=0; i<size; i++ )
    zIndexSetElemNC( idx, i, va_arg( args, int ) );
  va_end( args );
  return idx;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* set components of an integer vector from argument list. */
zIndex zIndexSetList(zIndex idx, ...)
{
  int i;
  va_list args;

  va_start( args, idx );
  for( i=0; i<zIndexSizeNC(idx); i++ )
    zIndexSetElemNC( idx, i, va_arg( args, int ) );
  va_end( args );
  return idx;
}
#endif /* __KERNEL__ */

/* copy an integer vector to another. */
zIndex zIndexCopy(zIndex src, zIndex dest)
{
  if( zIndexSize(src) != zIndexSize(dest) ){
    ZRUNERROR( ZEDA_ERR_SIZMIS_INDEX );
    return NULL;
  }
  return zIndexCopyNC( src, dest );
}

/* free an integer vector. */
void zIndexFree(zIndex idx)
{
  if( idx ){
    zArrayFree( idx );
    free( idx );
  }
}

/* zero an integer vector. */
zIndex zIndexZero(zIndex idx)
{
  memset( zArrayBuf(idx), 0, sizeof(int)*zIndexSizeNC(idx) );
  return idx;
}

/* ordinate an integer vector. */
zIndex zIndexOrder(zIndex idx, int s)
{
  int i;

  for( i=0; i<zIndexSizeNC(idx); i++ )
    zIndexSetElemNC( idx, i, s+i );
  return idx;
}

/* check if two integer vectors are equal. */
bool zIndexIsEqual(zIndex idx1, zIndex idx2)
{
  int i;

  if( zIndexSizeNC(idx1) != zIndexSizeNC(idx2) ) return false;
  for( i=0; i<zIndexSizeNC(idx1); i++ )
    if( zIndexElemNC(idx1,i) != zIndexElemNC(idx2,i) ) return false;
  return true;
}

/* swap two components of an integer vector. */
int zIndexSwap(zIndex idx, int p1, int p2)
{
  zSwap( int, zIndexElemNC(idx,p1), zIndexElemNC(idx,p2) );
  return zIndexElemNC( idx, p2 );
}

/* move a component of an integer vector to another position. */
int zIndexMove(zIndex idx, int from, int to)
{
  int i;
  int tmp;

  tmp = zIndexElemNC( idx, from );
  if( from == to ) return tmp;
  if( from < to )
    for( i=from; i<to; i++ )
      zIndexSetElemNC( idx, i, zIndexElemNC(idx,i+1) );
  else
    for( i=from; i>to; i-- )
      zIndexSetElemNC( idx, i, zIndexElemNC(idx,i-1) );
  return zIndexSetElemNC( idx, to, tmp );
}

/* remove an integer value from an integer vector. */
bool zIndexRemoveVal(zIndex index, int val)
{
  int i;
  bool ret = false;

  for( i=0; i<zIndexSizeNC(index); i++ )
    if( zIndexElemNC(index,i) == val ){
      zIndexRemove( index, i );
      ret = true;
    }
  return ret;
}

/* insert an integer value to an integer vector. */
void zIndexInsertVal(zIndex index, int val)
{
  int i;

  for( i=0; i<zIndexSizeNC(index); i++ )
    if( zIndexElemNC(index,i) >= val ){
      memmove( &zIndexElemNC(index,i+1), &zIndexElemNC(index,i), sizeof(int)*(zIndexSizeNC(index)-i) );
      break;
    }
  zIndexIncSize( index );
  zIndexSetElem( index, i, val );
}

/* remove a component from an integer vector. */
zIndex zIndexRemove(zIndex idx, int i)
{
  memmove( &zIndexElemNC(idx,i), &zIndexElemNC(idx,i+1), sizeof(int)*(zIndexSizeNC(idx)-i-1) );
  zIndexSizeNC(idx)--;
  return idx;
}

/* comparison function for zIndexSort. */
static int _zIndexSortCmp(void *p1, void *p2, void *dummy)
{
  if( *(int*)p1 > *(int*)p2 ) return 1;
  if( *(int*)p1 < *(int*)p2 ) return -1;
  return 0;
}
/* sort an integer vector in ascending order. */
void zIndexSort(zIndex index)
{
  zQuickSort( zIndexBufNC(index), zIndexSizeNC(index), sizeof(int), _zIndexSortCmp, NULL );
}

/* create an integer vector from a list of integers. */
zIndex zIndexCreateFromList(zIntList *list)
{
  zIndex index;
  zIntListCell *cp;
  int i;

  if( zListIsEmpty(list) ){
    ZRUNERROR( ZEDA_ERR_LIST2INDEX_FAILED );
    return NULL;
  }
  if( !( index = zIndexCreate( zListSize(list) ) ) ) return NULL;
  i=0;
  zListForEach( list, cp )
    zIndexElemNC(index,i++) = cp->data;
  return index;
}

#ifndef __KERNEL__
/* scan an integer vector from a file. */
zIndex zIndexFScan(FILE *fp)
{
  int i;
  int size;
  zIndex idx;

  if( !zFInt( fp, &size ) ){
    ZRUNERROR( ZEDA_ERR_SIZUNFOUND_INDEX );
    return NULL;
  }
  if( !( idx = zIndexCreate( size ) ) ) return NULL;
  for( i=0; i<size; i++ ){
    if( zFInt( fp, zArrayElemNC(idx,i) ) ){
      ZRUNERROR( ZEDA_WARN_INDEX_SIZMIS, i, size );
      break;
    }
  }
  return idx;
}

/* print out components of an integer vector to a file. */
void zIndexDataFPrint(FILE *fp, zIndex idx)
{
  int i;

  if( !idx ) return;
  for( i=0; i<zIndexSizeNC(idx); i++ )
    fprintf( fp, "%d ", zIndexElemNC(idx,i) );
  fprintf( fp, "\n" );
}

/* print out an integer vector to a file. */
void zIndexFPrint(FILE *fp, zIndex idx)
{
  int i;

  if( !idx )
    fprintf( fp, "(null integer vector)\n" );
  else{
    fprintf( fp, "%d (", zIndexSizeNC(idx) );
    for( i=0; i<zIndexSizeNC(idx); i++ )
      fprintf( fp, " %d", zIndexElemNC(idx,i) );
    fprintf( fp, " )\n" );
  }
}
#else
/* print out an integer vector (for kernel space programming). */
void zIndexPrint(zIndex idx)
{
  int i;

  if( !idx )
    printk( "(null integer vector)\n" );
  else{
    printk( "%d (", zIndexSizeNC(idx) );
    for( i=0; i<zIndexSizeNC(idx); i++ )
      printk( " %d", zIndexElemNC(idx,i) );
    printk( " )\n" );
  }
}
#endif /* __KERNEL__ */
