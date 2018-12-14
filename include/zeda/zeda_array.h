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
 * consists of the total number of elements and the pointer to
 * the array buffer.
 * \a array_t is the class name to be defined.
 * \a cell_t is the class name of the data to be arrayed.
 *
 * The number of elements is acquired by zArrayNum().
 * The array head can be accessed by zArrayBuf().
 * Each element can be accessed by zArrayElem().
 * For a faster access to each element, zArrayElemNC() is also
 * available, which does not check the specified location.
 *
 * For setting the number of elements and the array buffer,
 * zArraySetNum() and zArraySetBuf() are available. However,
 * it is not preferable to use them directly. Use zArrayAlloc()
 * instead.
 *
 * The array can be freed by calling zArrayFree().
 *
 * zArraySetElem() is available for setting each element, which
 * works only if the specified location is valid. zArraySetElemNC()
 * is also available, which provides a faster access to each
 * element without checking the validity of specified location.
 * For frequent accesses to the head, neck (one-cell-before-head)
 * and tail, zArrayHead(), zArrayNeck(), zArrayTail(),
 * zArraySetHead(), zArraySetNeck() and zArraySetTail() are
 * available.
 */
#ifdef __cplusplus
#define zArrayClass(array_t,cell_t) \
struct array_t{\
  uint num;\
  cell_t *buf;\
}
#else
#define zArrayClass(array_t,cell_t) \
typedef struct{\
  uint num;\
  cell_t *buf;\
} array_t
#endif /* __cplusplus */

#define zArrayNum(a)           (a)->num
#define zArrayBuf(a)           ( (a)->buf )

#define zArraySetNum(a,n)      ( zArrayNum(a) = (n) )
#define zArraySetBuf(a,b)      ( zArrayBuf(a) = (b) )

#define zArrayPosIsValid(a,p)  ( (p) < zArrayNum(a) && (p) >= 0 )

/* NOTE: do not use the following macro before allocating buffer */
#define zArrayElemSize(a)     sizeof(*zArrayBuf(a))

#define zArrayElemNC(a,i)      ( &zArrayBuf(a)[i] )
#define zArrayElem(a,i)        ( zArrayPosIsValid(a,i) ? zArrayElemNC(a,i) : NULL )
#define zArraySetElemNC(a,i,d) memcpy( zArrayElemNC(a,i), (d), zArrayElemSize(a) )
#define zArraySetElem(a,i,d)   if( zArrayPosIsValid(a,i) ) zArraySetElemNC(a,i,d)

#define zArrayHead(a)          zArrayElemNC( a, zArrayNum(a)-1 )
#define zArraySetHead(a,d)     zArraySetElemNC( a, zArrayNum(a)-1, d )
#define zArrayNeck(a)          zArrayElemNC( a, zArrayNum(a)-2 )
#define zArraySetNeck(a,d)     zArraySetElemNC( a, zArrayNum(a)-2, d )
#define zArrayTail(a)          zArrayElemNC( a, 0 )
#define zArraySetTail(a,d)     zArraySetElemNC( a, 0, d )

#define zArrayInit(arr) do{\
  zArraySetNum( arr, 0 );\
  zArraySetBuf( arr, NULL );\
} while(0)
/*! \brief allocate an array.
 * \param arr array class instance to be allocated.
 * \param type the data type of the array cells.
 * \param n the number of cells.
 */
#define zArrayAlloc(arr,type,n) do{\
  zArrayInit( arr );\
  if( (n) > 0 && !zArraySetBuf( arr, zAlloc(type,n) ) ){\
    ZALLOCERROR();\
    zArraySetNum( arr, 0 );\
  } else\
    zArraySetNum( arr, n );\
} while(0)

/*! \brief free an array.
 * \param arr a pointer to the array to be freed.
 */
#define zArrayFree(arr) do{\
  zFree( zArrayBuf(arr) );\
  zArraySetNum( arr, 0 );\
} while(0)

