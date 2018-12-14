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

  printf( "+++ append A +++\n" );
  list_create( &l1, 0, 4 );
  list_create( &l2, 5, 9 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListAppendA( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListDestroy( int_list_cell_t, &l1 );
  zListDestroy( int_list_cell_t, &l2 );

  printf( "+++ append Z +++\n" );
  list_create( &l1, 0, 4 );
  list_create( &l2, 5, 9 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListAppendZ( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListDestroy( int_list_cell_t, &l1 );
  zListDestroy( int_list_cell_t, &l2 );

  printf( "+++ append void lists +++\n" );
  zListAppendA( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  return 0;
}
