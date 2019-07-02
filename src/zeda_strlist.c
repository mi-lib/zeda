/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_string - string manipulations.
 */

#include <zeda/zeda_strlist.h>
#include <stdarg.h>

/* insert a string to a list of strings. */
zStrListCell *zStrListInsert(zStrList *list, char *str)
{
  zStrListCell *cell;

  if( !( cell = zAlloc( zStrListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( cell->data = zStrClone( str ) ) ){
    ZALLOCERROR();
    free( cell );
    return NULL;
  }
  zListInsertHead( list, cell );
  return cell;
}

/* free a cell of a list of strings. */
void zStrListCellFree(zStrListCell *cell)
{
  free( cell->data );
  free( cell );
}

/* destroy a list of strings. */
void zStrListDestroy(zStrList *list)
{
  zStrListCell *cell;

  while( !zListIsEmpty( list ) ){
    zListDeleteTail( list, &cell );
    zStrListCellFree( cell );
  }
}

/* get pointers to strings of a list. */
void zStrListGetPtr(zStrList *strlist, int n, ...)
{
  zStrListCell *cell;
  va_list args;
  char **ptr;

  cell = zListTail(strlist);
  va_start( args, n );
  while( --n >= 0 ){
    ptr = va_arg( args, char** );
    if( cell == zListRoot(strlist) )
      *ptr = NULL;
    else{
      *ptr = cell->data;
      cell = zListCellNext(cell);
    }
  }
  va_end( args );
}

/* find a string in a list of strings. */
zStrListCell *zStrListFindStr(zStrList *list, char *str)
{
  zStrListCell *cp;

  if( !str ) return NULL;
  zListForEach( list, cp )
    if( strcmp( cp->data, str ) == 0 ) return cp;
  return NULL;
}

/* print a list of strings to a file. */
void zStrListFPrint(FILE *fp, zStrList *list)
{
  zStrListCell *cell;

  zListForEach( list, cell )
    fprintf( fp, "%s\n", cell->data );
}

/* a list of addresses of strings. */

/* insert a string to a list of strings. */
zStrAddrListCell *zStrAddrListInsert(zStrAddrList *list, char *str)
{
  zStrAddrListCell *cell;

  if( !( cell = zAlloc( zStrAddrListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cell->data = str;
  zListInsertHead( list, cell );
  return cell;
}
