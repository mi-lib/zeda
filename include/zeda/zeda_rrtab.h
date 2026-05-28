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

/* ********************************************************** */
/*! \defgroup rrtab universal round-robin table.
 * \{ *//* ************************************************** */

#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_PROPERTY(cell_t) \
  uint num; \
  cell_t *buf;

#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_ALLOC_PROTOTYPE(tab_t) \
  tab_t *tab_t##Alloc(tab_t *tab, uint n)
#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_FREE_PROTOTYPE(tab_t) \
  void tab_t##Free(tab_t *tab)
#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_CELL_PROTOTYPE(tab_t,cell_t) \
  cell_t *tab_t##Cell(tab_t *tab, uint i, uint j)

#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_METHOD_PROTOTYPE(method_export,tab_t,cell_t) \
  method_export ZEDA_DEF_ROUNDROBINTABLE_CLASS_ALLOC_PROTOTYPE( tab_t ); \
  method_export ZEDA_DEF_ROUNDROBINTABLE_CLASS_FREE_PROTOTYPE( tab_t ); \
  method_export ZEDA_DEF_ROUNDROBINTABLE_CLASS_CELL_PROTOTYPE( tab_t, cell_t );

/*! \def ZEDA_DEF_ROUNDROBINTABLE_CLASS(tab_t,cell_t)
 * \brief generate round-robin table class.
 *
 * A macro ZEDA_DEF_ROUNDROBINTABLE_CLASS can generate a new round-robin table class, which consists of
 * the table size and the pointer to the buffer.
 */
#ifdef __cplusplus
#define ZEDA_DEF_ROUNDROBINTABLE_CLASS(method_export,tab_t,cell_t) \
ZDECL_STRUCT( tab_t ); \
ZEDA_DEF_ROUNDROBINTABLE_CLASS_METHOD_PROTOTYPE( method_export, tab_t, cell_t ) \
ZDEF_STRUCT( , tab_t ){ \
  ZEDA_DEF_ROUNDROBINTABLE_CLASS_PROPERTY( cell_t ) \
 public: \
  tab_t() : num{0}, buf{NULL} {} \
  tab_t(int _num){ tab_t##Alloc( this, _num ); } \
  ~tab_t(){ tab_t##Free( this ); } \
  tab_t *_alloc(uint _num){ return tab_t##Alloc( this, _num ); } \
  void _free(){ tab_t##Free( this ); } \
  cell_t &cell(uint i, uint j){ return *tab_t##Cell( this, i, j ); } \
};
#else
#define ZEDA_DEF_ROUNDROBINTABLE_CLASS(method_export,tab_t,cell_t) \
ZDEF_STRUCT( , tab_t ){ \
  ZEDA_DEF_ROUNDROBINTABLE_CLASS_PROPERTY( cell_t ) \
}; \
ZEDA_DEF_ROUNDROBINTABLE_CLASS_METHOD_PROTOTYPE( method_export, tab_t, cell_t )
#endif /* __cplusplus */

#define ZEDA_DEF_ROUNDROBINTABLE_CLASS_METHOD(tab_t,cell_t) \
ZEDA_DEF_ROUNDROBINTABLE_CLASS_ALLOC_PROTOTYPE( tab_t ) \
{ \
  if( !( tab->buf = zAlloc( cell_t, n*(n-1)/2 ) ) ){ \
    ZALLOCERROR(); \
    return NULL; \
  } \
  tab->num = n; \
  return tab; \
}\
ZEDA_DEF_ROUNDROBINTABLE_CLASS_FREE_PROTOTYPE( tab_t ) \
{ \
  zFree( tab->buf ); \
  tab->num = 0; \
}\
ZEDA_DEF_ROUNDROBINTABLE_CLASS_CELL_PROTOTYPE( tab_t, cell_t ) \
{ \
  if( i == j || i >= tab->num || j >= tab->num ) return NULL; \
  if( i > j ) zSwap( int, i, j ); \
  return tab->buf + ( 2*tab->num - i - 1 ) * i / 2 + j - i - 1; \
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
ZEDA_DEF_ROUNDROBINTABLE_CLASS( __ZEDA_EXPORT, zRRBool, bool )

__BEGIN_DECLS

#define zRRBoolCheck(tab,i,j)   ( zRRBoolCell(tab,i,j) ? *zRRBoolCell(tab,i,j) : false )
#define zRRBoolMark(tab,i,j)    do{ if( zRRBoolCell(tab,i,j) ){ *zRRBoolCell(tab,i,j) = true; } } while( 0 )
#define zRRBoolUnmark(tab,i,j)  do{ if( zRRBoolCell(tab,i,j) ){ *zRRBoolCell(tab,i,j) = false; } } while( 0 )

__END_DECLS

/*! \} */

#endif /* __ZEDA_RRTAB_H__ */
