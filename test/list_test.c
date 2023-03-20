#include <zeda/zeda.h>

void gen_intlist(zIntList *list, int n)
{
  int i;

  zListInit( list );
  for( i=0; i<n; i++ )
    zIntListAdd( list, i );
}

void gen_intlist_rand(zIntList *list, int n, int imin, int imax)
{
  int i;

  zListInit( list );
  for( i=0; i<n; i++ )
    zIntListAdd( list, zRandI(imin,imax) );
}

#define SIZE 10

void assert_list_item(void)
{
  zIntList list;
  zIntListCell *cp;
  int i;
  bool result = true;

  gen_intlist( &list, SIZE );
  for( i=0; i<zListSize(&list); i++ ){
    zListItem( &list, i, &cp );
    if( cp->data != i ) result = false;
  }
  zListDestroy( zIntListCell, &list );
  zAssert( zListItem, result );
}

bool test_purge_check(zIntList *list, int i)
{
  zIntListCell *cp;
  bool result = true;
  int k = 0;

  zListForEach( list, cp ){
    if( k < i ){
      if( cp->data != k ) result = false;
    } else{
      if( cp->data != k + 1 ) result = false;
    }
    k++;
  }
  return result;
}

void assert_purge(void)
{
  zIntList list;
  zIntListCell *cp;
  int i;
  bool result = true;

  for( i=0; i<SIZE; i++ ){
    gen_intlist( &list, SIZE );
    zListItem( &list, i, &cp );
    zListPurge( &list, cp );
    if( !test_purge_check( &list, i ) ) result = false;
    zFree( cp );
    zListDestroy( zIntListCell, &list );
  }
  zAssert( zListPurge, result );
}

bool test_swap_check(zIntList *list, int i, int j)
{
  int k = 0;
  zIntListCell *cp;
  bool result = true;

  zListForEach( list, cp ){
    if( k == i ){
      if( cp->data != j ) result = false;
    } else
    if( k == j ){
      if( cp->data != i ) result = false;
    } else{
      if( cp->data != k ) result = false;
    }
    k++;
  }
  return result;
}

void assert_swap(void)
{
  zIntList list;
  zIntListCell *c1, *c2;
  int i, j;
  bool result = true;

  gen_intlist( &list, SIZE );
  for( i=0; i<SIZE; i++ )
    for( j=0; j<SIZE; j++ ){
      zListItem( &list, i, &c1 );
      zListItem( &list, j, &c2 );
      zListCellSwap( zIntListCell, c1, c2 );
      if( !test_swap_check( &list, i, j ) ) result = false;
      zListCellSwap( zIntListCell, c1, c2 );
    }
  zAssert( zListCellSwap, result );
}

zListQuickSortDef(zIntList, zIntListCell)

int test_quicksort_cmp(void *v1, void *v2, void *dummy)
{
  if( ((zIntListCell*)v1)->data == ((zIntListCell*)v2)->data ) return 0;
  return ( ((zIntListCell*)v1)->data > ((zIntListCell*)v2)->data ) ? 1 : -1;
}

void assert_quicksort(void)
{
  int i, prev_val = 1 - INT_MAX;
  zIntList list;
  zIntListCell *cp;
  bool result = true;

  gen_intlist_rand( &list, SIZE, -100, 100 );
  zIntListQuickSort( &list, test_quicksort_cmp, NULL );
  for( i=0; !zListIsEmpty(&list); i++ ){
    zListDeleteTail( &list, &cp );
    if( cp->data < prev_val ) result = false;
    prev_val = cp->data;
    zFree( cp );
  }
  zAssert( zListQuickSortDef, result );
}

int main(void)
{
  assert_list_item();
  assert_swap();
  assert_purge();
  assert_quicksort();
  return EXIT_SUCCESS;
}
