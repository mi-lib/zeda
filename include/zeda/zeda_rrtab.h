/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_rrtab.h
 * \brief round-robin table.
 * \author Zhidao
 */

#ifndef __ZEDA_RRTAB_H__
#define __ZEDA_RRTAB_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup rrtab universal round-robin table.
 * \{ *//* ************************************************** */

/*! \def zRRTabClass(tab_t,cell_t)
 * \brief generate round-robin table class.
 *
 * A macro zRRTabClass() can generate a new round-robin
 * table class, which consists of the table size and
 * the pointer to the buffer.
 */
#define zRRTabClass(tab_t,cell_t) \
typedef struct{\
  uint num;\
  cell_t *buf;\
} tab_t;\
tab_t *tab_t##Alloc(tab_t *tab, uint n);\
void tab_t##Free(tab_t *tab);\
cell_t *tab_t##Cell(tab_t *tab, uint i, uint j);\

#define zRRTabClassMethod(tab_t,cell_t) \
tab_t *tab_t##Alloc(tab_t *tab, uint n)\
{\
  if( !( tab->buf = zAlloc( cell_t, n*(n-1)/2 ) ) ){\
    ZALLOCERROR();\
    return NULL;\
  }\
  tab->num = n;\
  return tab;\
}\
void tab_t##Free(tab_t *tab)\
{\
  zFree( tab->buf );\
  tab->num = 0;\
}\
cell_t *tab_t##Cell(tab_t *tab, uint i, uint j)\
{\
  if( i == j || i >= tab->num || j >= tab->num ) return NULL;\
  if( i > j ) zSwap( int, i, j );\
  return tab->buf + ( 2*tab->num - i - 1 ) * i / 2 + j - i - 1;\
}

/*! \} */

/* ********************************************************** */
/*! \defgroup booltab boolean table.
 * \{ *//* ************************************************** */

/*! \brief boolean table - an example of round-robin table.
 *
 * zBoolTab class can be utilized for round-robin checking
 * of pairs.
 * To set the true value for a pair of \a i th and \a j th
 * keys, call zBoolTabMark(). Or, to set the false value
 * for the keys, call zBoolTabUnmark().
 * zBoolTabCheck() tells if the pair of \a i th and \a j th
 * turns the boolean cell to be true.
 */
zRRTabClass( zRRBool, bool );

#define zRRBoolCheck(tab,i,j)   ( zRRBoolCell(tab,i,j) ? *zRRBoolCell(tab,i,j) : false )
#define zRRBoolMark(tab,i,j)    do{ if( zRRBoolCell(tab,i,j) ){ *zRRBoolCell(tab,i,j) = true; } } while( 0 )
#define zRRBoolUnmark(tab,i,j)  do{ if( zRRBoolCell(tab,i,j) ){ *zRRBoolCell(tab,i,j) = false; } } while( 0 )

/*! \} */

__END_DECLS

#endif /* __ZEDA_RRTAB_H__ */
