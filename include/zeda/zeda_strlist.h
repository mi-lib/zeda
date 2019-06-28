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

/*! \struct list of strings.
 *
 * Memory for the specified strings is internally allocated.
 */
zListClass(zStrList, zStrListCell, char *);

/*! \brief insert a string to a list of strings from the tail. */
__EXPORT zStrListCell *zStrListInsert(zStrList *list, char *str);

/*! \brief free a cell of a list of strings. */
__EXPORT void zStrListCellFree(zStrListCell *cell);

/*! \brief destroy a list of strings. */
__EXPORT void zStrListDestroy(zStrList *list);

/*! \brief get pointers to strings of a list. */
__EXPORT void zStrListGetPtr(zStrList *strlist, int n, ...);

/*! \brief print a list of strings to a file. */
__EXPORT void zStrListFPrint(FILE *fp, zStrList *list);
#define zStrListPrint(l) zStrListFPrint( stdout, (l) )

/*! \brief list of addresses of strings.
 *
 * Each cell of a list of this class only stores addresses of the
 * specified strings that is allocated elsewhere in the memory space.
 */
typedef zStrList zStrAddrList;
typedef zStrListCell zStrAddrListCell;

/*! \brief insert an address of a string to a list from the tail. */
__EXPORT zStrAddrListCell *zStrAddrListInsert(zStrAddrList *list, char *str);

/*! \brief destroy a list of addresses of strings. */
#define zStrAddrListDestroy(l) zListDestroy( zStrAddrListCell, l )

/*! \brief get pointers to strings of a list. */
#define zStrAddrListGetPtr zStrListGetPtr

/*! \brief print a list of strings to a file. */
#define zStrAddrListFPrint zStrListFPrint
#define zStrAddrListPrint zStrListPrint

#endif /* __KERNEL__ */

__END_DECLS

#endif /* __ZEDA_STRLIST_H__ */
