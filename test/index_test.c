#include <zeda/zeda.h>

void assert_clone(void)
{
  zIndex src, dest;
  int i, n = 10;

  src = zIndexCreate( n );
  for( i=0; i<n; i++ ){
    zIndexSetElem( src, i, zRandI(-n,n) );
  }
  zAssert( zIndexClone (null case), zIndexClone( NULL ) == NULL );
  dest = zIndexClone( src );
  zAssert( zIndexClone, zIndexEqual( src, dest ) );
  zIndexFree( src );
  zIndexFree( dest );
}

void assert_index_head_tail(void)
{
  zIndex index;

  index = zIndexCreate( 10 );
  zAssert( zIndexHead, zIndexHead(index) == 9 );
  zAssert( zIndexNeck, zIndexNeck(index) == 8 );
  zAssert( zIndexTail, zIndexTail(index) == 0 );
  zIndexFree( index );
}

void assert_index_swap(void)
{
  zIndex index, index_cmp;
  bool res;

  index = zIndexCreate( 10 );
  index_cmp = zIndexCreateList( 10, 0, 1, 5, 3, 4, 2, 6, 7, 8, 9 );
  zIndexSwap( index, 2, 5 );
  res = zIndexEqual( index, index_cmp );
  zIndexFree( index_cmp );
  zIndexFree( index );
  zAssert( zIndexSwap, res );
}

void assert_index_move(void)
{
  zIndex index, index_cmp;
  bool res1, res2;

  index = zIndexCreate( 10 );
  index_cmp = zIndexCreate( 10 );
  zIndexMove( index, 2, 7 );
  zIndexSetList( index_cmp, 0, 1, 3, 4, 5, 6, 7, 2, 8, 9 );
  res1 = zIndexEqual( index, index_cmp );
  zIndexMove( index, 6, 3 );
  zIndexSetList( index_cmp, 0, 1, 3, 7, 4, 5, 6, 2, 8, 9 );
  res2 = zIndexEqual( index, index_cmp );
  zIndexFree( index_cmp );
  zIndexFree( index );
  zAssert( zIndexMove, res1 && res2 );
}

#define N 100
#define SIZE 10

void assert_index_remove(void)
{
  zIndex index, index_cmp;
  int i, j, k;
  bool res = true;

  index = zIndexCreate( SIZE );
  index_cmp = zIndexCreate( SIZE );
  for( k=0; k<N; k++ ){
    zArraySize(index) = zArraySize(index_cmp) = SIZE;
    zIndexOrder( index, 0 );
    zIndexOrder( index_cmp, 0 );
    i = zRandI( 0, zArraySize(index)-1 );
    zIndexRemove( index, i );
    for( j=i+1; j<zArraySize(index_cmp); j++ )
      zIndexSetElem( index_cmp, j-1, zIndexElemNC(index_cmp,j) );
    zIndexDecSize( index_cmp );
    if( !zIndexEqual( index, index_cmp ) ) res = false;
  }
  zIndexFree( index_cmp );
  zIndexFree( index );
  zAssert( zIndexRemove, res );
}

void assert_index_remove_irregular(void)
{
  zIndex index, index_cmp;
  bool result = true;

  index = zIndexCreate( SIZE );
  index_cmp = zIndexCreate( SIZE-1 );
  zIndexOrder( index, 0 );
  zIndexRemove( index, 0 );
  zIndexOrder( index_cmp, 1 );
  if( !zIndexEqual( index, index_cmp ) ){
    eprintf( "(case 1) different indices\n" );
    zIndexFPrint( stderr, index );
    zIndexFPrint( stderr, index_cmp );
  }
  zIndexRemove( index, zIndexSizeNC(index) );
  if( !zIndexEqual( index, index_cmp ) ){
    eprintf( "(case 2) different indices\n" );
    zIndexFPrint( stderr, index );
    zIndexFPrint( stderr, index_cmp );
  }
  zIndexRemove( index, zIndexSizeNC(index)-1 );
  zIndexDecSize( index_cmp );
  if( !zIndexEqual( index, index_cmp ) ){
    eprintf( "(case 3) different indices\n" );
    zIndexFPrint( stderr, index );
    zIndexFPrint( stderr, index_cmp );
  }
  zIndexFree( index );
  zIndexFree( index_cmp );
  zAssert( zIndexOrder (irregular cases), result );
}

void assert_index_remove_and_insert_test(void)
{
  zIndex i1, i2;
  int i, val;
  bool result = true;

  i1 = zIndexAlloc( N );
  for( i=0; i<zIndexSizeNC(i1); i++ )
    zIndexSetElemNC( i1, i, zRandI(-N,N) );
  i2 = zIndexAlloc( N );
  zIndexSizeNC(i2) = 0;
  while( zIndexSizeNC(i1) > 0 ){
    i = zRandI( 0, zIndexSizeNC(i1)-1 );
    val = zIndexElemNC(i1,i);
    zIndexRemoveVal( i1, val );
    zIndexInsertVal( i2, N, val );
  }
  for( i=1; i<zIndexSizeNC(i2); i++ )
    if( zIndexElemNC(i2,i) < zIndexElemNC(i2,i-1) ) result = false;
  zIndexFree( i1 );
  zIndexFree( i2 );
  zAssert( zIndexRemoveVal + zIndexInsertVal, result );
}

void assert_index_sort(void)
{
  zIndex index;
  int i;
  bool result = true;

  index = zIndexAlloc( N );
  for( i=0; i<N; i++ )
    zIndexElemNC(index,i) = zRandI(-N,N);
  zIndexSort( index );
  for( i=1; i<N; i++ )
    if( zIndexElemNC(index,i-1) > zIndexElemNC(index,i) ) result = false;
  zIndexFree( index );
  zAssert( zIndexSort, result );
}

void assert_index_shuffle(void)
{
  zIndex index;
  int i, n = 10, count;
  bool result = true;

  index = zIndexCreate( n );
  zIndexShuffle( index, 0 );
  for( count=0, i=0; i<zIndexSizeNC(index); i++ )
    if( zIndexElemNC(index,i) == i ) count++;
  if( count > zIndexSizeNC(index) / 3 ) result = false;
  zIndexSort( index );
  for( count=0, i=0; i<zIndexSizeNC(index); i++ )
    if( zIndexElemNC(index,i) != i ) result = false;
  zIndexFree( index );
  zAssert( zIndexShuffle, result );
}

int main(void)
{
  zRandInit();
  assert_clone();
  assert_index_head_tail();
  assert_index_swap();
  assert_index_move();
  assert_index_remove();
  assert_index_remove_irregular();
  assert_index_remove_and_insert_test();
  assert_index_sort();
  assert_index_shuffle();
  return EXIT_SUCCESS;
}
