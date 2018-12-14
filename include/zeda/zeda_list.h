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
 * A macro zListClass() can simultaneously generate
 * a new (bidirectional) list class and a new list cell
 * class.
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
struct cell_t{\
  cell_t *prev, *next;\
  data_t data;\
  cell_t(){ prev = next = this; };\
};\
struct list_t{\
  int num;\
  cell_t root;\
  list_t(){ num = 0; };\
}
#else
#define zListClass(list_t,cell_t,data_t) \
typedef struct __##cell_t{\
  struct __##cell_t *prev, *next;\
  data_t data;\
} cell_t;\
typedef struct __##list_t{\
  int num;\
  cell_t root;\
} list_t
#endif /* __cplusplus */

/* dummy list cell class */
zListClass(zList, zListCell, void*);

/*! \brief a pointer to the previous cell of \a c. */
#define zListCellPrev(c)       (c)->prev
/*! \brief a pointer to the next cell of \a c. */
#define zListCellNext(c)       (c)->next
/*! \brief set the previous cell of \a c for \a p. */
#define zListCellSetPrev(c,p)  ( zListCellPrev(c) = (p) )
/*! \brief set the next cell of \a c for \a n. */
#define zListCellSetNext(c,n)  ( zListCellNext(c) = (n) )

/*! \brief initialize a list cell \a c. */
#define zListCellInit(c) do{\
  zListCellSetNext( c, c ); \
  zListCellSetPrev( c, c ); \
} while(0)

/*! \brief bind two list cells \a l and \a f.
    \a l is the latter, while \a f is the former. */
#define zListCellBind(l,f) do{\
  zListCellSetPrev( f, l ); \
  zListCellSetNext( l, f ); \
} while(0)

/*! \brief insert a list cell \a n to the next of \a c. */
#define zListCellInsertNext(c,n) do{\
  zListCellBind( n, zListCellNext(c) ); \
  zListCellBind( c, n ); \
} while(0)

/*! \brief insert a list cell \a p to the previous of \a c. */
#define zListCellInsertPrev(c,p) do{\
  zListCellBind( zListCellPrev(c), p ); \
  zListCellBind( p, c ); \
} while(0)

/*! \brief purge a cell \a c from a list. */
#define zListCellPurge(c) do{\
  zListCellBind( zListCellPrev((c)), zListCellNext((c)) ); \
  zListCellInit( (c) ); \
} while(0)

/*! \brief delete the next cell of \a c from a list.
    The deleted cell is stored into \a n. */
#define zListCellDeleteNext(c,n) do{\
  if( zListCellNext(c) != (c) ){ \
    *(n) = zListCellNext( c ); \
    zListCellPurge( *(n) );\
  } else\
    *(n) = NULL;\
} while(0)

/*! \brief delete the previous cell from a list.
    The deleted cell is stored into \a p. */
#define zListCellDeletePrev(c,p) do{\
  if( zListCellPrev(c) != (c) ){ \
    *(p) = zListCellPrev( c ); \
    zListCellPurge( *(p) );\
  } else\
    *(p) = NULL;\
} while(0)

/*! \brief swap the positions of two list cells \a c1 and \a c2.
    \a cell_t is the type of list cells. */
#define zListCellSwap(cell_t,c1,c2) do{\
  cell_t *__zlist_cell_swap_tmp; \
 \
  __zlist_cell_swap_tmp = zListCellPrev( c1 ); \
  zListCellBind( zListCellPrev(c2), c1 ); \
  zListCellBind( __zlist_cell_swap_tmp, c2 ); \
  __zlist_cell_swap_tmp = zListCellNext( c1 ); \
  zListCellBind( c1, zListCellNext(c2) ); \
  zListCellBind( c2, __zlist_cell_swap_tmp ); \
} while(0)

#ifndef __KERNEL__
/*! \brief write connections around a list cell \a c
    to the file \a f. */
#define zListCellFWrite(f,c) do{\
  fprintf( f, "cell [%p] ", c ); \
  fprintf( f, "%p <- prev | next-> %p\n", \
    zListCellPrev(c), zListCellNext(c) ); \
} while(0)
/*! \brief write connections around a list cell \a c
    to the standard output. */
#define zListCellWrite(c)  zListCellFWrite( stdout, c )
#else
#define zListCellWrite(c) do{\
  printk( "cell [%p] ", c ); \
  printk( "%p <- prev | next-> %p\n", \
    zListCellPrev(c), zListCellNext(c) ); \
} while(0)
#endif /* __KERNEL__ */

/* ********************************************************** */
/* bidirectional ring list structure
 * ********************************************************** */

/*! \brief the number of cells in a list \a l. */
#define zListNum(l)      (l)->num
/*! \brief the root cell of a list \a l. */
#define zListRoot(l)     ( &(l)->root )
/*! \brief the head cell (the previous of the root) of a list \a l. */
#define zListHead(l)     zListCellPrev( zListRoot( l ) )
/*! \brief the tail cell (the next of the root) of a list \a l. */
#define zListTail(l)     zListCellNext( zListRoot( l ) )

