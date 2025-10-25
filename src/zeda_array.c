/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_array - array operation.
 */

#include <zeda/zeda_array.h>

/* partition an array of pointers based on a pivot. */
int zQuickPartition(void *array, int nmemb, int size, int (* cmp)(void*,void*,void*), void *priv, int pivot_id)
{
  byte *base, *pivot;
  int i, t, h, m;

  base = (byte *)array;
  for( t=0, h=nmemb-1, m=pivot_id; ; t++, h-- ){
    pivot = base + size*m;
    while( cmp( pivot, base+size*t, priv ) > 0 ) t++;
    while( cmp( pivot, base+size*h, priv ) < 0 ) h--;
    if( t >= h ) break; /* no need to swap any elements in this try */
    for( i=0; i<size; i++ )
      zSwap( byte, base[size*t+i], base[size*h+i] );
    /* keep track of the pivot element */
    if( m == t ) m = h;
    else
    if( m == h ) m = t;
  }
  return t == h ? t + 1 : t;
}

/* quick sort for an array. */
void zQuickSort(void *array, int nmemb, int size, int (* cmp)(void*,void*,void*), void *priv)
{
  int t;

  if( nmemb <= 1 ) return;
  if( ( t = zQuickPartition( array, nmemb, size, cmp, priv, (nmemb-1)/2 ) ) > 1 )
    zQuickSort( array, t, size, cmp, priv );
  if( ( nmemb -= t ) > 1 )
    zQuickSort( (byte*)array+size*t, nmemb, size, cmp, priv );
}

/* insert a member into an array at sorted position. */
void *zInsertSort(void *array, void *memb, int i, int nmemb, int size, int (* cmp)(void*,void*,void*), void *priv)
{
  byte *p;

  if( i >= nmemb ){
    ZRUNERROR( "array already occupied" );
    return NULL;
  }
  for( p=(byte*)array+size*i; p > (byte*)array && cmp( p-size, memb, priv ) > 0; p-=size ){
    memcpy( p, p-size, size );
  }
  return memcpy( p, memb, size );
}

/* select an element of an array recursively. */
static void *_zQuickSelect(void *array, int nmemb, int size, int order, int (* cmp)(void*,void*,void*), void *priv)
{
  int pid;

  if( nmemb <= 1 ) return array;
  pid = zQuickPartition( array, nmemb, size, cmp, priv, ( nmemb - 1 ) / 2 );
  return order < pid ?
    _zQuickSelect( array, pid, size, order, cmp, priv ) :
    _zQuickSelect( (byte *)array+size*pid, nmemb - pid, size, order - pid, cmp, priv );
}

/* select an element of an array. */
void *zQuickSelect(void *array, int nmemb, int size, int order, int (* cmp)(void*,void*,void*), void *priv)
{
  if( !cmp ){
    ZRUNERROR( ZEDA_ERR_QUICKSORT_NO_COMPARATOR );
    return NULL;
  }
  if( nmemb <= 0 ){
    ZRUNERROR( ZEDA_ERR_QUICKSELECT_EMPTY );
    return NULL;
  }
  if( order < 0 || order >= nmemb ){
    ZRUNERROR( ZEDA_ERR_QUICKSELECT_INVALID_ORDER, order, nmemb - 1 );
    return NULL;
  }
  return _zQuickSelect( array, nmemb, size, order, cmp, priv );
}
