/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_index.h
 * \brief integer value array.
 * \author Zhidao
 */

#ifndef __ZEDA_INDEX_H__
#define __ZEDA_INDEX_H__

#include <stdarg.h>
#include <zeda/zeda_array.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup index integer value array.
 * \{ *//* ************************************************** */

/* ********************************************************** */
/*! \struct zIndex
 * \brief integer value array class
 *
 * zIndex is a container of an array of integer values.
 * The body structure is an instance of zIndexStruct.
 * \sa zArrayClass.
 *
 * To access elements of the integer array, use zIndexElem()
 * and zIndexSetElem().
 *
 * For frequent accesses to the head, neck (one-cell-before-head)
 * and tail, zIndexHead(), zIndexNeck() and zIndexTail()
 * are available.
 *//* ******************************************************* */
zArrayClass( zIndexStruct, int );
typedef zIndexStruct *zIndex;

#define zIndexIncNum(i)      ( zArrayNum(i)++ )
#define zIndexDecNum(i)      ( zArrayNum(i) -= (zArrayNum(i)>0 ? 1:0) )

#define zIndexElem(i,n)      ( *zArrayElemNC(i,n) )
#define zIndexSetElem(i,n,e) ( zIndexElem(i,n) = (e) )

#define zIndexHead(idx)      ( *zArrayHead(idx) )
#define zIndexNeck(idx)      ( *zArrayNeck(idx) )
#define zIndexTail(idx)      ( *zArrayTail(idx) )

/*! \brief allocate an array of integer values.
 *
 * zIndexAlloc() allocates an internal array of a zIndex instance
 * \a idx with a requested size \a size.
 * \return a pointer \a idx. Or, the null pointer if failing
 * to allocate new memories.
 */
__EXPORT zIndex zIndexAlloc(zIndex idx, int size);

/*! \brief create an array of integer values.
 *
 * zIndexCreate() creates a new array of integer values
 * with a size \a size. It initializes all values of the
 * array as { 0, 1, 2, ..., \a size -1 }.
 * \return a pointer to the zIndex instance newly allocated.
 * Or, the null pointer when failing to allocate new zIndex instance.
 */
__EXPORT zIndex zIndexCreate(int size);

/*! \brief create an array of integer according to a
 * specified list of arguments.
 *
 * zIndexCreateList() creates an array of integer values
 * from the specified list of arguments. \a num is the
 * number of values.
 * \return a pointer to the zIndex instance newly allocated.
 * Or, the null pointer when failing to allocate new zIndex instance.
 * \note
 * zIndexCreateList() is not available in the kernel space.
 */
#ifndef __KERNEL__
zIndex zIndexCreateList(int num, ...);
#endif /* __KERNEL__ */

/*! \brief set elements of an integer vector from argument list.
 *
 * zIndexSetList() sets elements of an array of integer values \a idx
 * from the specified list of arguments.
 * \retval \a idx
 * \note
 * zIndexCreateList() is not available in the kernel space.
 */
#ifndef __KERNEL__
zIndex zIndexSetList(zIndex idx, ...);
#endif /* __KERNEL__ */

/*! \brief free an array of integer values.
 *
 * zIndexFree() frees a zIndex instance \a idx,
 * freeing the assigned memory space.
 */
__EXPORT void zIndexFree(zIndex idx);

/*! \brief cleanup an array of integer values.
 *
 * zIndexClear() sets all elements of a zIndex instance
 * \a idex for zero.
 * \return a pointer \a idx.
 */
__EXPORT zIndex zIndexClear(zIndex idx);

/*! \brief order an array of integer values.
 *
 * zIndexOrder() sets all elements of \a idx in order,
 * beginning from \a s as { \a s, \a s+1, \a s+2, ... }.
 * \return a pointer \a idx.
 * \note
 * zIndexOrder() is not available in the kernel space.
 */
#ifndef __KERNEL__
__EXPORT zIndex zIndexOrder(zIndex idx, int s);
#endif /* __KERNEL__ */

/*! \brief check if two arrays of integer values are equal.
 *
 * zIndexIsEqual() checks if two arrays of integer values
 * \a idx1 and \a idx2 are equal.
 * \return the true value if \a idx1 and \a idx2 are equal,
 * or the false values, otherwise.
 */
__EXPORT bool zIndexIsEqual(zIndex idx1, zIndex idx2);

/*! \brief swap two elements of an array of integer values.
 *
 * zIndexSwap() swaps two components of a zIndex instance
 * \a idx at the positions of \a p1 and \a p2.
 * \return a newly swapped value at \a p2.
 * \note
 * if \a p1 or \a p2 is invalid, anything might happen.
 */
__EXPORT int zIndexSwap(zIndex idx, int p1, int p2);

/*! \brief move an element of an array of integer values.
 *
 * zIndexMove() moves a component at \a from' of a zIndex
 * instance \a idx to another position \a to. The components
 * between \a from and \a to are slided to make a space to
 * insert the component.
 * \return a newly replaced value at \a to.
 * \note
 * if \a from or \a to is invalid, anything might happen.
 */
__EXPORT int zIndexMove(zIndex idx, int from, int to);

#ifndef __KERNEL__
/*! \brief read an array of integer values from a file.
 *
 * zIndexFRead() reads a sequence of integer values from
 * the current position of a file \a fp, and create a new
 * zIndex instance.
 * The format to be read by zIndexFRead() is as follows:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexRead() is also available to read values from the
 * standard input.
 * \return a pointer to the newly allocated zIndex instance.
 * \sa zIndexRead, zIndexFWrite
 */
__EXPORT zIndex zIndexFRead(FILE *fp);
#define zIndexRead()   zIndexFRead( stdin )
/*! \brief output an array of integer values to a file.
 *
 * zIndexFWrite() outputs an array of integer values in
 * a zIndex instance \a idx to the current position of
 * a file \a fp in the following format:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexWrite() is also available to output values to the
 * standard output.
 * \sa zIndexFRead, zIndexWrite, zIndexDataFWrite
 */
__EXPORT void zIndexFWrite(FILE *fp, zIndex idx);
#define zIndexWrite(i) zIndexFWrite( stdout, i )
/*! \brief output an array of integer values to a file.
 *
 * zIndexDataFWrite() outputs an array of integer values
 * in a zIndex instance \a idx to the current position of
 * a file \a fp in the following format:
 *
 *  x1 x2 x3 ... xn
 *
 * zIndexDataWrite() is also available to output values
 * to the standard output.
 * \sa zIndexFRead, zIndexFWrite, zIndexDataWrite
 */
__EXPORT void zIndexDataFWrite(FILE *fp, zIndex idx);
#define zIndexDataWrite(i) zIndexDataFWrite( stdout, i )
#else
void zIndexWrite(zIndex idx);
#endif /* __KERNEL__ */

/*! \} */

__END_DECLS

#endif /* __ZEDA_INDEX_H__ */
