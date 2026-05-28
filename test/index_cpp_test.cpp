#include <zeda/zeda.h>

void assert_index_copy_clone(void)
{
  const int size = 10;
  zIndex src  = new zIndexStruct( size );
  zIndex dest = new zIndexStruct( size );
  for(int i=0; i<src->size; i++ ) (*src)[i] = zRandI( 0, size );
  *dest = *src;
  bool res1 = *src == *dest;
  delete dest;
  dest = src->clone();
  bool res2 = *src == *dest;
  delete dest;
  delete src;
  zAssert( C++::zIndex.operator=, res1 );
  zAssert( C++::zIndex.clone, res2 );
}

void assert_index_swap(void)
{
  zIndex index     = new zIndexStruct( 10 );
  zIndex index_cmp = new zIndexStruct( 10 );

  index_cmp->setList( 0, 1, 5, 3, 4, 2, 6, 7, 8, 9 );
  index->swap( 2, 5 );
  bool res = *index == *index_cmp;
  delete index_cmp;
  delete index;
  zAssert( C++::zIndex.swap, res );
}

void assert_index_move(void)
{
  zIndex index     = new zIndexStruct( 10 );
  zIndex index_cmp = new zIndexStruct( 10 );

  index->move( 2, 7 );
  index_cmp->setList( 0, 1, 3, 4, 5, 6, 7, 2, 8, 9 );
  bool res1 = *index == *index_cmp;
  index->move( 6, 3 );
  index_cmp->setList( 0, 1, 3, 7, 4, 5, 6, 2, 8, 9 );
  bool res2 = *index == *index_cmp;
  delete index_cmp;
  delete index;
  zAssert( C++::zIndex.move, res1 && res2 );
}

void assert_index_remove(void)
{
  const int size = 10;
  zIndex index     = new zIndexStruct( size );
  zIndex index_cmp = new zIndexStruct( size );
  const int n = 100;
  bool res = true;
  for(int k=0; k<n; k++ ){
    index->size = size;
    index_cmp->size = size;
    index->order();
    index_cmp->order();
    int i = zRandI( 0, index->size-1 );
    index->remove( i );
    for(int j=i+1; j<index_cmp->size; j++ )
      (*index_cmp)[j-1] = (*index_cmp)[j];
    index_cmp->size--;
    if( *index != *index_cmp ) res = false;
  }
  delete index_cmp;
  delete index;
  zAssert( C++::zIndex.remove, res );
}

void assert_index_remove_and_insert_test(void)
{
  const int n = 100;
  zIndex i1 = new zIndexStruct( n );
  zIndex i2 = new zIndexStruct( n );
  bool result = true;

  for(int i=0; i<i1->size; i++ )
    (*i1)[i] = zRandI(-n,n);
  i2->size = 0;
  while( i1->size > 0 ){
    int i = zRandI( 0, i1->size-1 );
    int val = (*i1)[i];
    i1->removeVal( val );
    i2->removeVal( val );
  }
  for(int i=1; i<i2->size; i++ )
    if( (*i2)[i] < (*i2)[i-1] ) result = false;
  delete i1;
  delete i2;
  zAssert( C++::zIndex.removeVal + insertVal, result );
}

void assert_index_sort(void)
{
  const int n = 100;
  zIndex index = new zIndexStruct( n );
  bool result = true;

  for(int i=0; i<n; i++ )
    (*index)[i] = zRandI(-n,n);
  index->sort();
  for(int i=1; i<n; i++ )
    if( (*index)[i-1] > (*index)[i] ) result = false;
  delete index;
  zAssert( C++::zIndex.sort, result );
}

void assert_index_shuffle(void)
{
  const int size = 10;
  zIndex index = new zIndexStruct( size );
  int i, count;
  bool result = true;

  index->shuffle( 0 );
  for( count=0, i=0; i<index->size; i++ )
    if( (*index)[i] == i ) count++;
  if( count > index->size / 3 ) result = false;
  index->sort();
  for( count=0, i=0; i<index->size; i++ )
    if( (*index)[i] != i ) result = false;
  delete index;
  zAssert( C++::zIndex.shuffle, result );
}

int main(void)
{
  zRandInit();
  assert_index_copy_clone();
  assert_index_swap();
  assert_index_move();
  assert_index_remove();
  assert_index_remove_and_insert_test();
  assert_index_sort();
  assert_index_shuffle();
  return EXIT_SUCCESS;
}
