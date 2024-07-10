#include <zeda/zeda_list.h>

void list_create(zIntList *list, int from, int to)
{
  int i;
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
  char list1[] = "list1";
  char list2[] = "list2";

  list_create( &l1, 0, 5 );
  list_print( &l1, list1 );

  printf( ">> duplicate\n" );
  zListDup( zIntListCell, &l1, &l2 );
  list_print( &l2, list2 );

  zListDestroy( zIntListCell, &l1 );
  zListDestroy( zIntListCell, &l2 );
  return 0;
}
