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

/* zIndexAlloc
 * - allocate internal array of integer vector.
 */
zIndex zIndexAlloc(zIndex idx, int size)
{
  register int i;

  zArrayAlloc( idx, int, size );
  if( !zArrayBuf(idx) ) return NULL;
  for( i=0; i<size; i++ )
    zIndexSetElem( idx, i, i ); /* ordered index as default */
  return idx;
}

/* zIndexCreate
 * - creation of integer vector.
 */
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
/* zIndexCreateList
 * - creation of integer vector from argument list.
 */
zIndex zIndexCreateList(int size, ...)
{
  register int i;
  zIndex idx;
  va_list args;

  if( !( idx = zIndexCreate( size ) ) ) return NULL;
  va_start( args, size );
  for( i=0; i<size; i++ )
    zIndexSetElem( idx, i, va_arg( args, int ) );
  va_end( args );
  return idx;
}
#endif /* __KERNEL__ */

#ifndef __KERNEL__
/* zIndexSetList
 * - set elements of an integer vector from argument list.
 */
zIndex zIndexSetList(zIndex idx, ...)
{
  register int i;
  va_list args;

  va_start( args, idx );
  for( i=0; i<zArrayNum(idx); i++ )
    zIndexSetElem( idx, i, va_arg( args, int ) );
  va_end( args );
  return idx;
}
#endif /* __KERNEL__ */

/* zIndexFree
 * - free integer vector.
 */
void zIndexFree(zIndex idx)
{
  if( idx ){
    zArrayFree( idx );
    free( idx );
  }
}

/* zIndexClear
 * - cleanup of integer vector.
 */
zIndex zIndexClear(zIndex idx)
{
  memset( zArrayBuf(idx), 0, sizeof(int)*zArrayNum(idx) );
  return idx;
}

#ifndef __KERNEL__
/* zIndexOrder
 * - ordination of integer vector.
 */
zIndex zIndexOrder(zIndex idx, int s)
{
  register uint i;

  for( i=0; i<zArrayNum(idx); i++ )
    zIndexSetElem( idx, i, s+i );
  return idx;
}
#endif /* __KERNEL__ */

/* zIndexIsEqual
 * - check if two arrays of integer values are equal.
 */
bool zIndexIsEqual(zIndex idx1, zIndex idx2)
{
  register uint i;

  if( zArrayNum(idx1) != zArrayNum(idx2) ) return false;
  for( i=0; i<zArrayNum(idx1); i++ )
    if( zIndexElem(idx1,i) != zIndexElem(idx2,i) ) return false;
  return true;
}

/* zIndexSwap
 * - swap of two components of integer vector.
 */
int zIndexSwap(zIndex idx, int p1, int p2)
{
  zSwap( int, zIndexElem(idx,p1), zIndexElem(idx,p2) );
  return zIndexElem( idx, p2 );
}

/* zIndexMove
 * - move a component of index to another position.
 */
int zIndexMove(zIndex idx, int from, int to)
{
  register int i;
  int tmp;

  tmp = zIndexElem( idx, from );
  if( from == to ) return tmp;
  if( from < to )
    for( i=from; i<to; i++ )
      zIndexSetElem( idx, i, zIndexElem(idx,i+1) );
  else
    for( i=from; i>to; i-- )
      zIndexSetElem( idx, i, zIndexElem(idx,i-1) );
  return zIndexSetElem( idx, to, tmp );
}

#ifndef __KERNEL__
/* zIndexFRead
 * - input of integer vector.
 */
zIndex zIndexFRead(FILE *fp)
{
  register int i, size;
  zIndex idx;

  size = zFInt( fp );
  if( !( idx = zIndexCreate( size ) ) )
    return NULL;

  for( i=0; i<size; i++ )
    zIndexSetElem( idx, i, zFInt( fp ) );
  return idx;
}

/* zIndexDataFWrite
 * - output of integer vector.
 */
void zIndexDataFWrite(FILE *fp, zIndex idx)
{
  register uint i;

  if( !idx ) return;
  for( i=0; i<zArrayNum(idx); i++ )
    fprintf( fp, "%d ", zIndexElem(idx,i) );
  fprintf( fp, "\n" );
}

/* zIndexFWrite
 * - output of integer vector.
 */
void zIndexFWrite(FILE *fp, zIndex idx)
{
  register uint i;

  if( !idx )
    fprintf( fp, "(null integer vector)\n" );
  else{
    fprintf( fp, "%d (", zArrayNum(idx) );
    for( i=0; i<zArrayNum(idx); i++ )
      fprintf( fp, " %d", zIndexElem(idx,i) );
    fprintf( fp, " )\n" );
  }
}
#else
/* zIndexWrite
 * - output of integer vector (for kernel space programming).
 */
void zIndexWrite(zIndex idx)
{
  register int i;

  if( !idx )
    printk( "(null integer vector)\n" );
  else{
    printk( "%d (", zArrayNum(idx) );
    for( i=0; i<zArrayNum(idx); i++ )
      printk( " %d", zIndexElem(idx,i) );
    printk( " )\n" );
  }
}
#endif /* __KERNEL__ */
