#include <zeda/zeda.h>

zArrayClass( int_array_t, int );

#define SIZE 5

void assert_array_append(void)
{
  int_array_t array1, array2;
  int c, i;
  bool result = true;

  zArrayAlloc( &array1, int, 0 );
  zArrayAlloc( &array2, int, 0 );
  for( i=0; i<SIZE; i++ ){
    c = i;
    zArrayAdd( &array1, int, &c );
    c = SIZE + i;
    zArrayAdd( &array2, int, &c );
  }
  zArrayAppend( &array1, &array2, int );
  for( result=true, i=0; i<SIZE*2; i++ ){
    c = *zArrayElemNC(&array1,i);
    if( c != i ) result = false;
  }
  zArrayFree( &array1 );
  zArrayFree( &array2 );
  zAssert( zArrayAppend, result );
}

void assert_array_delete(void)
{
  int_array_t array;
  int i;

  zArrayAlloc( &array, int, SIZE );
  for( i=0; i<SIZE; i++ ){
    *zArrayElemNC(&array,i) = i;
  }
  zArrayDelete( &array, int, 1 );
  zAssert( zArrayDelete, zArraySize(&array) == SIZE-1 && *zArrayElemNC(&array,0) == 0 && *zArrayElemNC(&array,1) == 2 );
  zArrayDelete( &array, int, SIZE );
  zAssert( zArrayDelete (invalid case), zArraySize(&array) == SIZE-1 && *zArrayElemNC(&array,0) == 0 && *zArrayElemNC(&array,1) == 2 );
  zArrayFree( &array );
  zArrayDelete( &array, int, 0 );
  zAssert( zArrayDelete (empty case), zArraySize(&array) == 0 );
}

void assert_array_insert(void)
{
  int_array_t array;
  int i, insertvalue = 100;

  zArrayAlloc( &array, int, SIZE );
  for( i=0; i<SIZE; i++ ){
    *zArrayElemNC(&array,i) = i;
  }
  zArrayInsert( &array, int, 1, &insertvalue );
  zAssert( zArrayInsert, zArraySize(&array) == SIZE+1 && *zArrayTail(&array) == 0 && *zArrayElemNC(&array,1) == insertvalue );
  zArrayInsert( &array, int, 0, &insertvalue );
  zAssert( zArrayInsert (insertion to tail), zArraySize(&array) == SIZE+2 && *zArrayTail(&array) == insertvalue );
  zArrayInsert( &array, int, zArraySize(&array), &insertvalue );
  zAssert( zArrayInsert (insertion to head), zArraySize(&array) == SIZE+3 && *zArrayHead(&array) == insertvalue );
  zArrayInsert( &array, int, zArraySize(&array)+1, &insertvalue );
  zAssert( zArrayInsert (invalid case), zArraySize(&array) == SIZE+3 );
  zArrayFree( &array );
}

int cmp(void *v1, void *v2, void *dummy)
{
  if( *(int*)v1 > *(int*)v2 ) return 1;
  if( *(int*)v1 < *(int*)v2 ) return -1;
  return 0;
}

void assert_quicksort(void)
{
  const int n = 100;
  int i;
  int_array_t array;
  int val;
  bool result = true;

  zArrayAlloc( &array, int, n );
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArraySetElemNC( &array, i, &val );
  }
  zArrayQuickSort( &array, cmp, NULL );
  for( i=1; i<n; i++ )
    if( *zArrayElemNC(&array,i-1) > *zArrayElemNC(&array,i) ) result = false;
  zArrayFree( &array );
  zAssert( zArrayQuickSort, result );
}

void assert_insertsort(void)
{
  const int n = 100;
  int i;
  int_array_t array;
  int val;
  bool result = true;

  zArrayAlloc( &array, int, n );
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArrayInsertSort( &array, &val, i, cmp, NULL );
  }
  for( i=1; i<n; i++ )
    if( *zArrayElemNC(&array,i-1) > *zArrayElemNC(&array,i) ) result = false;
  zArrayFree( &array );
  zAssert( zArrayInsertSort, result );
}

