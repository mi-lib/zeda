/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_ring.h
 * \brief ring buffer.
 * \author Zhidao
 */

#ifndef __ZEDA_RING_H__
#define __ZEDA_RING_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup ring universal ring buffer.
 * \{ *//* ************************************************** */

/*! \def zRingClass(ring_t,cell_t)
 * \brief generate ring buffer class.
 *
 * A macro zRingClass() can generate a new ring buffer
 * class, which consists of the total number of elements,
 * the current head of the ring buffer, and the pointer
 * to the ring buffer.
 * \a ring_t is the class name to be defined.
 * \a cell_t is the class name of the data to be arrayed.
 *
 * To allocate and free a ring buffer, use zRingAlloc()
 * and zRingFree().
 *
 * The number of elements is acquired by zRingSize().
 * The actual array head can be accessed by zRingBuf().
 * But, the ring buffer conceptually conceal the body
 * array. zRingHead() is preferable to be used, instead.
 * It allows to access to the data at the current head.
 * An elements after some elements the head can be safely
 * accessed by zRingElem() and zRingSetElem().
 *
 * zRingReset() resets the head position to the actual
 * head of the array.
 *
 * To advance and get back the head, use zRingIncHead()
 * and zRingDecHead(), respectively.
 */
#ifdef __cplusplus
#define zRingClass(ring_t,cell_t) \
struct ring_t{\
  int size;\
  int head;\
  cell_t *buf;\
  ring_t(){ size=head=0; buf=NULL; };\
  ~ring_t(){ if( buf ) delete [] buf; };\
}
#else
#define zRingClass(ring_t,cell_t) \
typedef struct{\
  int size;\
  int head;\
  cell_t *buf;\
} ring_t
#endif /* __cplusplus */

#define zRingSize(y)     (y)->size
#define zRingBuf(y)      ( (y)->buf )

/* NOTE: do not use the following macro before allocating buffer */
#define zRingElemSize(y) sizeof(*zRingBuf(y))

#define zRingHead(y)     ( &zRingBuf(y)[(y)->head] )
#define zRingElem(y,i)   ( &zRingBuf(y)[((y)->head+(i)) % (y)->size] )
#define zRingSetElem(a,i,d) \
  memcpy( zRingElem(a,i), (d), zRingElemSize(a) )

#define zRingReset(y) ( (y)->head = 0 );
#define zRingInit(y) do{\
  zRingSize(y) = (y)->head = 0;\
  zRingBuf(y) = NULL;\
} while(0)

/*! \brief allocate an array.
 * \param y ring buffer class instance to be created.
 * \param type the data type of the array cells.
 * \param s the size of the array.
 */
#define zRingAlloc(y,type,size) do{\
  zRingInit( y );\
  if( !( zRingBuf(y) = zAlloc(type,size) ) )\
    ZALLOCERROR();\
  else\
    zRingSize(y) = (size);\
} while(0)

#define zRingFree(y) do{\
  zFree( zRingBuf(y) );\
  zRingInit( y );\
} while(0)

#define zRingIncHead(y) do{\
  if( ++(y)->head >= zRingSize(y) ) (y)->head = 0;\
} while(0)
#define zRingDecHead(y) do{\
  if( --(y)->head < 0 ) (y)->head = zRingSize(y) - 1;\
} while(0)

/*! \} */

__END_DECLS

#endif /* __ZEDA_RING_H__ */
