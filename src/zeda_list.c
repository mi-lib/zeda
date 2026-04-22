/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_list - list operation.
 */

#include <zeda/zeda_list.h>

#ifndef __KERNEL__
/* print information of a list to a file. */
void _zListFPrint(FILE *fp, zList *list)
{
  int i = 0;
  zListCell *cp;

  fprintf( fp, "size = %d\n", zListSize(list) );
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

  printk( "size = %d\n", zListSize(list) );
  zListForEach( list, cp ){
    printk( "<%d>", i++ );
    zListCellPrint( cp );
  }
}
#endif /* __KERNEL__ */

/* add an integer number to a list. */
bool zIntListAdd(zIntList *list, int i)
{
  zIntListCell *cp;

  if( !( cp = zAlloc( zIntListCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  cp->data = i;
  zListInsertHead( list, cp );
  return true;
}

/* output a list of integer numbers. */
void zIntListFPrint(FILE *fp, zIntList *list)
{
  zIntListCell *cp;
  int i = 0;

  if( zListIsEmpty( list ) )
    fprintf( fp, "list is empty.\n" );
  else{
    zListForEach( list, cp ){
      if( i == 10 ){
        fprintf( fp, "\n" );
        i = 0;
      }
      fprintf( fp, " %d", cp->data );
      i++;
    }
    fprintf( fp, "\n" );
  }
}
