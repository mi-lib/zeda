/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_array.h
 * \brief array operation.
 * \author Zhidao
 */

#ifndef __ZEDA_ARRAY_H__
#define __ZEDA_ARRAY_H__

#include <zeda/zeda_misc.h>
#include <zeda/zeda_name.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup array universal array operations.
 * \{ *//* ************************************************** */

/*! \def zArrayClass(array_t,cell_t)
 * \brief generate array class.
 *
 * A macro zArrayClass() can generate a new array class, which
 * consists of the size of the array and the pointer to the array
 * buffer.
 * \a array_t is the class name to be defined.
 * \a cell_t is the class name of the data to be arrayed.
 *
 * The size of the array is acquired by zArraySize().
 * The array head can be accessed by zArrayBuf().
 * Each element can be accessed by zArrayElem().
 * For a faster access to each element, zArrayElemNC() is also
 * available, which does not check the specified location.
 *
 * The array can be freed by calling zArrayFree().
 *
 * zArraySetElem() is available in order to set each element, which
 * works only if the specified location is valid. zArraySetElemNC()
 * is also available, which provides a faster access to each
 * element without checking the validity of specified location.
 * For frequent accesses to the head, neck (one-cell-before-head)
 * and tail, zArrayHead(), zArrayNeck() and zArrayTail() are
 * available.
 */
#ifdef __cplusplus
#define zArrayClass(array_t,cell_t) \
struct array_t{ \
  int size; \
  cell_t *buf; \
  array_t() : size{0}, buf{NULL} {} \
  array_t(int _size){ zArrayAlloc( this, cell_t, _size ); } \
  bool isValidPos(int pos){ return zArrayPosIsValid( this, pos ); } \
  cell_t *getNC(int i){ return zArrayElemNC( this, i ); } \
  cell_t *get(int i){ return zArrayElem( this, i ); } \
  cell_t *head(){ return zArrayHead( this ); } \
  cell_t *neck(){ return zArrayNeck( this ); } \
  cell_t *tail(){ return zArrayTail( this ); } \
  cell_t *operator[](int i){ return get( i ); } \
  void assign(cell_t *_buf, int _size){ zArrayAssign( this, _buf, _size ); } \
  array_t *init(){ zArrayInit( this ); return this; } \
  array_t *alloc(int _size){ \
    zArrayAlloc( this, cell_t, _size ); \
    return this; \
  } \
  void _free(){ zArrayFree( this ); } \
  void move(array_t *src){ zArrayMove( src, this ); } \
  void sort(int (* cmp)(void*,void*,void*), void *util){ zArrayQuickSort( this, cmp, util ); } \
}
#else
#define zArrayClass(array_t,cell_t) \
typedef struct{\
  int size;\
  cell_t *buf;\
} array_t
#endif /* __cplusplus */

#define zArraySize(array)          (array)->size
#define zArrayBuf(array)           (array)->buf

#define zArrayPosIsValid(array,p)  ( (p) < zArraySize(array) && (p) >= 0 )

/* NOTE: do not use the following macro before allocating buffer */
#define zArrayElemSize(array)      sizeof(*zArrayBuf(array))

#define zArrayElemNC(array,i)      ( &zArrayBuf(array)[i] )
#define zArrayElem(array,i)        ( zArrayPosIsValid(array,i) ? zArrayElemNC(array,i) : NULL )
#define zArraySetElemNC(array,i,d) memcpy( zArrayElemNC(array,i), (d), zArrayElemSize(array) )
#define zArraySetElem(array,i,d)   ( zArrayPosIsValid(array,i) ? zArraySetElemNC(array,i,d) : NULL )

#define zArrayHead(array)          zArrayElemNC( array, zArraySize(array)-1 )
#define zArrayNeck(array)          zArrayElemNC( array, zArraySize(array)-2 )
#define zArrayTail(array)          zArrayElemNC( array, 0 )

/*! \brief assign a buffer and its size to an array. */
#define zArrayAssign(array,buf,size) do{\
  zArraySize(array) = (size);\
  zArrayBuf(array) = (buf);\
} while(0)

#define zArrayInit(array) zArrayAssign( array, NULL, 0 )

/*! \brief allocate an array.
 * \param arr array class instance to be allocated.
 * \param type the data type of the array cells.
 * \param n the number of cells.
 */
#define zArrayAlloc(array,type,n) do{\
  if( (n) <= 0 ) zArrayInit( array );\
  else{\
    if( !( zArrayBuf(array) = zAlloc( type, n ) ) ){\
      ZALLOCERROR();\
      zArraySize(array) = 0;\
    } else\
      zArraySize(array) = (n);\
  }\
} while(0)

