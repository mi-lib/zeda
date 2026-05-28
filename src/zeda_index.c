/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_index - integer value array.
 */

#include <zeda/zeda_index.h>
#include <zeda/zeda_string.h>
#include <zeda/zeda_rand.h>

/* allocate internal array of integer vector. */
zIndex zIndexAlloc(int size)
{
  zIndex index;

  if( !( index = zAlloc( zIndexStruct, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zArrayAlloc( index, int, size );
  if( !zArrayBuf(index) ){
    free( index );
    return NULL;
  }
  return index;
}

/* create an integer vector. */
zIndex zIndexCreate(int size)
{
  int i;
  zIndex index;

  if( !( index = zIndexAlloc( size ) ) ) return NULL;
  for( i=0; i<size; i++ )
    zIndexSetElemNC( index, i, i ); /* ordered index as default */
  return index;
}

#ifndef __KERNEL__
/* create an integer vector from argument list. */
zIndex zIndexCreateList(int size, ...)
{
  int i;
  zIndex index;
  va_list args;

  if( !( index = zIndexCreate( size ) ) ) return NULL;
  va_start( args, size );
  for( i=0; i<size; i++ )
    zIndexSetElemNC( index, i, va_arg( args, int ) );
  va_end( args );
  return index;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* set vector components from variable argument list. */
zIndex zIndexSetElemVList(zIndex index, va_list args)
{
  int i;

  for( i=0; i<zIndexSizeNC(index); i++ )
    zIndexSetElemNC( index, i, (int)va_arg( args, int ) );
  return index;
}
/* set components of an integer vector from argument list. */
zIndex zIndexSetElemList(zIndex index, ...)
{
  va_list args;

  va_start( args, index );
  zIndexSetElemVList( index, args );
  va_end( args );
  return index;
}
#endif /* __KERNEL__ */

/* copy an integer vector to another without checking the size consistency. */
zIndex zIndexCopyNC(const zIndex src, zIndex dest)
{
  _zIndexCopyNC( src, dest );
  return dest;
}

/* copy an integer vector to another. */
zIndex zIndexCopy(const zIndex src, zIndex dest)
{
  if( zIndexSize(src) != zIndexSize(dest) ){
    ZRUNERROR( ZEDA_ERR_INDEX_SIZEMISMATCH );
    return NULL;
  }
  _zIndexCopyNC( src, dest );
  return dest;
}

/* clone an integer vector. */
zIndex zIndexClone(const zIndex src)
{
  zIndex dest;

  if( !src ) return NULL;
  if( !( dest = zIndexAlloc( zIndexCapacity(src) ) ) ) return NULL;
  return zIndexCopyNC( src, dest );
}

/* free an integer vector. */
void zIndexFree(zIndex index)
{
  if( index ){
    zArrayFree( index );
    free( index );
  }
}

/* zero an integer vector. */
zIndex zIndexZero(zIndex index)
{
  memset( zArrayBuf(index), 0, sizeof(int)*zIndexSizeNC(index) );
  return index;
}

/* ordinate an integer vector. */
zIndex zIndexOrder(zIndex index, int s)
{
  int i;

  for( i=0; i<zIndexSizeNC(index); i++ )
    zIndexSetElemNC( index, i, s+i );
  return index;
}

/* check if two integer vectors are equal. */
bool zIndexEqual(const zIndex index1, const zIndex index2)
{
  int i;

  if( zIndexSizeNC(index1) != zIndexSizeNC(index2) ) return false;
  for( i=0; i<zIndexSizeNC(index1); i++ )
    if( zIndexElemNC(index1,i) != zIndexElemNC(index2,i) ) return false;
  return true;
}

/* swap two components of an integer vector without checking validity of specified positions. */
void zIndexSwapNC(zIndex index, int p1, int p2)
{
  zSwap( int, zIndexElemNC(index,p1), zIndexElemNC(index,p2) );
}

/* swap two components of an integer vector. */
bool zIndexSwap(zIndex index, int p1, int p2)
{
  if( !zIndexPosIsValid( index, p1 ) ){
    ZRUNERROR( ZEDA_ERR_INDEX_INVALID_POS, p1, zIndexSizeNC(index)-1 );
    return false;
  }
  if( !zIndexPosIsValid( index, p2 ) ){
    ZRUNERROR( ZEDA_ERR_INDEX_INVALID_POS, p2, zIndexSizeNC(index)-1 );
    return false;
  }
  zIndexSwapNC( index, p1, p2 );
  return true;
}

/* move a component of an integer vector to another position. */
int zIndexMove(zIndex index, int from, int to)
{
  int i;
  int tmp;

  tmp = zIndexElemNC( index, from );
  if( from == to ) return tmp;
  if( from < to )
    for( i=from; i<to; i++ )
      zIndexSetElemNC( index, i, zIndexElemNC(index,i+1) );
  else
    for( i=from; i>to; i-- )
      zIndexSetElemNC( index, i, zIndexElemNC(index,i-1) );
  return zIndexSetElemNC( index, to, tmp );
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
int zIndexInsertVal(zIndex index, int val)
{
  int i;

  if( zIndexSizeNC(index) >= zIndexCapacity(index) ){
    ZRUNERROR( ZEDA_ERR_INDEX_BUF_OCCUPIED );
    return -1;
  }
  for( i=0; i<zIndexSizeNC(index); i++ )
    if( zIndexElemNC(index,i) >= val ){
      memmove( &zIndexElemNC(index,i+1), &zIndexElemNC(index,i), sizeof(int)*(zIndexSizeNC(index)-i) );
      break;
    }
  zIndexIncSize( index );
  zIndexSetElem( index, i, val );
  return i;
}

/* remove a component from an integer vector. */
zIndex zIndexRemove(zIndex index, int i)
{
  int size;

  if( !zIndexPosIsValid( index, i ) ){
    ZRUNERROR( ZEDA_ERR_INDEX_INVALID_POS, i, zIndexSizeNC(index)-1 );
    return NULL;
  }
  if( ( size = zIndexSizeNC(index) - i - 1 ) > 0 )
    memmove( &zIndexElemNC(index,i), &zIndexElemNC(index,i+1), sizeof(int)*size );
  zIndexSizeNC(index)--;
  return index;
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

/* randomly shuffle components of an integer vector. */
zIndex zIndexShuffle(zIndex index, int count)
{
  int i, i1, i2;

  if( count == 0 )
    count = zIndexSizeNC(index) * 3;
  for( i=0; i<count; i++ ){
    i1 = zRandI( 0, zIndexSizeNC(index)-1 );
    do{
      i2 = zRandI( 0, zIndexSizeNC(index)-1 );
    } while( i2 == i1 );
    zIndexSwap( index, i1, i2 );
  }
  return index;
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
  zIndex index;

  if( !zFInt( fp, &size ) ){
    ZRUNERROR( ZEDA_ERR_INDEX_SIZENOTSPECIFIED );
    return NULL;
  }
  if( !( index = zIndexCreate( size ) ) ) return NULL;
  for( i=0; i<size; i++ ){
    if( zFInt( fp, zArrayElemNC(index,i) ) ){
      ZRUNERROR( ZEDA_ERR_INDEX_OVERSIZE, i, size );
      return NULL;
    }
  }
  return index;
}

/* print out components of an integer vector to a file. */
void zIndexDataFPrint(FILE *fp, const zIndex index)
{
  int i;

  if( !index ) return;
  for( i=0; i<zIndexSizeNC(index); i++ )
    fprintf( fp, "%d ", zIndexElemNC(index,i) );
  fprintf( fp, "\n" );
}

/* print out an integer vector to a file. */
void zIndexFPrint(FILE *fp, const zIndex index)
{
  int i;

  if( !index )
    fprintf( fp, "(null integer vector)\n" );
  else{
    fprintf( fp, "%d (", zIndexSizeNC(index) );
    for( i=0; i<zIndexSizeNC(index); i++ )
      fprintf( fp, " %d", zIndexElemNC(index,i) );
    fprintf( fp, " )\n" );
  }
}
#else
/* print out an integer vector (for kernel space programming). */
void zIndexPrint(const zIndex index)
{
  int i;

  if( !index )
    printk( "(null integer vector)\n" );
  else{
    printk( "%d (", zIndexSizeNC(index) );
    for( i=0; i<zIndexSizeNC(index); i++ )
      printk( " %d", zIndexElemNC(index,i) );
    printk( " )\n" );
  }
}
#endif /* __KERNEL__ */
