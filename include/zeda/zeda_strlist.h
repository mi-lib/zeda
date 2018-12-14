/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_string.h
 * \brief string manipulations.
 * \author Zhidao
 */

#ifndef __ZEDA_STRLIST_H__
#define __ZEDA_STRLIST_H__

#include <zeda/zeda_list.h>
#include <zeda/zeda_string.h>

__BEGIN_DECLS

#ifndef __KERNEL__

/*! \struct list of strings */
zListClass(zStrList, zStrListCell, char *);

/*! \brief insert a string to a list of strings from the tail. */
__EXPORT zStrListCell *zStrListInsert(zStrList *list, char *str, bool clone);

/*! \brief free a cell of a list of strings. */
__EXPORT void zStrListCellFree(zStrListCell *cell, bool clone);

/*! \brief destroy a list of strings. */
__EXPORT void zStrListDestroy(zStrList *list, bool clone);

/*! \brief get pointers to strings of a list. */
__EXPORT void zStrListGetPtr(zStrList *strlist, int n, ...);

/*! \brief output a list of strings to a file. */
__EXPORT void zStrListFWrite(FILE *fp, zStrList *list);
#define zStrListWrite(l) zStrListFWrite( stdout, (l) )

#endif /* __KERNEL__ */

__END_DECLS

#endif /* __ZEDA_STRLIST_H__ */
