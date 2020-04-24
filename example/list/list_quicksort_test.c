#include <zeda/zeda_list.h>
#include <zeda/zeda_string.h>
#include <zeda/zeda_rand.h>

zListQuickSortDef(zIntList, zIntListCell)

int cmp(void *v1, void *v2, void *dummy)
{
  if( ((zIntListCell*)v1)->data == ((zIntListCell*)v2)->data ) return 0;
  return ( ((zIntListCell*)v1)->data > ((zIntListCell*)v2)->data ) ? 1 : -1;
}

int main(void)
{
  register int i;
  zIntList list;
  zIntListCell *cp;

  zRandInit();
  zListInit( &list );

  for( i=0; i<100; i++ ){
    if( !( cp = zAlloc( zIntListCell, 1 ) ) ){
      ZALLOCERROR();
      exit( 1 );
    }
    cp->data = zRandI( -100, 100 );
    printf( "<%d> %p %d\n", i, cp, cp->data );
    zListInsertHead( &list, cp );
  }
  printf( "sorted.\n" );
  zIntListQuickSort( &list, cmp, NULL );
  for( i=0; !zListIsEmpty(&list); i++ ){
    zListDeleteTail( &list, &cp );
    printf( "%d ", cp->data );
    zFree( cp );
    if( i % 10 == 9 ) zEndl();
  }
  zListDestroy( zIntListCell, &list );
  return 0;
}
