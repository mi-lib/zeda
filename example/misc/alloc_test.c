#include <zeda/zeda_misc.h>

#define SIZE1 100
#define SIZE2 1000

int main(void)
{
  int *buf1, *buf2, *buf3;

  buf1 = zAlloc( int, SIZE1 );
  buf2 = zAlloc( int, SIZE2 );
  printf( "allocated %d-byte memory from %p\n", SIZE1, buf1 );
  printf( "allocated %d-byte memory from %p\n", SIZE2, buf2 );
  buf1 = zRealloc( buf1, int, SIZE2 );
  buf3 = zAlloc( int, SIZE1 );
  printf( "reallocated %d-byte memory from %p\n", SIZE2, buf1 );
  printf( "newly allocated %d-byte memory from %p\n", SIZE1, buf3 );

  zFree( buf1 );
  zFree( buf2 );
  zFree( buf3 );
  return 0;
}
