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

/*! \brief size of an integer value array.
 * \retval the size of the array if \a i is not null.
 * \retval 0 if \a i is the null pointer.
 */
#define zIndexSizeNC(i)      zArraySize(i)
#define zIndexSize(i)        ( (i) ? zIndexSizeNC(i) : 0 )

/*! \brief increment size of an index vector. */
#define zIndexIncSize(i)     ( zIndexSizeNC(i)++ )
/*! \brief decrement size of an index vector. */
#define zIndexDecSize(i)     ( zIndexSizeNC(i) -= ( zIndexSizeNC(i) > 0 ? 1 : 0 ) )

/*! \brief check if the specified position is valid for an index vector. */
#define zIndexPosIsValid(i,n) zArrayPosIsValid( i, n )

#define zIndexBufNC(i)         zArrayBuf(i)
#define zIndexBuf(i)           ( (i) ? zIndexBufNC(i) : NULL )

#define zIndexElemNC(i,n)      zIndexBufNC(i)[n]
#define zIndexElem(i,n)        ( zIndexPosIsValid(i,n) ? zIndexElemNC(i,n) : -1 )
#define zIndexSetElemNC(i,n,e) ( zIndexElemNC(i,n) = (e) )
#define zIndexSetElem(i,n,e)   ( zIndexPosIsValid(i,n) ? zIndexSetElemNC(i,n,e) : -1 )

#define zIndexHead(idx)      ( *zArrayHead(idx) )
#define zIndexNeck(idx)      ( *zArrayNeck(idx) )
#define zIndexTail(idx)      ( *zArrayTail(idx) )

/*! \brief allocate an array of integer values.
 *
 * zIndexAlloc() allocates a vector of integer values with a size
 * \a size.
 * \return
 * zIndexAlloc() returns a pointer to the allocated memory if
 * it succeeds. Otherwise, the null pointer is returned.
 */
__ZEDA_EXPORT zIndex zIndexAlloc(int size);

/*! \brief create an array of integer values.
 *
 * zIndexCreate() creates an array of integer values with a size
 * \a size. It initializes all values of the array as
 * { 0, 1, 2, ..., \a size -1 }.
 * \return
 * zIndexCreate() returns a pointer to the allocated array if it
 * succeeds. Otherwise, the null pointer is returned.
 */
__ZEDA_EXPORT zIndex zIndexCreate(int size);

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

#define _zIndexCopyNC(src,dest) memcpy( zIndexBufNC(dest), zIndexBufNC(src), sizeof(int)*zIndexSizeNC(dest) )

/*! \brief copy an integer vector to another.
 *
 * zIndexCopyNC() and zIndexCopy() copy an integer vector \a src to another
 * vector \a dest. The latter checks if the sizes of the two vectors are
 * the same, while the former does not.
 * \return
 * zIndexCopyNC() returns a pointer \a dest. If the size of \a src is smaller
 * than that of \a dest, anything might happen.
 *
 * zIndexCopy() returns a pointer \a dest if it succeeds. If the sizes of
 * \a src and \a dest are different, it returns the false value.
 */
__ZEDA_EXPORT zIndex zIndexCopyNC(zIndex src, zIndex dest);
__ZEDA_EXPORT zIndex zIndexCopy(zIndex src, zIndex dest);

/*! \brief clone an integer vector.
 *
 * zIndexClone() clones an integer vector \a src, namely, it allocates an integer
 * vector of the same size with \a src, and copies \a src to it.
 * \return
 * zIndexClone() returns a pointer to the cloned integer vector if it succeeds.
 * Otherwise, it returns the null pointer.
 */
__ZEDA_EXPORT zIndex zIndexClone(zIndex src);

/*! \brief free an array of integer values.
 *
 * zIndexFree() frees a zIndex instance \a idx,
 * freeing the assigned memory space.
 */
__ZEDA_EXPORT void zIndexFree(zIndex idx);

/*! \brief zero an array of integer values.
 *
 * zIndexZero() sets all components of an array of index values
 * \a idex for zeros.
 * \retval a pointer \a idx.
 */
__ZEDA_EXPORT zIndex zIndexZero(zIndex idx);

