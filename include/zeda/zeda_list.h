/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_list.h
 * \brief list operation.
 * \author Zhidao
 */

#ifndef __ZEDA_LIST_H__
#define __ZEDA_LIST_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup dlist dynamically-allocated list
 * \{ *//* ************************************************** */

/* ********************************************************** */
/*! \def zListClass(list_t,cell_t,data_t)
 * \brief generate bidirectional ring list class.
 *
 * A macro zListClass() simultaneously generates a new
 * (bidirectional) list class and a new list cell class.
 *
 * The list cell class, which is named \a cell_t has
 * pointers to the previous and the next cells, and
 * the data with a specified type \a data_t.
 *
 * The list class, which is named \a list_t, has a root
 * cell with the type \a data_t and the number of cells.
 *//* ******************************************************* */

#ifdef __cplusplus
#define zListClass(list_t,cell_t,data_t) \
struct cell_t{ \
  cell_t *prev, *next; \
  data_t data; \
  cell_t() : prev{this}, next{this} {} \
};\
struct list_t{ \
  int size; \
  cell_t root; \
  list_t() : size{0} {} \
 public: \
  cell_t *head(){ return zListHead( this ); } \
  cell_t *tail(){ return zListTail( this ); } \
  void init(){ zListInit( this ); } \
  bool isEmpty(){ return zListIsEmpty( this ); } \
  void insertHead(cell_t *cell){ zListInsertHead( this, cell ); } \
  void insertTail(cell_t *cell){ zListInsertTail( this, cell ); } \
  cell_t *deleteHead(){ \
    cell_t *cellptr; \
    zListDeleteHead( this, &cellptr ); \
    return cellptr; \
  } \
  cell_t *deleteTail(){ \
    cell_t *cellptr; \
    zListDeleteTail( this, &cellptr ); \
    return cellptr; \
  } \
  cell_t *purge(cell_t *cell){ zListPurge( this, cell ); return cell; } \
  void append(list_t *renterlist){ zListAppend( this, renterlist ); } \
}
#else
#define zListClass(list_t,cell_t,data_t) \
typedef struct _##cell_t{\
  struct _##cell_t *prev, *next;\
  data_t data;\
} cell_t;\
typedef struct _##list_t{\
  int size;\
  cell_t root;\
} list_t
#endif /* __cplusplus */

/*! \brief a pointer to the previous cell of \a cell. */
#define zListCellPrev(cell) (cell)->prev
/*! \brief a pointer to the next cell of \a cell. */
#define zListCellNext(cell) (cell)->next
/*! \brief set the previous cell of \a cell for \a prevcell. */
#define zListCellSetPrev(cell,prevcell) ( zListCellPrev(cell) = (prevcell) )
/*! \brief set the next cell of \a cell for \a nextcell. */
#define zListCellSetNext(cell,nextcell) ( zListCellNext(cell) = (nextcell) )

/*! \brief initialize a list cell \a cell. */
#define zListCellInit(cell) do{\
  zListCellSetNext( cell, cell );\
  zListCellSetPrev( cell, cell );\
} while(0)

/*! \brief bind two list cells \a latter and \a former.
    \a latter is to be the latter cell, while \a fomer is to be the former cell. */
#define zListCellBind(latter,former) do{\
  zListCellSetPrev( former, latter );\
  zListCellSetNext( latter, former );\
} while(0)

/*! \brief insert a list cell \a nextcell to the next of \a cell. */
#define zListCellInsertNext(cell,nextcell) do{\
  zListCellBind( nextcell, zListCellNext(cell) );\
  zListCellBind( cell, nextcell );\
} while(0)

/*! \brief insert a list cell \a prevcell to the previous of \a cell. */
#define zListCellInsertPrev(cell,prevcell) do{\
  zListCellBind( zListCellPrev(cell), prevcell );\
  zListCellBind( prevcell, cell );\
} while(0)

