/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_rand - random number generator.
 */

#include <zeda/zeda_rand.h>

/* ********************************************************** */
/* default random number generator
 * ********************************************************** */

#ifndef __KERNEL__
/* zRandInit
 * - initialization of pseudo random number sequence.
 */
void zRandInit(void)
{
  srand( (int)time( NULL ) );
}

/* zRandI
 * - integer random number.
 */
int zRandI(int min, int max)
{
  return rand() % ( max - min + 1 ) + min;
}

/* zRandF
 * - double-precision floating-point random number.
 */
double zRandF(double min, double max)
{
  return (double)rand() / RAND_MAX * ( max - min ) + min;
}
#endif /* __KERNEL__ */