/*! \brief order an array of integer values.
 *
 * zIndexOrder() sets all components of \a idx in order,
 * beginning from \a s as { \a s, \a s+1, \a s+2, ... }.
 * \retval a pointer \a idx.
 */
__ZEDA_EXPORT zIndex zIndexOrder(zIndex idx, int s);

/*! \brief check if two arrays of integer values are equal.
 *
 * zIndexEqual() checks if two arrays of integer values
 * \a idx1 and \a idx2 are equal.
 * \return the true value if \a idx1 and \a idx2 are equal,
 * or the false values, otherwise.
 */
__ZEDA_EXPORT bool zIndexEqual(zIndex idx1, zIndex idx2);

/*! \brief swap two elements of an array of integer values.
 *
 * zIndexSwap() swaps two components of a zIndex instance
 * \a idx at the positions of \a p1 and \a p2.
 * \return a newly swapped value at \a p2.
 * \note
 * if \a p1 or \a p2 is invalid, anything might happen.
 */
__ZEDA_EXPORT int zIndexSwap(zIndex idx, int p1, int p2);

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
__ZEDA_EXPORT int zIndexMove(zIndex idx, int from, int to);

/*! \brief remove a component from an integer vector.
 *
 * zIndexRemove() removes the \a i th component from an integer vector
 * \a idx. All the posterior components are shifted forward, and the
 * size of \a idx is decreased. Note that the memory allocated for
 * \a idx is remained, and only the value of the size is modified.
 * \return a pointer \a idx.
 */
__ZEDA_EXPORT zIndex zIndexRemove(zIndex idx, int i);

/*! \brief remove an integer value from an integer vector.
 *
 * zIndexRemoveVal() removes an integer value \a val from an integer vector
 * \a index. Namely, if \a val is a member of \a index, \a val is removed
 * from \a index and decrements the size of \a index.
 * \return
 * zIndexRemoveVal() returns the true value if \a val is a member of
 * \a index. Otherwise, it returns the false value.
 */
__ZEDA_EXPORT bool zIndexRemoveVal(zIndex index, int val);

/*! \brief insert an integer value to an integer vector.
 *
 * zIndexInsertVal() inserts an integer value \a val to an integer vector
 * \a index. The position of the newly inserted element is before the first
 * value that is larger than or equal to \a val. The size of \a index is
 * incremented as the result.
 * \a maxsize is the maximum size of buffer of \a index, which has to be
 * larger than or equal to the size of \a index.
 * \return
 * zIndexInsertVal() returns the index of the inserted element.
 * If the buffer of \a index is already occupied, i.e., \a maxsize is equal
 * to the size of \a index, -1 is returned.
 * \note
 * zIndexInsertVal() itself does not check the true size of the originally
 * allocated array. If a wrong number is given for \a maxsize, anything might
 * happen. Thus, this function should not be used unless the safety is
 * guaranteed by some means.
 */
__ZEDA_EXPORT int zIndexInsertVal(zIndex index, int maxsize, int val);

/*! \brief sort an integer vector in ascending order.
 */
__ZEDA_EXPORT void zIndexSort(zIndex index);

/*! \brief randomly shuffle components of an integer vector.
 *
 * zIndexShuffle() randomly shuffles components of an integer vector \a index.
 * \a count is the number of shuffles. If 0 is given for it, it is replaced is three times of the size
 * of \a index as the default number.
 * \return
 * zIndexShuffle() always returns the pointer \a index.
 */
__ZEDA_EXPORT zIndex zIndexShuffle(zIndex index, int count);

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
__ZEDA_EXPORT zIndex zIndexFScan(FILE *fp);
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
__ZEDA_EXPORT void zIndexFPrint(FILE *fp, zIndex idx);
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
__ZEDA_EXPORT void zIndexDataFPrint(FILE *fp, zIndex idx);
#define zIndexDataPrint(i) zIndexDataFPrint( stdout, i )
#else
void zIndexPrint(zIndex idx);
#endif /* __KERNEL__ */

#include <zeda/zeda_list.h>

/*! \brief create an integer vector from a list of integers. */
__ZEDA_EXPORT zIndex zIndexCreateFromList(zIntList *list);

/*! \} */

__END_DECLS

#endif /* __ZEDA_INDEX_H__ */