/*! \brief purge a cell \a cell from a list. */
#define zListCellPurge(cell) do{\
  zListCellBind( zListCellPrev(cell), zListCellNext(cell) );\
  zListCellInit( cell );\
} while(0)

/*! \brief delete the next cell of \a cell from a list.
    The deleted cell is stored into \a nextcellptr. */
#define zListCellDeleteNext(cell,nextcellptr) do{\
  if( zListCellNext(cell) != (cell) ){\
    *(nextcellptr) = zListCellNext(cell);\
    zListCellPurge( *(nextcellptr) );\
  } else\
    *(nextcellptr) = NULL;\
} while(0)

/*! \brief delete the previous cell of \a cell from a list.
    The deleted cell is stored into \a prevcellptr. */
#define zListCellDeletePrev(cell,prevcellptr) do{\
  if( zListCellPrev(cell) != (cell) ){\
    *(prevcellptr) = zListCellPrev(cell);\
    zListCellPurge( *(prevcellptr) );\
  } else\
    *(prevcellptr) = NULL;\
} while(0)

/*! \brief swap the positions of two list cells \a cell1 and \a cell2.
    \a cell_t is the type of list cells. */
#define zListCellSwap(cell_t,cell1,cell2) do{\
  cell_t *__zlist_cell_swap_tmp; \
  __zlist_cell_swap_tmp = zListCellPrev(cell1);\
  zListCellBind( zListCellPrev(cell2), cell1 );\
  zListCellBind( __zlist_cell_swap_tmp, cell2 );\
  __zlist_cell_swap_tmp = zListCellNext(cell1);\
  zListCellBind( cell1, zListCellNext(cell2) );\
  zListCellBind( cell2, __zlist_cell_swap_tmp );\
} while(0)

#ifndef __KERNEL__
/*! \brief print connections around a list cell \a cell to the file \a file. */
#define zListCellFPrint(fp,cell) do{\
  fprintf( fp, "cell [%p] ", cell );\
  fprintf( fp, "%p <- prev | next-> %p\n", zListCellPrev(cell), zListCellNext(cell) );\
} while(0)
/*! \brief print pointing information of a list cell \a cell to the standard output. */
#define zListCellPrint(cell)  zListCellFPrint( stdout, cell )
#else
#define zListCellPrint(cell) do{\
  printk( "cell [%p] ", cell );\
  printk( "%p <- prev | next-> %p\n", zListCellPrev(cell), zListCellNext(cell) );\
} while(0)
#endif /* __KERNEL__ */

/* ********************************************************** */
/* bidirectional ring list structure
 * ********************************************************** */

/*! \brief the size of a list \a list. */
#define zListSize(list)     (list)->size
/*! \brief the root cell of a list \a list. */
#define zListRoot(list)     ( &(list)->root )
/*! \brief the head cell (the previous of the root) of a list \a list. */
#define zListHead(list)     zListCellPrev( zListRoot(list) )
/*! \brief the tail cell (the next of the root) of a list \a list. */
#define zListTail(list)     zListCellNext( zListRoot(list) )

/*! \brief set the size of a list \a list for \a n (not recommended to use in user programs). */
#define zListSetSize(list,n) ( zListSize(list) = (n) )
/*! \brief increment the size of a list \a list (not recommended to use in user programs). */
#define zListIncSize(list)   ( zListSize(list)++ )
/*! \brief decrement the size of a list \a list (not recommended to use in user programs). */
#define zListDecSize(list)   ( zListSize(list)-- )

/*! \brief check if a list \a list is empty. */
#define zListIsEmpty(list)   ( zListSize(list) == 0 )

/*! \brief initialize a list \a list. */
#define zListInit(list) do{\
  zListSetSize( list, 0 );\
  zListCellInit( zListRoot(list) );\
} while(0)

/*! \brief destroy a list \a list. \a cell_t is the type of list cells. */
#define zListDestroy(cell_t,list) do{\
  cell_t *__zlist_destroy_cell = NULL;\
  while( !zListIsEmpty( list ) ){\
    zListDeleteHead( list, &__zlist_destroy_cell );\
    zFree( __zlist_destroy_cell );\
  }\
} while(0)

