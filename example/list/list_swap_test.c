#include <zeda/zeda_list.h>

void list_create(zIntList *list, int from, int to)
{
  register int i;
  zIntListCell *cp;

  zListInit( list );
  for( i=from; i<=to; i++ ){
    if( !( cp = zAlloc( zIntListCell, 1 ) ) ){
      ZALLOCERROR();
      exit( 1 );
    }
    cp->data = i;
    zListInsertTail( list, cp );
  }
}

void list_print(zIntList *list, char *name)
{
  zIntListCell *cp;

  printf( "<%s>\n", name );
  zListForEach( list, cp )
    printf( " value = %d\n", cp->data );
}

int main(void)
{
  zIntList l1, l2;
  zIntListCell *c1, *c2;

  list_create( &l1, 0, 5 );
  list_create( &l2, 6, 10 );
  list_print( &l1, "list1" );
  list_print( &l2, "list2" );

  printf( "List items swapped (1:1<->1:4), (2:1<->2:4).\n" );
  zListItem( &l1, 1, &c1 );
  zListItem( &l1, 4, &c2 );
  zListCellSwap( zIntListCell, c1, c2 );
  zListItem( &l2, 1, &c1 );
  zListItem( &l2, 4, &c2 );
  zListCellSwap( zIntListCell, c1, c2 );
  list_print( &l1, "list1" );
  list_print( &l2, "list2" );

  printf( "Lists swapped.\n" );
  zListSwap( zIntListCell, &l1, &l2 );
  list_print( &l1, "list1" );
  list_print( &l2, "list2" );

  zListDestroy( zIntListCell, &l1 );
  zListDestroy( zIntListCell, &l2 );
  return 0;
}
