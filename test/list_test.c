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

typedef struct{
  Z_NAMED_CLASS;
  int val;
} namedint_t;
zListClass( namedint_list_t, namedint_cell_t, namedint_t );

void assert_list_name_find(void)
{
  namedint_list_t list;
  namedint_cell_t *cp;
  int i, val;
  char buf[4];
  bool result1 = true;
  bool result2 = true;

  zListInit( &list );
  for( i=0; i<SIZE; i++ ){
    cp = zAlloc( namedint_cell_t, 1 );
    zI2A( i, buf );
    zNameSet( &cp->data, buf );
    cp->data.val = i;
    zListInsertHead( &list, cp );
  }
  for( i=0; i<SIZE; i++ ){
    zI2A( ( val = zRandI( 0, SIZE-1 ) ), buf );
    zListFindName( &list, buf, cp );
    if( atoi(buf) != val ) result1 = false;
  }
  zI2A( SIZE, buf );
  zListFindName( &list, buf, cp );
  result2 = cp ? false : true;
  while( !zListIsEmpty( &list ) ){
    zListDeleteHead( &list, &cp );
    zNameFree( &cp->data );
    free( cp );
  }
  zAssert( zListNameFind (successful case), result1 );
  zAssert( zListNameFind (failure case), result2 );
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
  zRandInit();
  assert_list_item();
  assert_list_name_find();
  assert_swap();
  assert_purge();
  assert_quicksort();
  return EXIT_SUCCESS;
}