/*! \brief set the number of cells in a list \a l for \a n.
    not preferable to use it. */
#define zListSetNum(l,n) ( zListNum(l) = (n) )
/*! \brief increment the number of cells in a list \a l.
    not preferable to use it. */
#define zListIncNum(l)   ( zListNum(l)++ )
/*! \brief decrement the number of cells in a list \a l.
    not preferable to use it. */
#define zListDecNum(l)   ( zListNum(l)-- )

/*! \brief check if a list \a l is empty. */
#define zListIsEmpty(l)  ( zListNum(l) == 0 )

/*! \brief initialize a list \a l. */
#define zListInit(l) do{\
  zListSetNum( l, 0 ); \
  zListCellInit( zListRoot( l ) ); \
} while(0)

/*! \brief destroy a list \a l. \a t is the type of list cells. */
#define zListDestroy(t,l) do{\
  t *__zlist_destroy_cell = NULL; \
 \
  while( !zListIsEmpty( l ) ){ \
    zListDeleteHead( l, &__zlist_destroy_cell ); \
    zFree( __zlist_destroy_cell ); \
  } \
} while(0)

/*! \brief insert a list cell \a n to the next of \a c in a list \a l. */
#define zListInsertNext(l,c,n) do{\
  zListCellInsertNext( c, n ); \
  zListIncNum(l); \
} while(0)

/*! \brief insert a list cell \a p to the previous of \a c in a list \a l. */
#define zListInsertPrev(l,c,p) do{\
  zListCellInsertPrev( c, p ); \
  zListIncNum(l); \
} while(0)

/*! \brief insert a list cell \a c to the head of a list \a l. */
#define zListInsertHead(l,c) zListInsertPrev( l, zListRoot(l), c )
/*! \brief insert a list cell \a c to the tail of a list \a l. */
#define zListInsertTail(l,c) zListInsertNext( l, zListRoot(l), c )

/*! \brief delete the next cell of \a c of a list \a l.
    The deleted cell is stored into \a n. */
#define zListDeleteNext(l,c,n) do{\
  zListCellDeleteNext( c, n ); \
  zListDecNum(l); \
} while(0)

/*! \brief delete the previous cell of \a c of a list \a l.
    The deleted cell is stored into \a p. */
#define zListDeletePrev(l,c,p) do{\
  zListCellDeletePrev( c, p ); \
  zListDecNum(l); \
} while(0)

/*! \brief delete the head cell of a list \a l.
    The deleted cell is stored into \a c. */
#define zListDeleteHead(l,c) zListDeletePrev( l, zListRoot(l), c )
/*! \brief delete the tail cell of a list \a l.
    The deleted cell is stored into \a c. */
#define zListDeleteTail(l,c) zListDeleteNext( l, zListRoot(l), c )

/*! \brief purge a list cell \a c in a list \a l. */
#define zListPurge(l,c) do{\
  zListCellPurge( c );\
  zListDecNum(l);\
} while(0)

/*! \brief append all cells in a list \a p to the head of
    another list \a a. As the result, \a p will be empty. */
#define zListAppendA(a,p) do{\
  if( !zListIsEmpty(p) ){\
    zListCellBind( zListHead(a), zListTail(p) );\
    zListCellBind( zListHead(p), zListRoot(a) );\
    zListNum(a) += zListNum(p);\
    zListInit(p);\
  }\
} while(0)

/*! \brief append all cells in a list \a p to the tail of
    another list \a a. As the result, \a p will be empty. */
#define zListAppendZ(a,p) do{\
  if( !zListIsEmpty(p) ){\
    zListCellBind( zListHead(p), zListTail(a) );\
    zListCellBind( zListRoot(a), zListTail(p) );\
    zListNum(a) += zListNum(p);\
    zListInit(p);\
  }\
} while(0)

#define zListAppend(a,p) zListAppendZ(a,p)

/*! \brief move a list to another. */
#define zListMove(src,dst) do{\
  zListCellBind( zListRoot(dst), zListTail(src) );\
  zListCellBind( zListHead(src), zListRoot(dst) );\
  zListSetNum( dst, zListNum(src) );\
} while(0)

/*! \brief swap two lists \a l1 and \a l2.
 */
#define zListSwap(cell_t,l1,l2) do{\
  int __tmpnum;\
  zListCellSwap( cell_t, zListRoot(l1), zListRoot(l2) );\
  __tmpnum = zListNum(l1);\
  zListSetNum( l1, zListNum(l2) );\
  zListSetNum( l2, __tmpnum );\
} while(0)

/*! \brief succeed a process for each cell of a list \a l from the
    current \a c to the head. */
#define zListToHead(l,c) \
  for( ; (c)!=zListRoot(l); (c)=zListCellNext(c) )
