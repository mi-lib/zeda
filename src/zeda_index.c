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
zIndex zIndexAlloc(zIndex idx, int size)
{
  register int i;

  zArrayAlloc( idx, int, size );
  if( !zArrayBuf(idx) ) return NULL;
  for( i=0; i<size; i++ )
    zIndexSetElemNC( idx, i, i ); /* ordered index as default */
  return idx;
}

/* create an integer vector. */
zIndex zIndexCreate(int size)
{
  zIndex idx;

  if( !( idx = zAlloc( zIndexStruct, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !zIndexAlloc( idx, size ) ) zFree( idx );
  return idx;
}

#ifndef __KERNEL__
/* create an integer vector from argument list. */
zIndex zIndexCreateList(int size, ...)
{
  register int i;
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
  register int i;
  va_list args;

  va_start( args, idx );
  for( i=0; i<zArraySize(idx); i++ )
    zIndexSetElemNC( idx, i, va_arg( args, int ) );
  va_end( args );
  return idx;
}
#endif /* __KERNEL__ */

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
  memset( zArrayBuf(idx), 0, sizeof(int)*zArraySize(idx) );
  return idx;
}

/* ordinate an integer vector. */
zIndex zIndexOrder(zIndex idx, int s)
{
  register uint i;

  for( i=0; i<zArraySize(idx); i++ )
    zIndexSetElemNC( idx, i, s+i );
  return idx;
}

/* check if two integer vectors are equal. */
bool zIndexIsEqual(zIndex idx1, zIndex idx2)
{
  register uint i;

  if( zArraySize(idx1) != zArraySize(idx2) ) return false;
  for( i=0; i<zArraySize(idx1); i++ )
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
  register int i;
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

#ifndef __KERNEL__
/* scan an integer vector from a file. */
zIndex zIndexFScan(FILE *fp)
{
  register int i, size;
  zIndex idx;

  size = zFInt( fp );
  if( !( idx = zIndexCreate( size ) ) )
    return NULL;

  for( i=0; i<size; i++ )
    zIndexSetElemNC( idx, i, zFInt( fp ) );
  return idx;
}

/* print out components of an integer vector to a file. */
void zIndexDataFPrint(FILE *fp, zIndex idx)
{
  register uint i;

  if( !idx ) return;
  for( i=0; i<zArraySize(idx); i++ )
    fprintf( fp, "%d ", zIndexElemNC(idx,i) );
  fprintf( fp, "\n" );
}

/* print out an integer vector to a file. */
void zIndexFPrint(FILE *fp, zIndex idx)
{
  register uint i;

  if( !idx )
    fprintf( fp, "(null integer vector)\n" );
  else{
    fprintf( fp, "%d (", zArraySize(idx) );
    for( i=0; i<zArraySize(idx); i++ )
      fprintf( fp, " %d", zIndexElemNC(idx,i) );
    fprintf( fp, " )\n" );
  }
}
#else
/* print out an integer vector (for kernel space programming). */
void zIndexPrint(zIndex idx)
{
  register int i;

  if( !idx )
    printk( "(null integer vector)\n" );
  else{
    printk( "%d (", zArraySize(idx) );
    for( i=0; i<zArraySize(idx); i++ )
      printk( " %d", zIndexElemNC(idx,i) );
    printk( " )\n" );
  }
}
#endif /* __KERNEL__ */
