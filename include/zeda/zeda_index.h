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

/*! \brief increment size of an index vector. */
#define zIndexIncSize(i)     ( zArraySize(i)++ )
/*! \brief decrement size of an index vector. */
#define zIndexDecSize(i)     ( zArraySize(i) -= ( zArraySize(i) > 0 ? 1 : 0 ) )

/*! \brief check if the specified position is valid for an index vector. */
#define zIndexPosIsValid(i,n) zArrayPosIsValid( i, n )

#define zIndexElemNC(i,n)      zArrayBuf(i)[n]
#define zIndexElem(i,n)        ( zIndexPosIsValid(i,n) ? zIndexElemNC(i,n) : -1 )
#define zIndexSetElemNC(i,n,e) ( zIndexElemNC(i,n) = (e) )
#define zIndexSetElem(i,n,e)   ( zIndexPosIsValid(i,n) ? zIndexSetElemNC(i,n,e) : -1 )

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

/*! \brief zero an array of integer values.
 *
 * zIndexZero() sets all components of an array of index values
 * \a idex for zeros.
 * \retval a pointer \a idx.
 */
__EXPORT zIndex zIndexZero(zIndex idx);

/*! \brief order an array of integer values.
 *
 * zIndexOrder() sets all components of \a idx in order,
 * beginning from \a s as { \a s, \a s+1, \a s+2, ... }.
 * \retval a pointer \a idx.
 */
__EXPORT zIndex zIndexOrder(zIndex idx, int s);

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
/*! \brief scan an array of integer values from a file.
 *
 * zIndexFScan() scans a sequence of integer values from
 * the current position of a file \a fp, and create a new
 * zIndex instance.
 * The format to be scanned by zIndexFScan() is as follows:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexScan() is also available to scan values from the
 * standard input.
 * \return a pointer to the newly allocated zIndex instance.
 * \sa zIndexScan, zIndexFPrint
 */
__EXPORT zIndex zIndexFScan(FILE *fp);
#define zIndexScan() zIndexFScan( stdin )

/*! \brief print an array of integer values to a file.
 *
 * zIndexFPrint() prints an array of integer values in
 * a zIndex instance \a idx out to the current position
 * of a file \a fp in the following format:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexPrint() is also available to print values out to
 * the standard output.
 * \sa zIndexFScan, zIndexPrint, zIndexDataFPrint
 */
__EXPORT void zIndexFPrint(FILE *fp, zIndex idx);
#define zIndexPrint(i) zIndexFPrint( stdout, i )
/*! \brief print an array of integer values out to a file.
 *
 * zIndexDataFPrint() prints an array of integer values in
 * a zIndex instance \a idx out to the current position of
 * a file \a fp in the following format:
 *
 *  x1 x2 x3 ... xn
 *
 * zIndexDataPrint() is also available to print values out
 * to the standard output.
 * \sa zIndexFScan, zIndexFPrint, zIndexDataPrint
 */
__EXPORT void zIndexDataFPrint(FILE *fp, zIndex idx);
#define zIndexDataPrint(i) zIndexDataFPrint( stdout, i )
#else
void zIndexPrint(zIndex idx);
#endif /* __KERNEL__ */

/*! \} */

__END_DECLS

#endif /* __ZEDA_INDEX_H__ */
