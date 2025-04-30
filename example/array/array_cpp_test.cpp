#include <zeda/zeda_rand.h>
#include <zeda/zeda_array.h>

zArrayClass( IntArray, int );

int cmp(void *v1, void *v2, void *dummy)
{
  if( *static_cast<int*>(v1) > *static_cast<int*>(v2) ) return 1;
  if( *static_cast<int*>(v1) < *static_cast<int*>(v2) ) return -1;
  return 0;
}

int main(void)
{
  const int size = 10;
  zRandInit();
  IntArray array{ size };
  for(int i=0; i<array.size; i++ ){
    array.buf[i] = zRandI( 0, array.size );
  }
  array.sort( cmp, NULL );
  for(int i=0; i<array.size; i++ ){
    std::cout << "[" << i << "] " << *array.get( i ) << std::endl;
  }
  return 0;
}
