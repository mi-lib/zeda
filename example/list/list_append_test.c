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

void list_write(zIntList *list, char *name)
{
  zIntListCell *cp;

  printf( "<%s>\n", name );
  zListForEach( list, cp )
    printf( " value = %d\n", cp->data );
}

int main(void)
{
  zIntList l1, l2;

  printf( "+++ append A +++\n" );
  list_create( &l1, 0, 4 );
  list_create( &l2, 5, 9 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListAppendA( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListDestroy( zIntListCell, &l1 );
  zListDestroy( zIntListCell, &l2 );

  printf( "+++ append Z +++\n" );
  list_create( &l1, 0, 4 );
  list_create( &l2, 5, 9 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListAppendZ( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  zListDestroy( zIntListCell, &l1 );
  zListDestroy( zIntListCell, &l2 );

  printf( "+++ append void lists +++\n" );
  zListAppendA( &l1, &l2 );
  list_write( &l1, "list1" );
  list_write( &l2, "list2" );
  return 0;
}