/*! zArrayFindName() is valid for an array of a named class.
 * \sa zNameFind.
 */
#define zArrayFindName(arr,name,ptr) \
  zNameFind( zArrayBuf(arr), zArrayNum(arr), name, ptr )

#ifndef __KERNEL__
/* since realloc() is not prepared in kernel space,
 * the following methods are only available in user space.
 */

/*! \brief add a new cell to an array.
 *
 * zArrayAdd() adds a new cell pointed by \a dat to an
 * array \a arr at the last, incrementing the number of
 * \a arr. \a type is the data type of the cell.
 */
#define zArrayAdd(arr,type,dat) do{\
  type *__zarray_ap;\
  __zarray_ap = zRealloc( zArrayBuf(arr), type, zArrayNum(arr)+1 );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArrayNum(arr)++;\
    zArrayBuf(arr) = __zarray_ap;\
    zArraySetHead( arr, dat );\
  }\
} while(0)

/*! \brief insert a new cell into an array.
 *
 * zArrayInsert() inserts a new cell pointed by \a dat to in an
 * array \a arr at the location specified by \a pos, incrementing
 * the number of \a arr. \a type is the data type of the cell.
 */
#define zArrayInsert(arr,type,pos,dat) do{\
  type *__zarray_ap;\
  if( (pos) == zArrayNum(arr) ){\
    zArrayAdd( arr, type, dat );\
  } else if( zArrayPosIsValid(arr,pos) ){\
    __zarray_ap = zRealloc( zArrayBuf(arr), type, zArrayNum(arr)+1 );\
    if( __zarray_ap == NULL )\
      ZALLOCERROR();\
    else{\
      zArrayBuf(arr) = __zarray_ap;\
      memmove( zArrayElemNC(arr,(pos)+1), zArrayElemNC(arr,pos), sizeof(type)*(zArrayNum(arr)-(pos)) );\
      zArraySetElemNC( arr, pos, dat );\
      zArrayNum(arr)++;\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArrayNum(arr)-1 );\
  }\
} while(0)

/*! \brief delete a cell from an array.
 *
 * zArrayDelete() deletes a cell at the location specified by
 * \a pos in an array \a arr at, decrementing the number of \a arr.
 * \a type is the data type of the cell.
 */
#define zArrayDelete(arr,type,pos) do{\
  type *__zarray_ap;\
  if( zArrayPosIsValid(arr,pos) ){\
    if( (pos) < zArrayNum(arr)-1 )\
      memmove( zArrayElemNC(arr,pos), zArrayElemNC(arr,(pos)+1), sizeof(type)*(zArrayNum(arr)-(pos)-1) );\
    if( zArrayNum(arr) > 1 ){\
      __zarray_ap = zRealloc( zArrayBuf(arr), type, zArrayNum(arr)-1 );\
      if( __zarray_ap == NULL )\
        ZALLOCERROR();\
      else{\
        zArrayNum(arr)--;\
        zArrayBuf(arr) = __zarray_ap;\
      }\
    } else{\
      zArrayFree( arr );\
    }\
  } else{\
    ZRUNWARN( "invalid position %d/%d in array specified", pos, zArrayNum(arr)-1 );\
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
  __zarray_ap = zRealloc( zArrayBuf(arr), type, zArrayNum(arr)+zArrayNum(subarr) );\
  if( __zarray_ap == NULL )\
    ZALLOCERROR();\
  else{\
    zArrayBuf(arr) = __zarray_ap;\
    memcpy( zArrayElemNC(arr,zArrayNum(arr)), zArrayBuf(subarr), zArrayNum(subarr)*sizeof(type) );\
    zArrayNum(arr) += zArrayNum(subarr);\
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
#define zArrayQuickSort(arr,cmp,priv) zQuickSort( (void*)zArrayBuf(arr), zArrayNum(arr), zArrayElemSize(arr), cmp, priv )

/*! \} */

__END_DECLS

#endif /* __ZEDA_ARRAY_H__ */