/*! \brief insert a list cell \a nextcell to the next of \a cell in a list \a list. */
#define zListInsertNext(list,cell,nextcell) do{\
  zListCellInsertNext( cell, nextcell );\
  zListIncSize( list );\
} while(0)

/*! \brief insert a list cell \a prevcell to the previous of \a cell in a list \a list. */
#define zListInsertPrev(list,cell,prevcell) do{\
  zListCellInsertPrev( cell, prevcell );\
  zListIncSize( list );\
} while(0)

/*! \brief insert a list cell \a cell to the head of a list \a list. */
#define zListInsertHead(list,cell) zListInsertPrev( list, zListRoot(list), cell )
/*! \brief insert a list cell \a cell to the tail of a list \a list. */
#define zListInsertTail(list,cell) zListInsertNext( list, zListRoot(list), cell )

/*! \brief delete the next cell of \a cell of a list \a list.
    The deleted cell is stored into \a nextcellptr. */
#define zListDeleteNext(list,cell,nextcellptr) do{\
  zListCellDeleteNext( cell, nextcellptr );\
  zListDecSize( list );\
} while(0)

/*! \brief delete the previous cell of \a cell of a list \a list.
    The deleted cell is stored into \a prevcellptr. */
#define zListDeletePrev(list,cell,prevcellptr) do{\
  zListCellDeletePrev( cell, prevcellptr );\
  zListDecSize( list );\
} while(0)

/*! \brief delete the head cell of a list \a list.
    The deleted cell is stored into \a cellptr. */
#define zListDeleteHead(list,cellptr) zListDeletePrev( list, zListRoot(list), cellptr )
/*! \brief delete the tail cell of a list \a list.
    The deleted cell is stored into \a cellptr. */
#define zListDeleteTail(list,cellptr) zListDeleteNext( list, zListRoot(list), cellptr )

/*! \brief purge a list cell \a cell in a list \a list. */
#define zListPurge(list,cell) do{\
  zListCellPurge( cell );\
  zListDecSize( list );\
} while(0)

/*! \brief append all cells in a list \a renterlist to the head of another list \a borrowerlist.
    As the result, \a renterlist becomes empty. */
#define zListAppendA(borrowerlist,renterlist) do{\
  if( !zListIsEmpty( renterlist ) ){\
    zListCellBind( zListHead(borrowerlist), zListTail(renterlist) );\
    zListCellBind( zListHead(renterlist), zListRoot(borrowerlist) );\
    zListSize(borrowerlist) += zListSize(renterlist);\
    zListInit( renterlist );\
  }\
} while(0)

/*! \brief append all cells in a list \a renterlist to the tail of another list \a borrowerlist.
    As the result, \a renterlist becomes empty. */
#define zListAppendZ(borrowerlist,renterlist) do{\
  if( !zListIsEmpty( renterlist ) ){\
    zListCellBind( zListHead(renterlist), zListTail(borrowerlist) );\
    zListCellBind( zListRoot(borrowerlist), zListTail(renterlist) );\
    zListSize(borrowerlist) += zListSize(renterlist);\
    zListInit( renterlist );\
  }\
} while(0)

#define zListAppend(borrowerlist,renterlist) zListAppendZ( borrowerlist, renterlist )

/*! \brief move a list to another. */
#define zListMove(src,dest) do{\
  if( zListIsEmpty( dest ) ){\
    zListCellBind( zListRoot(dest), zListTail(src) );\
    zListCellBind( zListHead(src), zListRoot(dest) );\
    zListSetSize( dest, zListSize(src) );\
    zListInit( src );\
  }\
} while(0)