/*! \brief clone an array.
 * zArrayClone() clones an array \a src to another array \a dest.
 */
#define zArrayClone(src,dest) do{ \
  zArrayInit( dest ); \
  if( ( zArrayBuf(dest) = zCloneMem( zArrayBuf(src), zArraySize(src) * zArrayElemSize(src) ) ) ){ \
    zArraySize(dest) = zArraySize(src); \
  } else{ \
    ZALLOCERROR(); \
  } \
} while(0)

/*! \brief free an array.
 * \param arr a pointer to the array to be freed.
 */
#define zArrayFree(array) do{\
  free( zArrayBuf(array) ); \
  zArrayInit( array ); \
} while(0)

/*! zArrayFindName() is valid for an array of a named class.
 * \sa zNameFind.
 */
#define zArrayFindName(array,name,ptr) \
  zNameFind( zArrayBuf(array), zArraySize(array), name, ptr )

#ifndef __KERNEL__
/* since realloc() is not implemented in kernel space,
 * the following methods are only available in user space.
 */

/*! \brief add a new cell to an array.
 *
 * zArrayAdd() adds a new cell pointed by \a dat to an array
 * \a array at the last, incrementing the size of \a array.
 * \a type is the data type of the cell.
 */
#define zArrayAdd(array,type,dat) do{\
  type *__zarray_ap;\
  __zarray_ap = zRealloc( zArrayBuf(array), type, zArraySize(array)+1 );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArraySize(array)++;\
    zArrayBuf(array) = __zarray_ap;\
    zArraySetElemNC( array, zArraySize(array)-1, dat );\
  }\
} while(0)

/*! \brief insert a new cell into an array.
 *
 * zArrayInsert() inserts a new cell pointed by \a dat to in an
 * array \a array at the location specified by \a pos, incrementing
 * the size of \a array. \a type is the data type of the cell.
 */
#define zArrayInsert(array,type,pos,dat) do{\
  type *__zarray_ap;\
  if( (pos) == zArraySize(array) ){\
    zArrayAdd( array, type, dat );\
  } else if( zArrayPosIsValid(array,pos) ){\
    __zarray_ap = zRealloc( zArrayBuf(array), type, zArraySize(array)+1 );\
    if( __zarray_ap == NULL )\
      ZALLOCERROR();\
    else{\
      zArrayBuf(array) = __zarray_ap;\
      memmove( zArrayElemNC(array,(pos)+1), zArrayElemNC(array,pos), sizeof(type)*(zArraySize(array)-(pos)) );\
      zArraySetElemNC( array, pos, dat );\
      zArraySize(array)++;\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArraySize(array)-1 );\
  }\
} while(0)

/*! \brief delete a cell from an array.
 *
 * zArrayDelete() deletes a cell at the location specified by
 * \a pos in an array \a array at, decrementing the size of \a array.
 * \a type is the data type of the cell.
 */
#define zArrayDelete(array,type,pos) do{\
  type *__zarray_ap;\
  if( zArrayPosIsValid(array,pos) ){\
    if( (pos) < zArraySize(array)-1 )\
      memmove( zArrayElemNC(array,pos), zArrayElemNC(array,(pos)+1), sizeof(type)*(zArraySize(array)-(pos)-1) );\
    if( zArraySize(array) > 1 ){\
      __zarray_ap = zRealloc( zArrayBuf(array), type, zArraySize(array)-1 );\
      if( __zarray_ap == NULL )\
        ZALLOCERROR();\
      else{\
        zArraySize(array)--;\
        zArrayBuf(array) = __zarray_ap;\
      }\
    } else{\
      zArrayFree( array );\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArraySize(array)-1 );\
  }\
} while(0)

/*! \brief append an array to another.
 *
 * zArrayAppend() appends an array pointed by \a subarray
 * to another array pointed by \a array.
 * \a type is the data type of the cell.
 */
#define zArrayAppend(array,subarray,type) do{\
  type *__zarray_ap;\
  __zarray_ap = zRealloc( zArrayBuf(array), type, zArraySize(array) + zArraySize(subarray) );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArrayBuf(array) = __zarray_ap;\
    memcpy( zArrayElemNC(array,zArraySize(array)), zArrayBuf(subarray), zArraySize(subarray)*sizeof(type) );\
    zArraySize(array) += zArraySize(subarray);\
  }\
} while(0)

/*! \brief move an array to another.
 *
 * zArrayMove() moves an array pointed by \a src to another \a dest.
 * As the result, \a src will be emptied.
 */
#define zArrayMove(src,dest) do{\
  zArrayFree( dest );\
  zArraySize(dest) = zArraySize(src);\
  zArrayBuf(dest) = zArrayBuf(src);\
  zArrayInit(src);\
} while(0)

