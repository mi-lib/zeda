#include <zeda/zeda_rand.h>

#define N 100

int main(int argc, char *argv[])
{
  register int i;
  int max, min, val;

  max = argc > 2 ? atoi(argv[2]) : 100;
  min = argc > 1 ? atoi(argv[1]) : 0;
  zRandInit();
  for( i=0; i<N; i++ ){
    val = zRandI( min, max );
    printf( "%d %d\n", val/10, val%10 );
  }
  return 0;
}
