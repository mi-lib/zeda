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

/* ********************************************************** */
/*! \defgroup index integer value array.
 * \{ *//* ************************************************** */

/*! \struct zIndex
 * \brief integer value array class
 *
 * zIndex is a container of an array of integer values.
 * The body structure is an instance of zIndexStruct.
 * \sa ZEDA_DEF_ARRAY_CLASS.
 *
 * To access elements of the integer array, use zIndexElem()
 * and zIndexSetElem().
 *
 * For frequent accesses to the head, neck (one-cell-before-head)
 * and tail, zIndexHead(), zIndexNeck() and zIndexTail()
 * are available.
 */
#ifdef __cplusplus
ZEDA_DEF_ARRAY_CLASS( _zIndexStruct, int );
struct zIndexStruct;
typedef zIndexStruct* zIndex;
struct zIndexStruct : public _zIndexStruct {
  zIndexStruct(){}
  zIndexStruct(int _size);
  ~zIndexStruct(){}
  int &operator[](int pos);
  void assignArray(int _size, int *_buf);
  zIndex setList(...);
  zIndexStruct &operator=(const zIndexStruct &src);
  zIndex clone();
  zIndex zero();
  zIndex order(int s);
  bool operator==(const zIndexStruct &anotherindex);
  bool operator!=(const zIndexStruct &anotherindex);
  bool swap(int p1, int p2);
  int move(int from, int to);
  zIndex remove(int i);
  bool removeVal(int val);
  int insertVal(int val);
  void sort();
  zIndex shuffle(int count);
  friend std::ostream &operator<<(std::ostream &stream, const zIndex index);
};
#else
ZEDA_DEF_ARRAY_CLASS( zIndexStruct, int );
typedef zIndexStruct* zIndex;
#endif /* __cplusplus */

__BEGIN_DECLS

/*! \brief capacity of an integer value array. */
#define zIndexCapacity(i)        zArrayCapacity(i)

/*! \brief size of an integer value array.
 * \retval the size of the array if \a i is not null.
 * \retval 0 if \a i is the null pointer.
 */
#define zIndexSizeNC(i)          zArraySize(i)
#define zIndexSize(i)            ( (i) ? zIndexSizeNC(i) : 0 )

/*! \brief set size of an array of integer values. */
#define zIndexSetSize(i,size)    zArraySetSize( i, size )
/*! \brief reset size of an array of integer values to its capacity. */
#define zIndexResetSize(i)       zArrayResetSize( i )
/*! \brief resize an array of integer values. */
#define zIndexResize(index,size) zArrayResize( index, size )

/*! \brief increment size of an index vector. */
#define zIndexIncSize(i)         ( zIndexSizeNC(i)++ )
/*! \brief decrement size of an index vector. */
#define zIndexDecSize(i)         ( zIndexSizeNC(i) -= ( zIndexSizeNC(i) > 0 ? 1 : 0 ) )

/*! \brief check if the specified position is valid for an index vector. */
#define zIndexPosIsValid(i,n)    zArrayPosIsValid( i, n )

#define zIndexBufNC(i)           zArrayBuf(i)
#define zIndexBuf(i)             ( (i) ? zIndexBufNC(i) : NULL )

/*! \brief assign a buffer of integer values to an index vector. */
#define zIndexAssignArray(index,size,buf) zArrayAssign( index, size, buf )

#define zIndexElemNC(i,n)        zIndexBufNC(i)[n]
#define zIndexElem(i,n)          ( zIndexPosIsValid(i,n) ? zIndexElemNC(i,n) : -1 )
#define zIndexSetElemNC(i,n,e)   ( zIndexElemNC(i,n) = (e) )
#define zIndexSetElem(i,n,e)     ( zIndexPosIsValid(i,n) ? zIndexSetElemNC(i,n,e) : -1 )

#define zIndexHead(index)        ( *zArrayHead(index) )
#define zIndexNeck(index)        ( *zArrayNeck(index) )
#define zIndexTail(index)        ( *zArrayTail(index) )

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
__ZEDA_EXPORT zIndex zIndexCreateList(int num, ...);
#endif /* __KERNEL__ */

/*! \brief set elements of an integer vector from argument list.
 *
 * zIndexSetElemList() sets elements of an array of integer values \a index from a given list of arguments.
 * \retval \a index
 * \note
 * zIndexSetElemList() is not available in the kernel space.
 */