#endif /* __KERNEL__ */

/*! \brief partition an array of pointers.
 *
 * zQuickPartition() partitions an array pointed by \a array based on the \a pivot_id th element of
 * \a array as a pivot. \a nmemb is the number of components of \a array, and \a size is the size of
 * an individual component.
 * The components of \a array are partitioned according to the comparison function \a cmp, namely,
 * all elements that make cmp(a,b,priv) > 0 are put in the former half.
 * \a util is for programmer's utility.
 * \return
 * zQuickPartition() returns the index of the head of the latter-half partition of \a array.
 */
__ZEDA_EXPORT int zQuickPartition(void *array, int nmemb, int size, int (* cmp)(void*,void*,void*), void *priv, int pivot_id);

/*! \brief quick sort for a pointer array.
 *
 * zQuickSort() sorts an array pointed by \a array by the quick sort algorithm.
 * \a nmemb is the number of components of \a array, and \a size is the size of an individual component.
 * The components of \a array are sorted in ascending order according to the comparison function \a cmp,
 * namely, a factor 'a' in the \a array is put after another factor 'b' if cmp(a,b,priv) > 0, where
 * \a util is for programmer's utility.
 * \return
 * zQuickSort() returns no value.
 */
__ZEDA_EXPORT void zQuickSort(void *array, int nmemb, int size, int (*cmp)(void*,void*,void*), void *util);

/*! \brief quick sort for an array.
 *
 * zArrayQuickSort() is a macro which is a wrapper of zQuickSort() that sorts
 * an array pointed by \a array based on the quick sort algorithm. \a cmp and
 * \a util have the same roles with those for zQuickSort().
 * \sa
 * zQuickSort()
 */
#define zArrayQuickSort(arr,cmp,util) zQuickSort( (void*)zArrayBuf(arr), zArraySize(arr), zArrayElemSize(arr), cmp, util )

/*! \brief insert a member into a pointer array at sorted position.
 *
 * zInsertSort() inserts a new member \a memb into an array pointed by
 * \a array in a sorted way. \a nmemb is the number of components of
 * \a array, and \a size is the size of each component. It supposes
 * that the array is occupied up to the \a i -1 th component.
 * 
 * The components of \a array will be sorted in ascending order according
 * to the comparison function \a cmp. Namely, a factor 'a' in the \a array
 * is put after another factor 'b' if cmp(a,b,util) > 0, where \a util is
 * for programmer's utility.
 * \return
 * zInsertSort() returns a pointer \a memb in the case of success, or the
 * null pointer if \a i is larger than or equal to \a size.
 */
__ZEDA_EXPORT void *zInsertSort(void *array, void *memb, int i, int nmemb, int size, int (* cmp)(void*,void*,void*), void *util);

/*! \brief insert a member into an array at sorted position.
 *
 * zArrayInsertSort() is a macro which is a wrapper of zInsertSort() that
 * inserts a new member \a memb into an array pointed by \a array in a
 * sorted way. \a cmp and \a util have the same roles with those for
 * zInsertSort().
 * \sa
 * zInsertSort()
 */
#define zArrayInsertSort(array,memb,i,cmp,util) zInsertSort( (void*)zArrayBuf(array), (memb), (i), zArraySize(array), zArrayElemSize(array), cmp, util )

/*! \brief select an element of an array.
 *
 * zQuickSelect() selects the \a order th element of an array \a array.
 * \a nmemb is the number of the array.
 * \a size is the size of an element of the array.
 * \a cmp is a comparator function, which accepts a user-defined private data \a priv.
 * \note
 * The order of \a array is modified in this process.
 * \return
 * zQuickSelect() returns the pointer to the selected element.
 */
__ZEDA_EXPORT void *zQuickSelect(void *array, int nmemb, int size, int order, int (* cmp)(void*,void*,void*), void *priv);

/*! \brief median of an array. */
#define zQuickMedian(array,nmemb,size,cmp,priv) zQuickSelect( array, nmemb, size, zArraySize(array)/2, cmp, priv )

/*! \brief select an element of an array. */
#define zArraySelect(array,order,cmp,priv)      zQuickSelect( (void*)zArrayBuf(array), zArraySize(array), zArrayElemSize(array), order, cmp, priv )

/*! \brief select the median of an array. */
#define zArrayMedian(array,cmp,priv)            zArraySelect( array, zArraySize(array)/2, cmp, priv )

/*! \} */

/* ********************************************************** */
/*! \defgroup array2 universal two-dimensional array operations.
 * \{ *//* ************************************************** */

