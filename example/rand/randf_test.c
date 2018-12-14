#include <zeda/zeda_rand.h>

#define N 10000
#define DIV 100

int main(int argc, char *argv[])
{
  register int i;
  double max, min, d, val;
  int rnd;

  max = argc > 2 ? atof(argv[2]) : 1;
  min = argc > 1 ? atof(argv[1]) : 0;
  if( min > max ) zSwap( double, min, max );
  d = ( max - min ) / DIV;

  zRandInit();
  for( i=0; i<N; i++ ){
    val = zRandF( min, max );
    rnd = val / d;
    printf( "%d %.15f\n", rnd, val - rnd*d );
  }
  return 0;
}