/*! \brief swap two lists \a list1 and \a list2. */
#define zListSwap(cell_t,list1,list2) do{\
  int __tmpsize;\
  zListCellSwap( cell_t, zListRoot(list1), zListRoot(list2) );\
  __tmpsize = zListSize(list1);\
  zListSetSize( list1, zListSize(list2) );\
  zListSetSize( list2, __tmpsize );\
} while(0)

/*! \brief duplicate a list \a src to \a dest. \a cell_t is the type of list cells. */
#define zListDup(cell_t,src,dest) do{\
  cell_t *__zlist_scp, *__zlist_new_cell;\
  zListInit( dest );\
  zListForEach( src, __zlist_scp ){\
    if( !( __zlist_new_cell = zAlloc( cell_t, 1 ) ) ){\
      ZALLOCERROR();\
      break;\
    }\
    zCopy( cell_t, __zlist_scp, __zlist_new_cell );\
    zListInsertHead( dest, __zlist_new_cell );\
  }\
  if( zListSize( dest ) != zListSize( src ) ){\
    zListDestroy( cell_t, dest );\
  }\
} while(0)

/*! \brief succeed a process for each cell of a list \a list from the current \a cell to the head. */
#define zListToHead(list,cell) \
  for( ; (cell)!=zListRoot(list); (cell)=zListCellNext(cell) )
/*! \brief succeed a process for each cell of a list \a list from the current \a cell to the tail. */
#define zListToTail(list,cell) \
  for( ; (cell)!=zListRoot(list); (cell)=zListCellPrev(cell) )
/*! \brief succeed a process for each cell in a list \a list.
    Each cell is pointed by \a cell from the tail to the head. */
#define zListForEach(list,cell) \
  for( (cell)=zListTail(list); (cell)!=zListRoot(list); (cell)=zListCellNext(cell) )
/*! \brief succeed a process for each cell in a list \a list.
    Each cell is pointed by \a cell from the head back to the tail. */
#define zListForEachRew(list,cell) \
  for( (cell)=zListHead(list); (cell)!=zListRoot(list); (cell)=zListCellPrev(cell) )

/*! \def zListItem(list,i,cp)
    \brief refer the \a i th cell of a list \a list, and let \a cp point the cell. */
#define zListItem(list,i,cp) do{\
  int __z_list_item_tmp;\
  *(cp) = NULL;\
  if( (i) >= 0 && (i) < zListSize(list) ){\
    if( (i) <= zListSize(list) - (i) ){\
      __z_list_item_tmp = 0;\
      zListForEach( list, *(cp) )\
        if( __z_list_item_tmp++ == (i) ) break;\
    } else{\
      __z_list_item_tmp = zListSize( list );\
      zListForEachRew( list, *(cp) )\
        if( --__z_list_item_tmp == (i) ) break;\
    }\
  }\
} while(0)

/*! \brief find a list cell of \a list by a name string \a _name. */
#define zListFindName(list,_name,cp) do{\
  zListForEach( list, *(cp) ){\
    if( strcmp( zName(&(*(cp))->data), _name ) == 0 ) break;\
  }\
  if( *(cp) == zListRoot(list) ) *(cp) = NULL;\
} while(0)

/*! \brief define the quick sort method for a list class.
 *
 * zListQuickSortDef() defines a quick sort function for a given list class \a list_t and a list cell
 * class \a cell_t. \a list_t class must stand upon \a cell_t class.
 *
 * The function defined will be named 'list_t'QuickSort() with the following prototype.
 *
 * \a list_t *list_tQuickSort(\a list_t *list, int (*cmp)(void*,void*,void*), void *util);
 *
 * The cells of \a list will be sorted in ascending order according to the comparison function \a cmp.
 * (The factor \a a in the \a list is put after another factor \a b if \a cmp(\a a,\a b,\a p) > 0.
 * \a p is for programmer's utility, given by \a util.)
 */