void assert_select(void)
{
  const int n = 1000;
  int i;
  int_array_t array, array_clone;
  int val;
  bool result1 = true, result2;

  zArrayAlloc( &array, int, n );
  for( i=0; i<n; i++ ){
    val = zRandI(-100,100);
    zArraySetElemNC( &array, i, &val );
  }
  zArrayClone( &array, &array_clone );
  zArrayQuickSort( &array_clone, cmp, NULL );

  for( i=0; i<n; i++ ){
    val = *(int *)zArraySelect( &array, i, cmp, NULL );
    if( val != *zArrayElemNC( &array_clone, i ) ) result1 = false;
  }
  result2 = zArraySelect( &array, n, cmp, NULL ) == NULL;

  zArrayFree( &array );
  zArrayFree( &array_clone );
  zAssert( zArraySelect, result1 );
  zAssert( zArraySelect (range over case), result2 );
}

void assert_median(void)
{
  const int n_max = 100;
  int i, n;
  int_array_t array;
  int val;
  bool result1, result2, result3, result4, result5, result6;

  result1 = result2 = result3 = result4 = result5 = result6 = true;
  /* odd members cases */
  for( n=0; n<n_max; n++ ){
    zArrayAlloc( &array, int, 2*n+1 );
    for( i=0; i<n; i++ ){
      val = -1;
      zArraySetElemNC( &array, i, &val );
    }
    val = 0;
    zArraySetElemNC( &array, n, &val );
    for( i=0; i<n; i++ ){
      val = 1;
      zArraySetElemNC( &array, n+i+1, &val );
    }
    result1 = *(int *)zArrayMedian( &array, cmp, NULL ) == 0;
    for( i=0; i<zArraySize(&array); i++ ){
      val = 0;
      zArraySetElemNC( &array, i, &val );
    }
    result2 = *(int *)zArrayMedian( &array, cmp, NULL ) == 0;
    for( i=0; i<zArraySize(&array); i++ ){
      val = i;
      zArraySetElemNC( &array, i, &val );
    }
    result3 = *(int *)zArrayMedian( &array, cmp, NULL ) == ( zArraySize(&array) - 1 ) / 2;
    zArrayFree( &array );
  }
  /* even members cases */
  for( n=1; n<n_max; n++ ){
    zArrayAlloc( &array, int, 2*n );
    for( i=0; i<n; i++ ){
      val = -1;
      zArraySetElemNC( &array, i, &val );
    }
    for( i=0; i<n; i++ ){
      val = 1;
      zArraySetElemNC( &array, n+i, &val );
    }
    result4 = *(int *)zArrayMedian( &array, cmp, NULL ) == 1;
    for( i=0; i<zArraySize(&array)-1; i++ ){
      val = 0;
      zArraySetElemNC( &array, i, &val );
    }
    val = 1;
    zArraySetElemNC( &array, i, &val );
    result5 = *(int *)zArrayMedian( &array, cmp, NULL ) == 0;
    for( i=0; i<zArraySize(&array); i++ ){
      val = i;
      zArraySetElemNC( &array, i, &val );
    }
    result6 = *(int *)zArrayMedian( &array, cmp, NULL ) == zArraySize(&array) / 2;
    zArrayFree( &array );
  }
  zAssert( zArrayMedian (odd members case), result1 );
  zAssert( zArrayMedian (odd members all equal case), result2 );
  zAssert( zArrayMedian (odd members ordered case), result3 );
  zAssert( zArrayMedian (even members case), result4 );
  zAssert( zArrayMedian (even members all equal case), result5 );
  zAssert( zArrayMedian (even members ordered case), result6 );
}


int main(void)
{
  zRandInit();
  assert_array_append();
  assert_array_delete();
  assert_array_insert();
  assert_quicksort();
  assert_insertsort();
  assert_select();
  assert_median();
  return EXIT_SUCCESS;
}
