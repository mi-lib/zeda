#include <zeda/zeda_list.h>

int main(void)
{
  register int i;
  zIntList list;
  zIntListCell *cp;

  zListInit( &list );
  for( i=0; i<10; i++ ){
    if( !( cp = zAlloc( zIntListCell, 1 ) ) ){
      ZALLOCERROR();
      exit( 1 );
    }
    cp->data = i;
    zListInsertTail( &list, cp );
  }

  while( !zListIsEmpty(&list) ){
    cp = zListHead( &list );
    zListPurge( &list, cp );
    printf( "value = %d\n", cp->data );
    zFree( cp );
  }
  zListPrint( (zList*)&list );
  zListDestroy( zIntListCell, &list );
  printf( "list test terminated.\n" );
  return 0;
}
