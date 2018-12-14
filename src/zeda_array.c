/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_array - array operation.
 */

#include <zeda/zeda_array.h>

/* quick sort for static list structure */

/* zQuickSort
 * - quick sort for an array of pointers.
 */
void zQuickSort(void *array, size_t nmemb, size_t size, int (* cmp)(void*,void*,void*), void *priv)
{
  byte *base, *pivot;
  size_t i, t, h, m;

  if( nmemb <= 1 ) return;
  base = array;
  for( t=0, h=nmemb-1, m=(t+h)/2; ; t++, h-- ){
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
  if( t == h ) t++;
  if( t > 1 )
    zQuickSort( base, t, size, cmp, priv );
  if( ( nmemb -= t ) > 1 )
    zQuickSort( base+size*t, nmemb, size, cmp, priv );
}
