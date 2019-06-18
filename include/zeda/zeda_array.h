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
struct array_t{\
  uint size;\
  cell_t *buf;\
}
#else
#define zArrayClass(array_t,cell_t) \
typedef struct{\
  uint size;\
  cell_t *buf;\
} array_t
#endif /* __cplusplus */

#define zArraySize(a)          (a)->size
#define zArrayBuf(a)           (a)->buf

#define zArrayPosIsValid(a,p)  ( (p) < zArraySize(a) && (p) >= 0 )

/* NOTE: do not use the following macro before allocating buffer */
#define zArrayElemSize(a)      sizeof(*zArrayBuf(a))

#define zArrayElemNC(a,i)      ( &zArrayBuf(a)[i] )
#define zArrayElem(a,i)        ( zArrayPosIsValid(a,i) ? zArrayElemNC(a,i) : NULL )
#define zArraySetElemNC(a,i,d) memcpy( zArrayElemNC(a,i), (d), zArrayElemSize(a) )
#define zArraySetElem(a,i,d)   ( zArrayPosIsValid(a,i) ? zArraySetElemNC(a,i,d) : NULL )

#define zArrayHead(a)          zArrayElemNC( a, zArraySize(a)-1 )
#define zArrayNeck(a)          zArrayElemNC( a, zArraySize(a)-2 )
#define zArrayTail(a)          zArrayElemNC( a, 0 )

#define zArrayInit(arr) do{\
  zArraySize(arr) = 0;\
  zArrayBuf(arr) = NULL;\
} while(0)
/*! \brief allocate an array.
 * \param arr array class instance to be allocated.
 * \param type the data type of the array cells.
 * \param n the number of cells.
 */
#define zArrayAlloc(arr,type,n) do{\
  if( (n) <= 0 ) zArrayInit( arr );\
  else{\
    if( !( zArrayBuf(arr) = zAlloc( type, n ) ) ){\
      ZALLOCERROR();\
      zArraySize(arr) = 0;\
    } else\
      zArraySize(arr) = (n);\
  }\
} while(0)

/*! \brief free an array.
 * \param arr a pointer to the array to be freed.
 */
#define zArrayFree(arr) do{\
  free( zArrayBuf(arr) );\
  zArrayInit( arr );\
} while(0)

/*! zArrayFindName() is valid for an array of a named class.
 * \sa zNameFind.
 */
#define zArrayFindName(arr,name,ptr) \
  zNameFind( zArrayBuf(arr), zArraySize(arr), name, ptr )

#ifndef __KERNEL__
/* since realloc() is not implemented in kernel space,
 * the following methods are only available in user space.
 */

/*! \brief add a new cell to an array.
 *
 * zArrayAdd() adds a new cell pointed by \a dat to an array
 * \a arr at the last, incrementing the size of \a arr.
 * \a type is the data type of the cell.
 */
#define zArrayAdd(arr,type,dat) do{\
  type *__zarray_ap;\
  __zarray_ap = zRealloc( zArrayBuf(arr), type, zArraySize(arr)+1 );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArraySize(arr)++;\
    zArrayBuf(arr) = __zarray_ap;\
    zArraySetElemNC( arr, zArraySize(arr)-1, dat );\
  }\
} while(0)

/*! \brief insert a new cell into an array.
 *
 * zArrayInsert() inserts a new cell pointed by \a dat to in an
 * array \a arr at the location specified by \a pos, incrementing
 * the size of \a arr. \a type is the data type of the cell.
 */
#define zArrayInsert(arr,type,pos,dat) do{\
  type *__zarray_ap;\
  if( (pos) == zArraySize(arr) ){\
    zArrayAdd( arr, type, dat );\
  } else if( zArrayPosIsValid(arr,pos) ){\
    __zarray_ap = zRealloc( zArrayBuf(arr), type, zArraySize(arr)+1 );\
    if( __zarray_ap == NULL )\
      ZALLOCERROR();\
    else{\
      zArrayBuf(arr) = __zarray_ap;\
      memmove( zArrayElemNC(arr,(pos)+1), zArrayElemNC(arr,pos), sizeof(type)*(zArraySize(arr)-(pos)) );\
      zArraySetElemNC( arr, pos, dat );\
      zArraySize(arr)++;\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArraySize(arr)-1 );\
  }\
} while(0)

/*! \brief delete a cell from an array.
 *
 * zArrayDelete() deletes a cell at the location specified by
 * \a pos in an array \a arr at, decrementing the size of \a arr.
 * \a type is the data type of the cell.
 */
