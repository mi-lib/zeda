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

void assert_clone(void)
{
  byte src[BUFSIZ], *dest;
  int i;

  for( i=0; i<BUFSIZ; i++ )
    src[i] = zRandI(0,0xff);
  zAssert( zClone, ( dest = zClone( src, BUFSIZ ) ) && memcmp( dest, src, BUFSIZ ) == 0 );
  free( dest );
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

  if( !assert_ftoa_one( 0.0 ) ) result = false;
  if( !assert_ftoa_one( 123.456 ) ) result = false;
  if( !assert_ftoa_one( 123456789 ) ) result = false;
  if( !assert_ftoa_one( 1.234567 ) ) result = false;
  if( !assert_ftoa_one( 0.12345 ) ) result = false;
  if( !assert_ftoa_one( 0.0123 ) ) result = false;
  if( !assert_ftoa_one( 0.00000123456 ) ) result = false;
  if( !assert_ftoa_one( 1.2345e-7 ) ) result = false;
  if( !assert_ftoa_one( 1.2345e-12 ) ) result = false;
  if( !assert_ftoa_one( 1.2345e7 ) ) result = false;
  if( !assert_ftoa_one( 1.2345e12 ) ) result = false;
  if( !assert_ftoa_one( 1.999999999 ) ) result = false;
  if( !assert_ftoa_one( 1.999999999999 ) ) result = false;
  if( !assert_ftoa_one( 0.0001999999999 ) ) result = false;
  if( !assert_ftoa_one( 0.00001 ) ) result = false;
  if( !assert_ftoa_one( 0.000001 ) ) result = false;
  if( !assert_ftoa_one( 0.0000011 ) ) result = false;
  if( !assert_ftoa_one( 0.000000000001 ) ) result = false;
  zAssert( ftoa, result );
}

bool assert_itoa_ordinal_one(int val, const char *str)
{
  char buf[BUFSIZ];
  return strncmp( itoa_ordinal( val, buf, BUFSIZ ), str, BUFSIZ ) == 0;
}

void assert_itoa_ordinal(void)
{
  bool result = true;

  if( !assert_itoa_ordinal_one(   1,   "1st" ) ) result = false;
  if( !assert_itoa_ordinal_one(   2,   "2nd" ) ) result = false;
  if( !assert_itoa_ordinal_one(   3,   "3rd" ) ) result = false;
  if( !assert_itoa_ordinal_one(   4,   "4th" ) ) result = false;
  if( !assert_itoa_ordinal_one(  11,  "11th" ) ) result = false;
  if( !assert_itoa_ordinal_one(  12,  "12th" ) ) result = false;
  if( !assert_itoa_ordinal_one(  13,  "13th" ) ) result = false;
  if( !assert_itoa_ordinal_one( 101, "101st" ) ) result = false;
  if( !assert_itoa_ordinal_one( 102, "102nd" ) ) result = false;
  if( !assert_itoa_ordinal_one( 103, "103rd" ) ) result = false;
  if( !assert_itoa_ordinal_one( 104, "104th" ) ) result = false;
  if( !assert_itoa_ordinal_one( 111, "111th" ) ) result = false;
  if( !assert_itoa_ordinal_one( 112, "112th" ) ) result = false;
  if( !assert_itoa_ordinal_one( 113, "113th" ) ) result = false;
  zAssert( itoa_ordinal, result );
}

int main(void)
{
  zEchoOff();
  zAssert( zMax, zMax(1,0) == 1 );
  zAssert( zMin, zMin(1,0) == 0 );
  zAssert( zLimit, zLimit( 0.5, 0, 1 ) == 0.5 && zLimit( -1, 0, 1 ) == 0 && zLimit( 10, 0, 1 ) == 1 );
  zAssert( zBound, zBound( 0.5, 1, 0 ) == zBound( 0.5, 0, 1 ) );
  assert_swap();
  assert_clone();
  zAssert( atox, atox( "1g2h3i" ) == 0x102030 && atox( "1a2b3c" ) == 0x1a2b3c );
  assert_itoa();
  assert_ftoa();
  assert_itoa_ordinal();
  return EXIT_SUCCESS;
}
