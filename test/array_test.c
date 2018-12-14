#include <zeda/zeda.h>

zArrayClass( int_array_t, int );

int cmp(void *v1, void *v2, void *dummy)
{
  if( *(int*)v1 > *(int*)v2 ) return 1;
  if( *(int*)v1 < *(int*)v2 ) return -1;
  return 0;
}

int assert_quicksort(int n)
{
  register int i;
  int_array_t array;
  int val;

  zArrayAlloc( &array, int, n );
  zRandInit();
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArraySetElemNC( &array, i, &val );
  }
  zArrayQuickSort( &array, cmp, NULL );
  for( i=1; i<n; i++ )
    if( *zArrayElemNC(&array,i-1) > *zArrayElemNC(&array,i) ) return 0;
  zArrayFree( &array );
  return 1;
}

#define N 100

int main(void)
{
  zAssert( zArrayQuickSort, assert_quicksort( N ) );
  return EXIT_SUCCESS;
}
