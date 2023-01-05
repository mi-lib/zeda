/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_rand - random number generator.
 */

#include <zeda/zeda_rand.h>

#ifndef __KERNEL__

/* ********************************************************** */
/* CLASS: zRandMT
 * Mersenne twister
 *
 * This implementation of Mersenne twister proposed by
 * M. Matsumoto and T. Nishimura (1995) is a simple
 * rearrangement of the code written by Mr. Isaku Wada.
 * The original sources zmtrand.{h,c} are available at:
 *  http://www001.upp.so-net.ne.jp/isaku/index.html
 * but, debugged uninitialized flag.
 * **********************************************************
 */

/* default history of Mersenne twister */
static zRandMT __z_rand_mt_default, *__z_rand_mt = NULL;

#define Z_RAND_MT_MAX  0xFFFFFFFFUL
#define Z_RAND_MT_MGC1 1812433253UL
#define Z_RAND_MT_MGC2 0x9908B0DFUL
#define Z_RAND_MT_MGC3 0x9D2C5680UL
#define Z_RAND_MT_MGC4 0xEFC60000UL
#define Z_RAND_MT_CYC1 227
#define Z_RAND_MT_CYC2 396
#define Z_RAND_MT_MASK 0x80000000UL

/* random value (32 bit, unsigned) generated by Mersenne twister. */
static ulong _zRandMTVal(zRandMT *mt)
{
  static ulong mag[] = { 0, Z_RAND_MT_MGC2 };
  ulong r;
  int i;

  if( !mt && !( mt = __z_rand_mt ) ){
    zRandInitMT( NULL );
    mt = __z_rand_mt;
  }
  if( ( i = mt->index ) == 0 ){
    for( ; i<Z_RAND_MT_CYC1; i++ ){
      r = ( mt->x[i]&Z_RAND_MT_MASK ) | ( mt->x[i+1]&~Z_RAND_MT_MASK );
      mt->x[i] = mt->x[i+Z_RAND_MT_CYC2+1]^(r>>1)^mag[r&1];
    }
    for( ; i<Z_RAND_MT_HISTORY; i++ ){
      r = ( mt->x[i]&Z_RAND_MT_MASK ) | ( mt->x[i+1]&~Z_RAND_MT_MASK );
      mt->x[i] = mt->x[i-Z_RAND_MT_CYC1  ]^(r>>1)^mag[r&1];
    } /* i = Z_RAND_MT_HISTORY */
      r = ( mt->x[i]&Z_RAND_MT_MASK ) | ( mt->x[0]  &~Z_RAND_MT_MASK );
      mt->x[i] = mt->x[  Z_RAND_MT_CYC2  ]^(r>>1)^mag[r&1];
    i=0;
  }
  r = mt->x[i];
  if( ++i > Z_RAND_MT_HISTORY ) i = 0;
  mt->index = i;
  /* twisting */
  r ^= (r>>11);
  r ^= (r<<7 ) & Z_RAND_MT_MGC3;
  r ^= (r<<15) & Z_RAND_MT_MGC4;
  r ^= (r>>18);
  return r;
}

/* initialize Mersenne twister with an explicit seed. */
void zRandInitSeedMT(zRandMT *mt, ulong seed)
{
  uint i;

  if( !mt )
    mt = __z_rand_mt = &__z_rand_mt_default;
  mt->x[0] = ( seed &= Z_RAND_MT_MAX );
  for( i=1; i<=Z_RAND_MT_HISTORY; i++ )
    mt->x[i] = seed
      = ( Z_RAND_MT_MGC1 * (seed^(seed>>30)) + i ) & Z_RAND_MT_MAX;
  mt->index = 0;
  for( i=0; i<Z_RAND_MT_HISTORY; i++ )
    _zRandMTVal( mt ); /* skip the first Z_RAND_MT_HISTORY numbers. */
  /* for normal distribution */
  mt->nd_sw = false;
  mt->nd_last = 0;
}

/* initialize Mersenne twister. */
void zRandInitMT(zRandMT *mt)
{
  static ulong seed;

  seed = time( NULL );
  if( mt ) seed++;
  zRandInitSeedMT( mt, seed );
}

/* random integer number by Mersenne twister. */
int zRandMTI(zRandMT *mt, int min, int max)
{
  return _zRandMTVal( mt ) % ( max - min + 1 ) + min;
}

/* random double-precision floating-point number by Mersenne twister. */
double zRandMTF(zRandMT *mt, double min, double max)
{
  return (double)_zRandMTVal( mt ) / Z_RAND_MT_MAX * ( max - min ) + min;
}

/* random [0,1] double-precision floating-point number by Mersenne twister. */
double zRandMTN(zRandMT *mt)
{
  return (double)_zRandMTVal( mt ) / Z_RAND_MT_MAX;
}

/* random [0,1) double-precision floating-point number by Mersenne twister. */
double zRandMTNU(zRandMT *mt)
{
  return (double)_zRandMTVal( mt ) / ( (double)Z_RAND_MT_MAX + 1 );
}

/* return a pointer to the default random number generator. */
zRandMT *zRandMTDefault(void){ return __z_rand_mt; }

#endif /* __KERNEL__ */
