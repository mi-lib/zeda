/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_name.h
 * \brief common named class.
 * \author Zhidao
 */

#ifndef __ZEDA_NAME_H__
#define __ZEDA_NAME_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

/*! \def Z_NAMED_CLASS
 * \brief an inheritance of named class.
 *
 * Z_NAMED_CLASS macro works as a keyword to inherit a
 * named class, namely, a class which can be identified
 * by a string.
 *
 * The inheritance can have a string to represent its
 * name, which is referred by zName(). When setting the
 * name, use zNameSet(). When destroying the instance,
 * call zNameDestroy() to free the name string.
 *
 * When searching an instance with a name \a name from
 * an array of the class, call zNameFind( array, n, name, ptr );
 * where \a array is the pointer to array, \a n is the
 * size of the array, \a name is a string to be referred,
 * and \a ptr is a pointer to the element found. If it
 * fails to find a corresponding element, the null pointer
 * set for \a ptr.
 */
#ifdef __cplusplus
#define Z_NAMED_CLASS    public: char *name;
#else
#define Z_NAMED_CLASS    char *name;
#endif /* __cplusplus */

/*! \brief the default name for no-name. */
#define ZNONAME          "noname"

#define zNamePtr(c)      (c)->name
#define zNameSetPtr(c,p) ( (c)->name = (p) )
#define zName(c)         ( (c) && zNamePtr(c) ? zNamePtr(c) : ZNONAME )
#define zNameSet(c,n)    zNameSetPtr( c, zStrClone(n) )
#define zNameDestroy(c)  zFree(zNamePtr(c))

#define zNameFind(array,n,name,ptr) do{\
  int __z_name_find_id;\
  (ptr) = NULL;\
  if( (name) != NULL && strcmp( (name), ZNONAME ) && (array) != NULL )\
    for( __z_name_find_id=0; __z_name_find_id<(n); __z_name_find_id++ )\
      if( zNamePtr( &(array)[__z_name_find_id] ) &&\
          !strcmp( (name), zName( (&(array)[__z_name_find_id]) ) ) ){\
        (ptr) = &(array)[__z_name_find_id];\
        break;\
      }\
} while(0)

__END_DECLS

#endif /* __ZEDA_NAME_H__ */
