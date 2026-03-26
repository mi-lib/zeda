#include <zeda/zeda.h>

ulong gen_bit_rand(ubyte b[], int bitwidth)
{
  ulong val = 0;

  while( --bitwidth >= 0 ){
    val <<= 1;
    val |= ( b[bitwidth] = rand() % 2 );
  }
  return val;
}

ulong compose_bit(ubyte b[], int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7)
{
  return b[0] << i0 | b[1] << i1 | b[2] << i2 | b[3] << i3 | b[4] << i4 | b[5] << i5 | b[6] << i6 | b[7] << i7;
}

void assert_bit_getset(void)
{
  ubyte b[8];
  ubyte val, val_check;
  int i;

  val = gen_bit_rand( b, 8 );
  for( val_check=0, i=0; i<8; i++ )
    bit_get( val, i ) ? bit_set( val_check, i ) : bit_reset( val_check, i );
  zAssert( bit_get / bit_set / bit_reset, val == val_check );
}

void assert_bit_cmp(void)
{
  ubyte val1;
  ubyte val2;
  ubyte val_cmp;

  val1 = bitstrtoi( "01011101" );
  val2 = bitstrtoi( "01101011" );
  val_cmp = bitstrtoi( "11001001" );
  zAssert( bit_cmp, (ubyte)bit_cmp( val1, val2 ) == val_cmp );
}

void assert_bit_rotate(void)
{
  ubyte b[8];
  ulong val;

  val = gen_bit_rand( b, 8 );
  zAssert( bit_rotate,
    bit_rotate(val,8,0) == compose_bit(b,0,1,2,3,4,5,6,7) &&
    bit_rotate(val,8,1) == compose_bit(b,1,2,3,4,5,6,7,0) &&
    bit_rotate(val,8,2) == compose_bit(b,2,3,4,5,6,7,0,1) &&
    bit_rotate(val,8,3) == compose_bit(b,3,4,5,6,7,0,1,2) &&
    bit_rotate(val,8,4) == compose_bit(b,4,5,6,7,0,1,2,3) &&
    bit_rotate(val,8,5) == compose_bit(b,5,6,7,0,1,2,3,4) &&
    bit_rotate(val,8,6) == compose_bit(b,6,7,0,1,2,3,4,5) &&
    bit_rotate(val,8,7) == compose_bit(b,7,0,1,2,3,4,5,6) &&
    bit_rotate(val,8,-1) == compose_bit(b,7,0,1,2,3,4,5,6) &&
    bit_rotate(val,8,-2) == compose_bit(b,6,7,0,1,2,3,4,5) &&
    bit_rotate(val,8,-3) == compose_bit(b,5,6,7,0,1,2,3,4) &&
    bit_rotate(val,8,-4) == compose_bit(b,4,5,6,7,0,1,2,3) &&
    bit_rotate(val,8,-5) == compose_bit(b,3,4,5,6,7,0,1,2) &&
    bit_rotate(val,8,-6) == compose_bit(b,2,3,4,5,6,7,0,1) &&
    bit_rotate(val,8,-7) == compose_bit(b,1,2,3,4,5,6,7,0) );
}

void assert_bit_reverse(void)
{
  ubyte b[8];
  ulong val;

  val = gen_bit_rand( b, 8 );
  zAssert( bit_reverse, bit_reverse(val,8) == compose_bit(b,7,6,5,4,3,2,1,0) );
}

void assert_endian_reverse(void)
{
  zAssert( endian_reverse16, endian_reverse16(0x1234) == 0x3412 );
  zAssert( endian_reverse32, endian_reverse32(0x12345678) == 0x78563412 );
  zAssert( endian_reverse64, endian_reverse64(0x12345678abcdefab) == 0xabefcdab78563412 );
}

void assert_endian_check(void)
{
#ifndef __WINDOWS__
  zAssert( endian_check, endian_check() == __BYTE_ORDER );
#endif
}

void assert_bitstrtoi(void)
{
  ubyte bval;
  ulong lval;

  bval = bitstrtoi( "11010011" );
  zAssert( bitstrtoi (regular case), bval == 0xd3 );
  bval = bitstrtoi( "11a1cd11" );
  zAssert( bitstrtoi (irregular case), bval == 0xd3 );
  lval = bitstrtoi( "1101001110100111" );
  zAssert( bitstrtoi (long case), lval == 0xd3a7 );
}

int main(void)
{
  zRandInit();
  assert_bit_getset();
  assert_bit_cmp();
  assert_bit_rotate();
  assert_bit_reverse();
  assert_endian_reverse();
  assert_endian_check();
  assert_bitstrtoi();
  return EXIT_SUCCESS;
}
