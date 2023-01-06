#include <unistd.h>
#include <zeda/zeda_rand.h>

void assert_rand_int_gen(int min, int max, int val[], int n)
{
  int i;

  for( i=0; i<n; i++ )
    val[i] = zRandI( min, max );
}

void assert_rand_init_gen(ulong seed, int min, int max, int val[], int n)
{
  zRandInitSeed( seed );
  assert_rand_int_gen( min, max, val, n );
  zRandInit();
  assert_rand_int_gen( min, max, val+n, n );
}

void assert_rand_seed_gen(int min, int max, int val1[], int val2[], int n)
{
  ulong seed;

  seed = zRandInit();
  assert_rand_int_gen( min, max, val1, n );
  zRandInitSeed( seed );
  assert_rand_int_gen( min, max, val2, n );
}

bool assert_rand_check_equal(int val1[], int val2[], int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( val1[i] != val2[i] ) return false;
  return true;
}

bool assert_rand_check_diff(int val1[], int val2[], int n)
{
  int i;
  bool retval = false;

  for( i=0; i<n; i++ )
    if( val1[i] != val2[i] ) retval = true;
  return retval;
}

#define N     100
#define SEED 1234
#define MIN     0
#define MAX   100

void assert_rand_init()
{
  int val1[N*2], val2[N*2];

  assert_rand_init_gen( SEED, MIN, MAX, val1, N );
  sleep( 1 );
  assert_rand_init_gen( SEED, MIN, MAX, val2, N );
  zAssert( zRandInitSeedMT,
    assert_rand_check_equal( val1, val2, N ) && assert_rand_check_diff( val1+N, val2+N, N ) );
  sleep( 1 );
  assert_rand_seed_gen( MIN, MAX, val1, val2, N );
  zAssert( zRandInitMT, assert_rand_check_equal( val1, val2, N ) );
}

int main(int argc, char *argv[])
{
  assert_rand_init();
  return 0;
}
