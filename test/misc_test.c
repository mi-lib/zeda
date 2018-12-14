#include <zeda/zeda.h>
#include <math.h>

void assert_swap(void)
{
  char ac, ac0, bc, bc0;
  int ai, ai0, bi, bi0;

  ac0 = ac = 'A';
  bc0 = bc = 'B';
  zSwap( char, ac, bc );

  ai0 = ai = 1;
  bi0 = bi = 2;
  zSwap( int, ai, bi );

  zAssert( zSwap, ac == bc0 && bc == ac0 && ai == bi0 && bi == ai0 );
}

void assert_itoa(void)
{
  char str[BUFSIZ];

  zAssert( itoa, strcmp( itoa( 123, str ), "123" ) == 0 );
  zAssert( itoa_fill, strcmp( itoa_fill( 123, 5, '*', str ), "**123" ) == 0 && strcmp( itoa_fill( 123, 2, '*', str ), "123" ) == 0 );
  zAssert( itoa_zerofill, strcmp( itoa_zerofill( 123, 5, str ), "00123" ) == 0 && strcmp( itoa_zerofill( 123, 2, str ), "123" ) == 0 );
}

bool assert_ftoa_one(double val)
{
  char buf[BUFSIZ];

  return fabs( val - atof( ftoa( val, buf ) ) ) < 1e-10 ? true : false;
}

void assert_ftoa(void)
{
  bool result = true;

  assert_ftoa_one( 0.0 );
  assert_ftoa_one( 123.456 );
  assert_ftoa_one( 123456789 );
  assert_ftoa_one( 1.234567 );
  assert_ftoa_one( 0.12345 );
  assert_ftoa_one( 0.0123 );
  assert_ftoa_one( 0.00000123456 );
  assert_ftoa_one( 1.2345e-7 );
  assert_ftoa_one( 1.2345e-12 );
  assert_ftoa_one( 1.2345e7 );
  assert_ftoa_one( 1.2345e12 );
  assert_ftoa_one( 1.999999999 );
  assert_ftoa_one( 1.999999999999 );
  assert_ftoa_one( 0.0001999999999 );
  assert_ftoa_one( 0.00001 );
  assert_ftoa_one( 0.000001 );
  assert_ftoa_one( 0.0000011 );
  assert_ftoa_one( 0.000000000001 );
  zAssert( ftoa, result );
}

int main(void)
{
  zEchoOff();
  zAssert( zMax, zMax(1,0) == 1 );
  zAssert( zMin, zMin(1,0) == 0 );
  zAssert( zLimit, zLimit( 0.5, 0, 1 ) == 0.5 && zLimit( -1, 0, 1 ) == 0 && zLimit( 10, 0, 1 ) == 1 );
  zAssert( zBound, zBound( 0.5, 1, 0 ) == zBound( 0.5, 0, 1 ) );
  assert_swap();
  zAssert( atox, atox( "1g2h3i" ) == 0x102030 && atox( "1a2b3c" ) == 0x1a2b3c );
  assert_itoa();
  assert_ftoa();
  return EXIT_SUCCESS;
}