#ifndef __KERNEL__
__ZEDA_EXPORT zIndex zIndexSetElemVList(zIndex index, va_list args);
__ZEDA_EXPORT zIndex zIndexSetElemList(zIndex index, ...);
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
__ZEDA_EXPORT zIndex zIndexCopyNC(const zIndex src, zIndex dest);
__ZEDA_EXPORT zIndex zIndexCopy(const zIndex src, zIndex dest);

/*! \brief clone an integer vector.
 *
 * zIndexClone() clones an integer vector \a src, namely, it allocates an integer
 * vector of the same size with \a src, and copies \a src to it.
 * \return
 * zIndexClone() returns a pointer to the cloned integer vector if it succeeds.
 * Otherwise, it returns the null pointer.
 */
__ZEDA_EXPORT zIndex zIndexClone(const zIndex src);

/*! \brief free an array of integer values.
 *
 * zIndexFree() frees the internal array of a zIndex instance \a index and itself as well.
 */
__ZEDA_EXPORT void zIndexFree(zIndex index);

/*! \brief zero an array of integer values.
 *
 * zIndexZero() sets all components of an array of index values
 * \a idex for zeros.
 * \retval a pointer \a index.
 */
__ZEDA_EXPORT zIndex zIndexZero(zIndex index);

/*! \brief order an array of integer values.
 *
 * zIndexOrder() sets all components of \a index in order, beginning from \a s as
 * { \a s, \a s+1, \a s+2, ... }.
 * \retval a pointer \a index.
 */
__ZEDA_EXPORT zIndex zIndexOrder(zIndex index, int s);

/*! \brief check if two arrays of integer values are equal.
 *
 * zIndexEqual() checks if two arrays of integer values \a index1 and \a index2 are equal.
 * \return
 * zIndexEqual() returns the true value if \a index1 and \a index2 are equal, or the false values, otherwise.
 */
__ZEDA_EXPORT bool zIndexEqual(const zIndex index1, const zIndex index2);

/*! \brief swap two elements of an array of integer values.
 *
 * zIndexSwap() swaps two components of an array of integer values \a index at positions of \a p1 and \a p2
 * without checking validity of the specified positions.
 *
 * zIndexSwap() swaps two components of an array of integer values \a index at positions of \a p1 and \a p2.
 * It checks if both \a p1 and \a p2 are valid for \a index before operation.
 * \return
 * zIndexSwap() returns the true value if it succeeds ot swap the components.
 * If either \a p1 or \a p2 is out of range of \a idex, it returns the false value.
 */
__ZEDA_EXPORT void zIndexSwapNC(zIndex index, int p1, int p2);
__ZEDA_EXPORT bool zIndexSwap(zIndex index, int p1, int p2);

/*! \brief move an element of an array of integer values.
 *
 * zIndexMove() moves a component at \a from to another position \a to of an array of integer values \a index.
 * The components between \a from and \a to are slided to make a space to insert the component.
 * \return
 * zIndexMove() returns the newly replaced value at \a to.
 * \note
 * if \a from or \a to is invalid, anything might happen.
 */
__ZEDA_EXPORT int zIndexMove(zIndex index, int from, int to);

/*! \brief remove a component from an integer vector.
 *
 * zIndexRemove() removes the \a i th component from an integer vector \a index. All the posterior
 * components are shifted forward, and the size of \a index is decreased. Note that the memory allocated
 * for \a index (referable by zIndexCapacity()) is remained, and only the value of the size is modified.
 * \return
 * zIndexRemove() returns a pointer \a index.
 */
__ZEDA_EXPORT zIndex zIndexRemove(zIndex index, int i);

/*! \brief remove an integer value from an integer vector.
 *
 * zIndexRemoveVal() removes an integer value \a val from an integer vector \a index. Namely, if \a val
 * is a member of \a index, \a val is removed from \a index, and decrements the size of \a index.
 * Note that the memory allocated for \a index (referable by zIndexCapacity()) is remained.
 * \return
 * zIndexRemoveVal() returns the true value if \a val is a member of \a index. Otherwise, it returns
 * the false value.
 */
__ZEDA_EXPORT bool zIndexRemoveVal(zIndex index, int val);

/*! \brief insert an integer value to an integer vector.
 *
 * zIndexInsertVal() inserts an integer value \a val to an integer vector \a index. The position of the
 * newly inserted element is before the first value that is larger than or equal to \a val. The size of
 * \a index is incremented as the result.
 * If the size of \a index is already equal to its capacity, nothing happens.
 * \return
 * zIndexInsertVal() returns the index of the inserted element.
 * If the buffer of \a index is already occupied, i.e., the size of \a index is equal to its capacity,
 * it returns -1.
 */
