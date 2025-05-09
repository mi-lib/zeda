#include <zeda/zeda_rand.h>
#include <zeda/zeda_array.h>

zArray2Class( IntArray, int );

int main(void)
{
  const int rowsize = 5, colsize = 3;
  zRandInit();
  IntArray array{ rowsize, colsize };
  for(int i=0; i<array.rowsize(); i++ ){
    for(int j=0; j<array.colsize(); j++ ){
      array[i][j] = zRandI( 0, array.rowsize() * array.colsize() );
    }
  }
  for(int i=0; i<array.rowsize(); i++ ){
    for(int j=0; j<array.colsize(); j++ ){
      std::cout << ' ' << *zArray2Elem(&array,i,j);
    }
    std::cout << std::endl;
  }
  for(int i=0; i<array.rowsize(); i++ ){
    for(int j=0; j<array.colsize(); j++ ){
      std::cout << "[" << i << "][" << j << "] " << array[i][j] << std::endl;
    }
  }
  return 0;
}
