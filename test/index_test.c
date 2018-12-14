#include <zeda/zeda.h>

int main(void)
{
  zIndex index, index_cmp;
  bool res1, res2;

  index = zIndexCreate( 10 );
  index_cmp = zIndexCreate( 10 );

  zAssert( zIndexHead, zIndexHead(index) == 9 );
  zAssert( zIndexNeck, zIndexNeck(index) == 8 );
  zAssert( zIndexTail, zIndexTail(index) == 0 );

  zIndexSwap( index, 2, 5 );
  zIndexSetList( index_cmp, 0, 1, 5, 3, 4, 2, 6, 7, 8, 9 );
  zAssert( zIndexSwap, zIndexIsEqual( index, index_cmp ) );

  zIndexMove( index, 2, 7 );
  zIndexSetList( index_cmp, 0, 1, 3, 4, 2, 6, 7, 5, 8, 9 );
  res1 = zIndexIsEqual( index, index_cmp );
  zIndexMove( index, 6, 3 );
  zIndexSetList( index_cmp, 0, 1, 3, 7, 4, 2, 6, 5, 8, 9 );
  res2 = zIndexIsEqual( index, index_cmp );
  zAssert( zIndexMove, res1 && res2 );

  zIndexFree( index );
  zIndexFree( index_cmp );
  return EXIT_SUCCESS;
}
