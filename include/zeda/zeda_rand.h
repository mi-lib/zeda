/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_rand.h
 * \brief random number generator.
 *
 * This implementation of Mersenne twister proposed by
 * M. Matsumoto and T. Nishimura (1995) is a simple
 * rearrangement of the code written by Mr. Isaku Wada.
 * The original sources zmtrand.{h,c} are available at:
 *  http://www001.upp.so-net.ne.jp/isaku/index.html
 * \author Zhidao
 */

#include <zeda/zeda_misc.h>

#ifndef __ZEDA_RAND_H__
#define __ZEDA_RAND_H__

__BEGIN_DECLS

/*! random number operations are not available in the kernel space */
#ifndef __KERNEL__

/* ********************************************************** */
/*! \defgroup rand default random number generator
 * convenient reimplementations of the standard rand() function.
 * Note that they are naive implementations of pseudo-random
 * byte sequences.
 * \{ *//* ************************************************** */

/*! \cond */
#define Z_RAND_MT_HISTORY 623
/*! \endcond */

/* ********************************************************** */
/*! \brief random number generator based on Mersenne twister.
 *//********************************************************* */
typedef struct{
  ulong  x[Z_RAND_MT_HISTORY+1]; /*!< state vector */
  int    index;                  /*!< index */
  bool   nd_sw;                  /*!< a switch for normal distribution */
  double nd_last;                /*!< a memory for normal distribution */
} zRandMT;

/*! \brief initialize a random number generator based on Mersenne twister.
 *
 * zRandInitMT() initializes Mersenne twister \a mt
 * by seeding the current time for a new history.
 * If the null pointer is given for \a mt, it makes
 * use of the internal default instance.
 */
__EXPORT void zRandInitMT(zRandMT *mt);

/*! \brief a pseudo-random integer between \a min and \a max. */
__EXPORT int zRandMTI(zRandMT *mt, int min, int max);

/*! \brief a pseudo-random double-precision floating-point value
 * between \a min and \a max. */
__EXPORT double zRandMTF(zRandMT *mt, double min, double max);

/*! \brief a pseudo-random double-precision floating-point value
 * in the range of [0,1]. */
__EXPORT double zRandMTN(zRandMT *mt);

/*! \brief a pseudo-random value in the range of [0,1). */
__EXPORT double zRandMTNU(zRandMT *mt);

/*! \brief return a pointer to the default random number generator. */
__EXPORT zRandMT *zRandMTDefault(void);

#define zRandInit()     zRandInitMT( NULL )
#define zRandI(min,max) zRandMTI( NULL, min, max )
#define zRandF(min,max) zRandMTF( NULL, min, max )
#define zRandN()        zRandMTN( NULL )
#define zRandNU()       zRandMTNU( NULL )

/*! \} */

#endif /* __KERNEL__ */

__END_DECLS

#endif /* __ZEDA_RAND_H__ */
