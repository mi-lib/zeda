#include <zeda/zeda.h>

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
  res = zIndexIsEqual( index, index_cmp );
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
  res1 = zIndexIsEqual( index, index_cmp );
  zIndexMove( index, 6, 3 );
  zIndexSetList( index_cmp, 0, 1, 3, 7, 4, 5, 6, 2, 8, 9 );
  res2 = zIndexIsEqual( index, index_cmp );
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
    if( !zIndexIsEqual( index, index_cmp ) ) res = false;
  }
  zIndexFree( index_cmp );
  zIndexFree( index );
  zAssert( zIndexRemove, res );
}

int main(void)
{
  zRandInit();
  assert_index_head_tail();
  assert_index_swap();
  assert_index_move();
  assert_index_remove();
  return EXIT_SUCCESS;
}