/*! \brief succeed a process for each cell of a list \a l from the
    current \a c to the tail. */
#define zListToTail(l,c) \
  for( ; (c)!=zListRoot(l); (c)=zListCellPrev(c) )
/*! \brief succeed a process for each cell in a list \a l.
    Each cell is pointed by \a c from the tail to the head. */
#define zListForEach(l,c) \
  for( (c)=zListTail(l); (c)!=zListRoot(l); (c)=zListCellNext(c) )
/*! \brief succeed a process for each cell in a list \a l.
    Each cell is pointed by \a c from the head back to the tail. */
#define zListForEachRew(l,c) \
  for( (c)=zListHead(l); (c)!=zListRoot(l); (c)=zListCellPrev(c) )

/*! \def zListItem(list,i,cp)
    \brief refer the \a i 'th cell of a list \a list, and let
    \a cp point the cell. */
#define zListItem(list,i,cp) do{\
  int __z_list_item_tmp;\
  *(cp) = NULL;\
  if( (i) >= 0 && (i) < zListNum(list) ){\
    if( (i) <= zListNum(list) - (i) ){\
      __z_list_item_tmp = 0;\
      zListForEach( list, *(cp) )\
        if( __z_list_item_tmp++ == (i) ) break;\
    } else{\
      __z_list_item_tmp = zListNum( list );\
      zListForEachRew( list, *(cp) )\
        if( --__z_list_item_tmp == (i) ) break;\
    }\
  }\
} while(0)

/*! \brief define the quick sort method for a list class.
 *
 * zListQuickSortDef() defines a quick sort function
 * for a given list class \a list_t and a list cell
 * class \a cell_t. \a list_t class must stand upon
 * \a cell_t class.
 *
 * The function defined will be named 'list_t'QuickSort()
 * with the following prototype.
 *
 * \a list_t *list_tQuickSort(\a list_t *list, int (*cmp)(void*,void*,void*), void *priv);
 *
 * The cells of \a list will be sorted in ascending order
 * according to the comparison function \a cmp.
 * (The factor \a a in the \a list is put after another
 * factor \a b if \a cmp(\a a,\a b,\a p) > 0. \a p is
 * for programmer's utility, given by \a priv.)
 */
#define zListQuickSortDef(list_t, cell_t) \
static void _##list_t##InnerQuickSort(cell_t *head, cell_t *tail, int (*cmp)(void*,void*,void*), void *priv);\
void _##list_t##InnerQuickSort(cell_t *head, cell_t *tail, int (*cmp)(void*,void*,void*), void *priv)\
{\
  cell_t *hp, *tp, *pivot, *p;\
\
  for( pivot=hp=head, tp=tail; ; ){\
    /* choose a pivot for the head value */\
    while( cmp( (void *)hp, (void *)pivot, priv ) > 0 && hp != tail )\
      hp = zListCellPrev( hp );\
    while( cmp( (void *)tp, (void *)pivot, priv ) < 0 && tp != head )\
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
    _##list_t##InnerQuickSort( head, zListCellNext(hp), cmp, priv );\
  if( tp != tail )\
    _##list_t##InnerQuickSort( zListCellPrev(tp), tail, cmp, priv );\
}\
\
list_t *list_t##QuickSort(list_t *list, int (* cmp)(void*,void*,void*), void *priv)\
{\
  _##list_t##InnerQuickSort( zListHead(list), zListTail(list), cmp, priv );\
  return list;\
}

/*! \brief output the list connection.
 *
 * zListFWrite() outputs the connection information of a
 * list \a list to the current position of a file \a fp
 * with the following form.
 *
 *  number = X

 *  <0> cell [0xXXXXXXXXX]

 *      0xXXXXXXXXA <- prev | next -> 0xXXXXXXXXB

 *  <1> cell [0xXXXXXXXXB]

 *      0xXXXXXXXXX <- prev | next -> 0xXXXXXXXXC

 *   ...
 *
 * zListWrite() is also available to output to the standard
 * output.
 * \sa
 * zListCellFWrite, zListCellWrite
 */
#ifndef __KERNEL__
__EXPORT void _zListFWrite(FILE *fp, zList *list);
#define zListFWrite(f,l)         _zListFWrite( f, (zList *)(l) )
#define zListWrite(l)            zListFWrite( stdout, l )
#else
void zListWrite(zList *list);
#endif /* __KERNEL__ */

/* zStack/zQueue alias */

/*! \brief stack push operation. */
#define zStackPush(s,v)          zListInsertHead(s,v)
/*! \brief stack pop operation. */
#define zStackPop(s,c)           zListDeleteHead(s,c)

/*! \brief enqueue operation. */
#define zQueueEnqueue(q,v)       zListInsertTail(q,v)
/*! \brief dequeue operation. */
#define zQueueDequeue(q,c)       zListDeleteHead(q,c)

/*! \} */

__END_DECLS

#endif /* __ZEDA_LIST_H__ */