#define zArrayDelete(arr,type,pos) do{\
  type *__zarray_ap;\
  if( zArrayPosIsValid(arr,pos) ){\
    if( (pos) < zArraySize(arr)-1 )\
      memmove( zArrayElemNC(arr,pos), zArrayElemNC(arr,(pos)+1), sizeof(type)*(zArraySize(arr)-(pos)-1) );\
    if( zArraySize(arr) > 1 ){\
      __zarray_ap = zRealloc( zArrayBuf(arr), type, zArraySize(arr)-1 );\
      if( __zarray_ap == NULL )\
        ZALLOCERROR();\
      else{\
        zArraySize(arr)--;\
        zArrayBuf(arr) = __zarray_ap;\
      }\
    } else{\
      zArrayFree( arr );\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArraySize(arr)-1 );\
  }\
} while(0)

/*! \brief append an array to another.
 *
 * zArrayAppend() appends an array pointed by \a subarr
 * to another array pointed by \a arr.
 * \a type is the data type of the cell.
 */
#define zArrayAppend(arr,subarr,type) do{\
  type *__zarray_ap;\
  __zarray_ap = zRealloc( zArrayBuf(arr), type, zArraySize(arr)+zArraySize(subarr) );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArrayBuf(arr) = __zarray_ap;\
    memcpy( zArrayElemNC(arr,zArraySize(arr)), zArrayBuf(subarr), zArraySize(subarr)*sizeof(type) );\
    zArraySize(arr) += zArraySize(subarr);\
  }\
} while(0)

#endif /* __KERNEL__ */

/*! \brief quick sort for a pointer array.
 *
 * zQuickSort() sorts an array pointed by \a array based on the quick sort
 * algorithm. \a nmemb is the number of components of \a array, and \a size
 * is the size of each component.
 * The components of \a array will be sorted in ascending order according
 * to the comparison function \a cmp. Namely, a factor 'a' in the \a array
 * is put after another factor 'b' if cmp(a,b,priv) > 0, where \a priv is
 * for programmer's utility.
 * \return
 * zQuickSort() returns no value.
 */
__EXPORT void zQuickSort(void *array, size_t nmemb, size_t size, int (*cmp)(void*,void*,void*), void *priv);

/*! \brief quick sort for an array.
 *
 * zArrayQuickSort() is a macro which is a wrapper of zQuickSort() that sorts
 * an array pointed by \a array based on the quick sort algorithm. \a cmp and
 * \a priv have the same roles with those for zQuickSort().
 * \sa
 * zQuickSort()
 */
#define zArrayQuickSort(arr,cmp,priv) zQuickSort( (void*)zArrayBuf(arr), zArraySize(arr), zArrayElemSize(arr), cmp, priv )

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
struct array_t{\
  uint size[2];\
  cell_t *buf;\
}
#else
#define zArray2Class(array_t,cell_t) \
typedef struct{\
  uint size[2];\
  cell_t *buf;\
} array_t
#endif /* __cplusplus */

#define zArray2Size(a,i)  (a)->size[i]
#define zArray2RowSize(a) (a)->size[0]
#define zArray2ColSize(a) (a)->size[1]
#define zArray2Buf(a)     ( (a)->buf )

#define zArray2PosIsValid(a,r,c) ( (r) < zArray2RowSize(a) && (r) >= 0 && (c) < zArray2ColSize(a) && (c) >= 0 )

/* NOTE: do not use the following macro before allocating buffer */
#define zArray2ElemSize(a)       sizeof(*zArray2Buf(a))

#define zArray2ElemNC(a,i,j)      ( &zArray2Buf(a)[(i)*zArray2ColSize(a) + (j)] )
#define zArray2Elem(a,i,j)        ( zArray2PosIsValid(a,i,j) ? zArray2ElemNC(a,i,j) : NULL )
#define zArray2SetElemNC(a,i,j,d) memcpy( zArray2ElemNC(a,i,j), (d), zArray2ElemSize(a) )
#define zArray2SetElem(a,i,j,d)   ( zArray2PosIsValid(a,i,j) ? zArray2SetElemNC(a,i,j,d) : NULL )

#define zArray2Init(arr) do{\
  zArray2RowSize(arr) = zArray2ColSize(arr) = 0;\
  zArray2Buf(arr) = NULL;\
} while(0)
/*! \brief allocate an array.
 * \param arr array class instance to be allocated.
 * \param type the data type of the array cells.
 * \param n the number of cells.
 */
#define zArray2Alloc(arr,type,r,c) do{\
  zArray2Init( arr );\
  if( (r) > 0 && (c) > 0 && !( zArray2Buf(arr) = zAlloc(type,(r)*(c)) ) ){\
    ZALLOCERROR();\
    zArray2RowSize(arr) = 0;\
    zArray2ColSize(arr) = 0;\
  } else{\
    zArray2RowSize(arr) = (r);\
    zArray2ColSize(arr) = (c);\
  }\
} while(0)

/*! \brief free an array.
 * \param arr a pointer to the array to be freed.
 */
#define zArray2Free(arr) do{\
  free( zArray2Buf(arr) );\
  zArray2Init( arr );\
} while(0)

/*! \} */

__END_DECLS

#endif /* __ZEDA_ARRAY_H__ */
