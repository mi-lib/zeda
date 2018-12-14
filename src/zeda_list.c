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
/* _zListFWrite
 * - print out an information of a list.
 */
void _zListFWrite(FILE *fp, zList *list)
{
  register int i = 0;
  zListCell *cp;

  fprintf( fp, "number = %d\n", zListNum( list ) );
  zListForEach( list, cp ){
    fprintf( fp, "<%d>", i++ );
    zListCellFWrite( fp, cp );
  }
}
#else
/* zListWrite
 * - print out an information of a list(for kernel module).
 */
void zListWrite(zList *list)
{
  int i;
  zListCell *cp;

  printk( "number = %d\n", zListNum( list ) );
  zListForEach( list, cp ){
    printk( "<%d>", i++ );
    zListCellWrite( cp );
  }
}
#endif /* __KERNEL__ */
