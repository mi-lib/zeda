#include <zeda/zeda_list.h>

zListClass(int_list_t, int_list_cell_t, int);

void list_create(int_list_t *list, int from, int to)
{
  register int i;
  int_list_cell_t *cp;

  zListInit( list );
  for( i=from; i<=to; i++ ){
    if( !( cp = zAlloc( int_list_cell_t, 1 ) ) ){
      ZALLOCERROR();
      exit( 1 );
    }
    cp->data = i;
    zListInsertTail( list, cp );
  }
}

void list_write(int_list_t *list, char *name)
{
  int_list_cell_t *cp;

  printf( "<%s>\n", name );
  zListForEach( list, cp )
    printf( " value = %d\n", cp->data );
}

int main(void)
{
  int_list_t l1, l2;
  int_list_cell_t *c1, *c2;

  list_create( &l1, 0, 5 );
  list_create( &l2, 6, 10 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );

  printf( "List items swapped (1:1<->1:4), (2:1<->2:4).\n" );
  zListItem( &l1, 1, &c1 );
  zListItem( &l1, 4, &c2 );
  zListCellSwap( int_list_cell_t, c1, c2 );
  zListItem( &l2, 1, &c1 );
  zListItem( &l2, 4, &c2 );
  zListCellSwap( int_list_cell_t, c1, c2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );

  printf( "Lists swapped.\n" );
  zListSwap( int_list_cell_t, &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );

  zListDestroy( int_list_cell_t, &l1 );
  zListDestroy( int_list_cell_t, &l2 );
  return 0;
}
