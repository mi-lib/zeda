#include <zeda/zeda_array.h>
#include <zeda/zeda_rand.h>

typedef struct{
  int val[3];
} int3_t;

zArrayClass( int3_array_t, int3_t );

void int3_randgen(int3_t *item)
{
  item->val[0] = zRandI( -10, 10 );
  item->val[1] = zRandI( -10, 10 );
  item->val[2] = zRandI( -10, 10 );
}

int cmp(void *v1, void *v2, void *dummy)
{
  if( ((int3_t*)v1)->val[0] > ((int3_t*)v2)->val[0] ) return 1;
  if( ((int3_t*)v1)->val[0] < ((int3_t*)v2)->val[0] ) return -1;
  if( ((int3_t*)v1)->val[1] > ((int3_t*)v2)->val[1] ) return 1;
  if( ((int3_t*)v1)->val[1] < ((int3_t*)v2)->val[1] ) return -1;
  if( ((int3_t*)v1)->val[2] > ((int3_t*)v2)->val[2] ) return 1;
  if( ((int3_t*)v1)->val[2] < ((int3_t*)v2)->val[2] ) return -1;
  return 0;
}

#define N 10

int main(void)
{
  register int i;
  int3_array_t array;
  int3_t *cp;

  zArrayAlloc( &array, int3_t, N );
  zRandInit();

  for( i=0; i<N; i++ ){
    int3_randgen( ( cp = zArrayElemNC(&array,i) ) );
    printf( "<%d> %3d %3d %3d\n", i, cp->val[0], cp->val[1], cp->val[2] );
  }
  printf( "Sorted.\n" );
  zArrayQuickSort( &array, cmp, NULL );
  for( i=0; i<N; i++ ){
    cp = zArrayElemNC(&array,i);
    printf( "<%d> %3d %3d %3d\n", i, cp->val[0], cp->val[1], cp->val[2] );
  }
  zArrayFree( &array );
  return 0;
}