#define zListQuickSortDef(list_t, cell_t) \
static void _##list_t##InnerQuickSort(cell_t *head, cell_t *tail, int (*cmp)(void*,void*,void*), void *util);\
void _##list_t##InnerQuickSort(cell_t *head, cell_t *tail, int (*cmp)(void*,void*,void*), void *util)\
{\
  cell_t *hp, *tp, *pivot, *p;\
\
  for( pivot=hp=head, tp=tail; ; ){\
    /* choose a pivot for the head value */\
    while( cmp( (void *)hp, (void *)pivot, util ) > 0 && hp != tail )\
      hp = zListCellPrev( hp );\
    while( cmp( (void *)tp, (void *)pivot, util ) < 0 && tp != head )\
      tp = zListCellNext( tp );\
    if( hp == tp || hp == zListCellPrev( tp ) )\
      break;\
    zListCellSwap( cell_t, hp, tp );\
    /* if head or tail is swapped, it should be renewed. */\
    if( hp == head ) head = tp;\
    if( tail == tp ) tail = hp;\
    p = zListCellNext( hp );\
    hp = zListCellPrev( tp );\
    tp = p;\
  }\
  if( hp != head )\
    _##list_t##InnerQuickSort( head, zListCellNext(hp), cmp, util );\
  if( tp != tail )\
    _##list_t##InnerQuickSort( zListCellPrev(tp), tail, cmp, util );\
}\
\
list_t *list_t##QuickSort(list_t *list, int (* cmp)(void*,void*,void*), void *util)\
{\
  _##list_t##InnerQuickSort( zListHead(list), zListTail(list), cmp, util );\
  return list;\
}

/* dummy list cell class */
zListClass(zList, zListCell, void*);

/*! \brief print connection information of a list.
 *
 * zListFPrint() prints the connection information of a list \a list to the current position of
 * a file \a fp with the following form.
 *
 *  number = X

 *  <0> cell [0xXXXXXXXXX]

 *      0xXXXXXXXXA <- prev | next -> 0xXXXXXXXXB

 *  <1> cell [0xXXXXXXXXB]

 *      0xXXXXXXXXX <- prev | next -> 0xXXXXXXXXC

 *   ...
 *
 * zListPrint() is also available to print the information to the standard output.
 * \sa
 * zListCellFPrint, zListCellPrint
 */
#ifndef __KERNEL__
__ZEDA_EXPORT void _zListFPrint(FILE *fp, zList *list);
#define zListFPrint(fp,list) _zListFPrint( fp, (zList *)(list) )
#define zListPrint(list)     zListFPrint( stdout, list )
#else
void zListPrint(zList *list);
#endif /* __KERNEL__ */

/* zStack/zQueue alias */

/*! \brief stack push operation. */
#define zStackPush(stack,cell) zListInsertHead(stack,cell)
/*! \brief stack pop operation. */
#define zStackPop(stack,cell)  zListDeleteHead(stack,cell)

/*! \brief pop the head cell of a stack, and push it to another. */
#define zStackMoveCell(cell_t,src,dest) do{\
  cell_t *__z_list_stack_move_cp;\
  zStackPop( src, &__z_list_stack_move_cp );\
  zStackPush( dest, __z_list_stack_move_cp );\
} while(0)

/*! \brief enqueue operation. */
#define zQueueEnqueue(queue,cell) zListInsertTail(queue,cell)
/*! \brief dequeue operation. */
#define zQueueDequeue(queue,cell) zListDeleteHead(queue,cell)

/*! \brief dequeue the head cell of a queue, and enqueue it to another. */
#define zQueueMoveCell(cell_t,src,dest) do{\
  cell_t *__z_list_queue_move_cp;\
  zQueueDequeue( src, &__z_list_queue_move_cp );\
  zQueueEnqueue( dest, __z_list_queue_move_cp );\
} while(0)

/*! \} */

/*! \brief a list of integer numbers */
zListClass( zIntList, zIntListCell, int );

/*! \brief a list of integer numbers */
__ZEDA_EXPORT bool zIntListAdd(zIntList *list, int i);

__END_DECLS

#endif /* __ZEDA_LIST_H__ */