__ZEDA_EXPORT int zIndexInsertVal(zIndex index, int val);

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
 * zIndexFScan() scans a sequence of integer values from the current position of a file \a fp, and create
 * a new zIndex instance.
 * The format to be scanned by zIndexFScan() is as follows:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexScan() is also available to scan values from the standard input.
 * \return a pointer to the newly allocated zIndex instance.
 * \sa zIndexScan, zIndexFPrint
 */
__ZEDA_EXPORT zIndex zIndexFScan(FILE *fp);
#define zIndexScan() zIndexFScan( stdin )

/*! \brief print an array of integer values to a file.
 *
 * zIndexFPrint() prints an array of integer values in a zIndex instance \a index out to the current
 * position of a file \a fp in the following format:
 *
 *  \a n ( x1 x2 x3 ... xn )
 *
 * where \a n is the number of values.
 *
 * zIndexPrint() is also available to print values out to the standard output.
 * \sa zIndexFScan, zIndexPrint, zIndexDataFPrint
 */
__ZEDA_EXPORT void zIndexFPrint(FILE *fp, const zIndex index);
#define zIndexPrint(i) zIndexFPrint( stdout, i )
/*! \brief print an array of integer values out to a file.
 *
 * zIndexDataFPrint() prints an array of integer values in a zIndex instance \a index out to the current
 * position of a file \a fp in the following format:
 *
 *  x1 x2 x3 ... xn
 *
 * zIndexDataPrint() is also available to print values out to the standard output.
 * \sa zIndexFScan, zIndexFPrint, zIndexDataPrint
 */
__ZEDA_EXPORT void zIndexDataFPrint(FILE *fp, const zIndex index);
#define zIndexDataPrint(i) zIndexDataFPrint( stdout, i )
#else
void zIndexPrint(const zIndex index);
#endif /* __KERNEL__ */

#include <zeda/zeda_list.h>

/*! \brief create an integer vector from a list of integers. */
__ZEDA_EXPORT zIndex zIndexCreateFromList(zIntList *list);

/*! \} */

__END_DECLS

#ifdef __cplusplus
inline zIndexStruct::zIndexStruct(int _size) : _zIndexStruct( _size ) { zIndexOrder( this, 0 ); }
inline int &zIndexStruct::operator[](int pos){ return zIndexElemNC( this, pos ); }
inline void zIndexStruct::assignArray(int _size, int *_buf){ zIndexAssignArray( this, _size, _buf ); }
inline zIndex zIndexStruct::setList(...){
  va_list args;
  va_start( args, this );
  zIndexSetElemVList( this, args );
  va_end( args );
  return this;
}
inline zIndexStruct &zIndexStruct::operator=(const zIndexStruct &src){ return *zIndexCopy( const_cast<zIndex>(&src), this ); }
inline zIndex zIndexStruct::clone(){ return zIndexClone( this ); }
inline zIndex zIndexStruct::zero(){ return zIndexZero( this ); }
inline zIndex zIndexStruct::order(int s = 0){ return zIndexOrder( this, s ); }
inline bool zIndexStruct::operator==(const zIndexStruct &anotherindex){ return zIndexEqual( this, const_cast<zIndex>(&anotherindex) ); }
inline bool zIndexStruct::operator!=(const zIndexStruct &anotherindex){ return !zIndexEqual( this, const_cast<zIndex>(&anotherindex) ); }
inline bool zIndexStruct::swap(int p1, int p2){ return zIndexSwap( this, p1, p2 ); }
inline int zIndexStruct::move(int from, int to){ return zIndexMove( this, from, to ); }
inline zIndex zIndexStruct::remove(int i){ return zIndexRemove( this, i ); }
inline bool zIndexStruct::removeVal(int val){ return zIndexRemoveVal( this, val ); }
inline int zIndexStruct::insertVal(int val){ return zIndexInsertVal( this, val ); }
inline void zIndexStruct::sort(){ zIndexSort( this ); }
inline zIndex zIndexStruct::shuffle(int count){ return zIndexShuffle( this, count ); }
inline std::ostream &operator<<(std::ostream &stream, const zIndex index){
  stream << index->size << " ( " << (*index)[0];
  for(int i=1; i<index->size; i++ ) stream << ", " << (*index)[i];
  stream << " )" << std::endl;
  return stream;
}
#endif /* __cplusplus */

#endif /* __ZEDA_INDEX_H__ */
