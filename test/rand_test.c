#include <unistd.h>
#include <zeda/zeda_rand.h>

void assert_rand_init_gen(ulong seed, int min, int max, int val[], int n)
{
  int i;

  zRandInitSeed( seed );
  for( i=0; i<n; i++ )
    val[i] = zRandI( min, max );
  zRandInit();
  for( i=0; i<n; i++ )
    val[n+i] = zRandI( min, max );
}

bool assert_rand_init_check(int val1[], int val2[], int n)
{
  int i;
  bool retval = false;

  for( i=0; i<n; i++ )
    if( val1[i] != val2[i] ) return false;
  for( i=0; i<n; i++ )
    if( val1[n+i] != val2[n+i] ) retval = true;
  return retval;
}

#define N 100
#define SEED 1234
#define MIN     0
#define MAX   100

void assert_rand_init()
{
  int val1[N*2], val2[N*2];

  assert_rand_init_gen( SEED, MIN, MAX, val1, N );
  sleep( 1 );
  assert_rand_init_gen( SEED, MIN, MAX, val2, N );
  zAssert( zRandInitSeedMT and zRandInitMT, assert_rand_init_check( val1, val2, N ) );
}

int main(int argc, char *argv[])
{
  assert_rand_init();
  return 0;
}
