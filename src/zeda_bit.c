/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_bit - bit operations.
 */

#include <zeda/zeda_bit.h>

/* endian_reverse16
 * - convert 16-bit little/bit endian to/from big/little endian.
 */
uint16_t endian_reverse16(uint16_t val)
{
  union{
    uint16_t wval;
    uint8_t bval[2];
  } org, cnv;

  org.wval = val;
  cnv.bval[1] = org.bval[0];
  cnv.bval[0] = org.bval[1];
  return cnv.wval;
}

/* endian_reverse32
 * - convert 32-bit little/bit endian to/from big/little endian.
 */
uint32_t endian_reverse32(uint32_t val)
{
  union{
    uint32_t lval;
    uint8_t bval[4];
  } org, cnv;

  org.lval = val;
  cnv.bval[3] = org.bval[0];
  cnv.bval[2] = org.bval[1];
  cnv.bval[1] = org.bval[2];
  cnv.bval[0] = org.bval[3];
  return cnv.lval;
}

/* endian_reverse64
 * - convert 64-bit little/bit endian to/from big/little endian.
 */
uint64_t endian_reverse64(uint64_t val)
{
  union{
    uint64_t lval;
    uint8_t bval[8];
  } org, cnv;

  org.lval = val;
  cnv.bval[7] = org.bval[0];
  cnv.bval[6] = org.bval[1];
  cnv.bval[5] = org.bval[2];
  cnv.bval[4] = org.bval[3];
  cnv.bval[3] = org.bval[4];
  cnv.bval[2] = org.bval[5];
  cnv.bval[1] = org.bval[6];
  cnv.bval[0] = org.bval[7];
  return cnv.lval;
}

/* bit_rotate
 * - rotate a bit sequence.
 */
ulong bit_rotate(ulong val, int bitwidth, int d)
{
  ulong mask;

  if( bitwidth > (int)( sizeof(ulong) << 3 ) )
    ZRUNWARN( "too large bit width. the result might be wrong." );
  val &= ( mask = ( 1 << bitwidth ) - 1 ); /* masking */
  if( ( d %= bitwidth ) < 0 ) d += bitwidth;
  return ( ( val << d ) & mask ) | ( val >> ( bitwidth - d ) );
}

/* bit_reverse
 * - reverse a bit sequence.
 */
ulong bit_reverse(ulong val, int width)
{
  ulong result;

  for( result=0; width>0; val>>=1, width-- ){
    result <<= 1;
    result |= val & 0x1;
  }
  return result;
}

/* sprintf_bit
 * - convert a bit sequence to a string.
 */
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
/* fprintf_bit
 * - output a bit sequence to a file.
 */
void fprintf_bit(FILE *fp, ulong val, int width)
{
  char buf[ULONG_BIT_BUFSIZ];

  sprintf_bit( buf, val, width );
  fprintf( fp, "%s", buf );
}
#else
/* printk_bit
 * - output a bit sequence (for kernel module).
 */
void printk_bit(ulong val, int width)
{
  char buf[ULONG_BIT_BUFSIZ];

  sprintf_bit( buf, val, width );
  printk( "%s", buf );
}
#endif /* __KERNEL__ */
