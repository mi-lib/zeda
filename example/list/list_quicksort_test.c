#include <zeda/zeda_list.h>
#include <zeda/zeda_string.h>
#include <zeda/zeda_rand.h>

zListClass(list_t, cell_t, int);
zListQuickSortDef(list_t, cell_t)

int cmp(void *v1, void *v2, void *dummy)
{
  if( ((cell_t*)v1)->data == ((cell_t*)v2)->data ) return 0;
  return ( ((cell_t*)v1)->data > ((cell_t*)v2)->data ) ? 1 : -1;
}

int main(void)
{
  register int i;
  list_t list;
  cell_t *cp;

  zRandInit();
  zListInit( &list );

  for( i=0; i<100; i++ ){
    if( !( cp = zAlloc( cell_t, 1 ) ) ){
      ZALLOCERROR();
      exit( 1 );
    }
    cp->data = zRandI( -100, 100 );
    printf( "<%d> %p %d\n", i, cp, cp->data );
    zListInsertHead( &list, cp );
  }
  printf( "sorted.\n" );
  list_tQuickSort( &list, cmp, NULL );
  for( i=0; !zListIsEmpty(&list); i++ ){
    zListDeleteTail( &list, &cp );
    printf( "%d ", cp->data );
    zFree( cp );
    if( i % 10 == 9 ) zEndl();
  }
  zListDestroy( cell_t, &list );
  return 0;
}
