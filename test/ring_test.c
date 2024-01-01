#include <zeda/zeda.h>

#define N 5

zRingClass( int_ring_array_t, int );

bool cmp(int_ring_array_t *ira, int i0, int i1, int i2, int i3, int i4)
{
  return *zRingElem(ira,0) == i0 &&
         *zRingElem(ira,1) == i1 &&
         *zRingElem(ira,2) == i2 &&
         *zRingElem(ira,3) == i3 &&
         *zRingElem(ira,4) == i4 ? true : false;
}

int main(void)
{
  int_ring_array_t ira;
  int i;
  bool result;

  zRingAlloc( &ira, int, N );
  for( i=0; i<N; i++ )
    zRingSetElem( &ira, i, &i );

  result = true;
  zRingIncHead( &ira ); if( !cmp( &ira, 1, 2, 3, 4, 0 ) ) result = false;
  zRingIncHead( &ira ); if( !cmp( &ira, 2, 3, 4, 0, 1 ) ) result = false;
  zRingIncHead( &ira ); if( !cmp( &ira, 3, 4, 0, 1, 2 ) ) result = false;
  zRingIncHead( &ira ); if( !cmp( &ira, 4, 0, 1, 2, 3 ) ) result = false;
  zAssert( zRingIncHead, result );

  result = true;
  zRingDecHead( &ira ); if( !cmp( &ira, 3, 4, 0, 1, 2 ) ) result = false;
  zRingDecHead( &ira ); if( !cmp( &ira, 2, 3, 4, 0, 1 ) ) result = false;
  zRingDecHead( &ira ); if( !cmp( &ira, 1, 2, 3, 4, 0 ) ) result = false;
  zRingDecHead( &ira ); if( !cmp( &ira, 0, 1, 2, 3, 4 ) ) result = false;
  zAssert( zRingDecHead, result );

  zRingFree( &ira );
  return 0;
}
