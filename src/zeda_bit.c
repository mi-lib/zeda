/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_bit - bit operations.
 */

#include <zeda/zeda_bit.h>

/* check type of endian of the current architecture. */
int endian_check(void)
{
  union{
    uint32_t val32;
    uint8_t val8[4];
  } testval;

  testval.val32 = 0xaabbccdd;
  if( testval.val8[0] == 0xaa &&
      testval.val8[1] == 0xbb &&
      testval.val8[2] == 0xcc &&
      testval.val8[3] == 0xdd ) return Z_ENDIAN_BIG;
  if( testval.val8[0] == 0xdd &&
      testval.val8[1] == 0xcc &&
      testval.val8[2] == 0xbb &&
      testval.val8[3] == 0xaa ) return Z_ENDIAN_LITTLE;
  /* middle endian not supported. */
  return Z_ENDIAN_UNKNOWN;
}

/* convert 16-bit little/bit endian to/from big/little endian. */
uint16_t endian_reverse16(uint16_t val)
{
  return ( val << 8 & 0xff00 ) | ( val >> 8 & 0xff );
}

/* convert 32-bit little/bit endian to/from big/little endian. */
uint32_t endian_reverse32(uint32_t val)
{
  return ( val << 24 & 0xff000000 ) |
         ( val <<  8 & 0x00ff0000 ) |
         ( val >>  8 & 0x0000ff00 ) |
         ( val >> 24 & 0x000000ff );
}

/* convert 64-bit little/bit endian to/from big/little endian. */
uint64_t endian_reverse64(uint64_t val)
{
  return ( val << 56 & 0xff00000000000000 ) |
         ( val << 40 & 0x00ff000000000000 ) |
         ( val << 24 & 0x0000ff0000000000 ) |
         ( val <<  8 & 0x000000ff00000000 ) |
         ( val >>  8 & 0x00000000ff000000 ) |
         ( val >> 24 & 0x0000000000ff0000 ) |
         ( val >> 40 & 0x000000000000ff00 ) |
         ( val >> 56 & 0x00000000000000ff );
}

/* rotate a bit sequence. */
ulong bit_rotate(ulong val, int bitwidth, int d)
{
  ulong mask;

  if( bitwidth > (int)( sizeof(ulong) << 3 ) )
    ZRUNWARN( ZEDA_WARN_TOOLNG_BITWIDTH );
  val &= ( mask = ( 1 << bitwidth ) - 1 ); /* masking */
  if( ( d %= bitwidth ) < 0 ) d += bitwidth;
  return ( ( val << d ) & mask ) | ( val >> ( bitwidth - d ) );
}

/* reverse a bit sequence. */
ulong bit_reverse(ulong val, int width)
{
  ulong result;

  for( result=0; width>0; val>>=1, width-- ){
    result <<= 1;
    result |= val & 0x1;
  }
  return result;
}

/* convert a bit sequence to a string. */
void sprintf_bit(char *str, ulong val, int width)
{
  register int i;
  ulong mask;

  mask = 1 << ( width - 1 );
  for( i=0; i<width; i++, mask>>=1 )
    str[i] = ( ( val & mask ) >> ( width - i - 1 ) ) + '0';
  str[i] = '\0';
}

#define ULONG_BIT_BUFSIZ 256
#ifndef __KERNEL__
/* output a bit sequence to file. */
void fprintf_bit(FILE *fp, ulong val, int width)
{
  char buf[ULONG_BIT_BUFSIZ];

  sprintf_bit( buf, val, width );
  fprintf( fp, "%s", buf );
}
#else
/* output a bit sequence (for kernel module). */
void printk_bit(ulong val, int width)
{
  char buf[ULONG_BIT_BUFSIZ];

  sprintf_bit( buf, val, width );
  printk( "%s", buf );
}
#endif /* __KERNEL__ */
