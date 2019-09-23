/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_list - list operation.
 */

#include <zeda/zeda_list.h>

/* ********************************************************** */
/* CATEGORY: dynamically-allocated list
 * ********************************************************** */

/* ********************************************************** */
/* CLASS: zList
 * list structure
 * ********************************************************** */

#ifndef __KERNEL__
/* print information of a list to a file. */
void _zListFPrint(FILE *fp, zList *list)
{
  register int i = 0;
  zListCell *cp;

  fprintf( fp, "size = %d\n", zListSize( list ) );
  zListForEach( list, cp ){
    fprintf( fp, "<%d>", i++ );
    zListCellFPrint( fp, cp );
  }
}
#else
/* print information of a list (for kernel module). */
void zListPrint(zList *list)
{
  int i;
  zListCell *cp;

  printk( "size = %d\n", zListSize( list ) );
  zListForEach( list, cp ){
    printk( "<%d>", i++ );
    zListCellPrint( cp );
  }
}
#endif /* __KERNEL__ */
