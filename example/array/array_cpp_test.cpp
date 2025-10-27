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
  IntArray array_org{ size }, array_sorted;
  for(int i=0; i<array_org.size; i++ ){
    *array_org[i] = zRandI( 0, array_org.size );
  }
  std::cout << "original array" << std::endl;
  for(int i=0; i<array_org.size; i++ ){
    std::cout << "[" << i << "] " << *array_org[i] << std::endl;
  }
  array_sorted.clone( array_org );
  array_sorted.sort( cmp, NULL );
  std::cout << "sorted array" << std::endl;
  for(int i=0; i<array_sorted.size; i++ ){
    std::cout << "[" << i << "] " << *array_sorted[i] << std::endl;
  }
  std::cout << "head: " << *array_sorted.head() << std::endl;
  std::cout << "neck: " << *array_sorted.neck() << std::endl;
  std::cout << "tail: " << *array_sorted.tail() << std::endl;
  return 0;
}
