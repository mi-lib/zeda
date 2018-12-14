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

#define WIDTH 8

int main(void)
{
  ubyte b[WIDTH];
  ulong val;

  zRandInit();
  val = gen_bit_rand( b, WIDTH );
  zAssert( bit_rotate,
    bit_rotate(val,WIDTH,0) == compose_bit(b,0,1,2,3,4,5,6,7) &&
    bit_rotate(val,WIDTH,1) == compose_bit(b,1,2,3,4,5,6,7,0) &&
    bit_rotate(val,WIDTH,2) == compose_bit(b,2,3,4,5,6,7,0,1) &&
    bit_rotate(val,WIDTH,3) == compose_bit(b,3,4,5,6,7,0,1,2) &&
    bit_rotate(val,WIDTH,4) == compose_bit(b,4,5,6,7,0,1,2,3) &&
    bit_rotate(val,WIDTH,5) == compose_bit(b,5,6,7,0,1,2,3,4) &&
    bit_rotate(val,WIDTH,6) == compose_bit(b,6,7,0,1,2,3,4,5) &&
    bit_rotate(val,WIDTH,7) == compose_bit(b,7,0,1,2,3,4,5,6) &&
    bit_rotate(val,WIDTH,-1) == compose_bit(b,7,0,1,2,3,4,5,6) &&
    bit_rotate(val,WIDTH,-2) == compose_bit(b,6,7,0,1,2,3,4,5) &&
    bit_rotate(val,WIDTH,-3) == compose_bit(b,5,6,7,0,1,2,3,4) &&
    bit_rotate(val,WIDTH,-4) == compose_bit(b,4,5,6,7,0,1,2,3) &&
    bit_rotate(val,WIDTH,-5) == compose_bit(b,3,4,5,6,7,0,1,2) &&
    bit_rotate(val,WIDTH,-6) == compose_bit(b,2,3,4,5,6,7,0,1) &&
    bit_rotate(val,WIDTH,-7) == compose_bit(b,1,2,3,4,5,6,7,0) );
  zAssert( bit_reverse, bit_reverse(val,WIDTH) == compose_bit(b,7,6,5,4,3,2,1,0) );

  zAssert( endian_reverse16, endian_reverse16(0x1234) == 0x3412 );
  zAssert( endian_reverse32, endian_reverse32(0x12345678) == 0x78563412 );
  zAssert( endian_reverse64, endian_reverse64(0x12345678abcdefab) == 0xabefcdab78563412 );
  return EXIT_SUCCESS;
}