/*! \def zArray2Class(array_t,cell_t)
 * \brief generate two-dimensional array class.
 *
 * A macro zArray2Class() can generate a new two-dimensional array
 * class, which consists of the row and column sizes of the array
 * and the pointer to the array buffer.
 * \a array_t is the class name to be defined.
 * \a cell_t is the class name of the data to be arrayed.
 *
 * The row and column sizes of the array are acquired by zArray2Size().
 * zArray2RowSize() and zArray2ColSize() are also available.
 * Each element can be accessed by zArray2Elem().
 * For a faster access to each element, zArray2ElemNC() is also
 * available, which does not check the specified location.
 *
 * The array can be freed by calling zArray2Free().
 *
 * zArray2SetElem() is available in order to set each element, which
 * works only if the specified location is valid. zArray2SetElemNC()
 * is also available, which provides a faster access to each element
 * without checking the validity of specified location.
 */
#ifdef __cplusplus
#define zArray2Class(array_t,cell_t) \
struct array_t{ \
  int size[2]; \
  cell_t *buf; \
  array_t *init(){ zArray2Init( this ); return this; } \
  array_t *alloc(int rowsize, int colsize){ zArray2Alloc( this, cell_t, rowsize, colsize ); return this; } \
  void _free(){ zArray2Free( this ); } \
  array_t() : size{ 0, 0 }, buf{NULL} {} \
  array_t(int _rowsize, int _colsize){ alloc( _rowsize, _colsize ); } \
  int rowsize(){ return zArray2RowSize( this ); } \
  int colsize(){ return zArray2ColSize( this ); } \
  bool isValidPos(int row, int col){ return zArray2PosIsValid( this, row, col ); } \
  cell_t *getNC(int i, int j){ return zArray2ElemNC( this, i, j ); } \
  cell_t *get(int i, int j){ return zArray2Elem( this, i, j ); } \
  cell_t *operator[](int i){ return zArray2RowBuf( this, i ); } \
  void setNC(int i, int j, cell_t *elem){ zArray2SetElemNC( this, i, j, elem ); } \
  bool set(int i, int j, cell_t *elem){ return zArray2SetElem( this, i, j, elem ) ? true : false; } \
}
#else
#define zArray2Class(array_t,cell_t) \
typedef struct{ \
  int size[2]; \
  cell_t *buf; \
} array_t
#endif /* __cplusplus */

#define zArray2Size(array,i)   (array)->size[i]
#define zArray2RowSize(array)  (array)->size[0]
#define zArray2ColSize(array)  (array)->size[1]
#define zArray2Buf(array)      ( (array)->buf )
#define zArray2RowBuf(array,i) ( (i) < zArray2RowSize(array) ? &zArray2Buf(array)[(i)*zArray2ColSize(array)] : NULL )

#define zArray2PosIsValid(array,r,c) ( (r) < zArray2RowSize(array) && (r) >= 0 && (c) < zArray2ColSize(array) && (c) >= 0 )

/* NOTE: do not use the following macro before allocating buffer */
#define zArray2ElemSize(array)       sizeof(*zArray2Buf(array))

#define zArray2ElemNC(array,i,j)      ( &zArray2Buf(array)[(i)*zArray2ColSize(array) + (j)] )
#define zArray2Elem(array,i,j)        ( zArray2PosIsValid(array,i,j) ? zArray2ElemNC(array,i,j) : NULL )
#define zArray2SetElemNC(array,i,j,d) memcpy( zArray2ElemNC(array,i,j), (d), zArray2ElemSize(array) )
#define zArray2SetElem(array,i,j,d)   ( zArray2PosIsValid(array,i,j) ? zArray2SetElemNC(array,i,j,d) : NULL )

#define zArray2Init(array) do{\
  zArray2RowSize(array) = zArray2ColSize(array) = 0;\
  zArray2Buf(array) = NULL;\
} while(0)
/*! \brief allocate an array.
 * \param array array class instance to be allocated.
 * \param type the data type of the array cells.
 * \param n the number of cells.
 */
#define zArray2Alloc(array,type,r,c) do{\
  zArray2Init( array );\
  if( (r) > 0 && (c) > 0 && !( zArray2Buf(array) = zAlloc(type,(r)*(c)) ) ){\
    ZALLOCERROR();\
    zArray2RowSize(array) = 0;\
    zArray2ColSize(array) = 0;\
  } else{\
    zArray2RowSize(array) = (r);\
    zArray2ColSize(array) = (c);\
  }\
} while(0)

/*! \brief free an array.
 * \param array a pointer to the array to be freed.
 */
#define zArray2Free(array) do{\
  free( zArray2Buf(array) );\
  zArray2Init( array );\
} while(0)

/*! \} */

__END_DECLS

#endif /* __ZEDA_ARRAY_H__ */
