#include <zeda/zeda.h>

zArrayClass( int_array_t, int );

void assert_array_elemsize(void)
{
  int_array_t array;
  bool result1, result2;

  zArrayAlloc( &array, int, 0 );
  result1 = zArrayElemSize(&array) == 0;
  zArrayAlloc( &array, int, 1 );
  result2 = zArrayElemSize(&array) == sizeof(int);
  zArrayFree( &array );
  zAssert( zArrayElemSize (invalid case), result1 );
  zAssert( zArrayElemSize (valid case), result2 );
}

void assert_array_resize(void)
{
  int_array_t array;
  const int size = 5;
  bool result1, result2, result3, result4;

  zArrayAlloc( &array, int, size );
  zArrayResize( &array, -1 );
  result1 = zArraySize(&array) == zArrayCapacity(&array);
  zArrayResize( &array, 0 );
  result2 = zArraySize(&array) == 0;
  zArrayResize( &array, size );
  result3 = zArraySize(&array) == size;
  zArrayResize( &array, size*2 );
  result4 = zArraySize(&array) == size;
  zArrayFree( &array );
  zAssert( zArrayResize (negative case), result1 );
  zAssert( zArrayResize (zero case), result2 );
  zAssert( zArrayResize (up-to-capacity case), result3 );
  zAssert( zArrayResize (exceeded case), result4 );
}

void assert_array_pos_is_valid(void)
{
  int_array_t array;
  int size = 5;
  bool result1, result2, result3, result4;

  zArrayAlloc( &array, int, size );
  size--;
  zArrayResize( &array, size );
  result1 = !zArrayPosIsValid( &array, -1 );
  result2 = zArrayPosIsValid( &array, 0 );
  result3 = zArrayPosIsValid( &array, size-1 );
  result4 = !zArrayPosIsValid( &array, size );
  zArrayFree( &array );
  zAssert( zArrayPosIsValid (negative case), result1 );
  zAssert( zArrayPosIsValid (zero case), result2 );
  zAssert( zArrayPosIsValid (maximum case), result3 );
  zAssert( zArrayPosIsValid (exceeded case), result4 );
}

void assert_array_move(void)
{
  int_array_t array_src, array_dest;
  const int size = 5;
  bool result;

  zArrayAlloc( &array_src, int, size );
  zArrayInit( &array_dest );
  zArrayMove( &array_src, &array_dest );
  result = zArraySize(&array_src) == 0 && zArraySize(&array_dest) == size;
  zArrayFree( &array_src );
  zArrayFree( &array_dest );
  zAssert( zArrayMove, result );
}

void assert_array_add(void)
{
  int_array_t array;
  int val;
  bool result1, result2;

  zArrayAlloc( &array, int, 5 );
  zArrayResize( &array, 0 );
  val = 1; zArrayAdd( &array, int, &val );
  val = 2; zArrayAdd( &array, int, &val );
  val = 3; zArrayAdd( &array, int, &val );
  result1 = zArraySize(&array) == 3 && zArrayCapacity(&array) == 5;
  val = 4; zArrayAdd( &array, int, &val );
  val = 5; zArrayAdd( &array, int, &val );
  val = 6; zArrayAdd( &array, int, &val );
  result2 = zArraySize(&array) == 6 && zArrayCapacity(&array) == 6;
  zArrayFree( &array );
  zAssert( zArrayAdd (up-to-capacity case), result1 );
  zAssert( zArrayAdd (over-capacity case), result2 );
}

void assert_array_append(void)
{
  int_array_t array1, array2;
  const int size = 5;
  int c, i;
  bool result = true;

  zArrayAlloc( &array1, int, 0 );
  zArrayAlloc( &array2, int, 0 );
  for( i=0; i<size; i++ ){
    c = i;
    zArrayAdd( &array1, int, &c );
    c = size + i;
    zArrayAdd( &array2, int, &c );
  }
  zArrayAppend( &array1, &array2, int );
  for( result=true, i=0; i<size*2; i++ ){
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
  const int size = 5;
  int i;
  bool result1, result2, result3;

  zArrayAlloc( &array, int, size );
  for( i=0; i<size; i++ ){
    *zArrayElemNC(&array,i) = i;
  }
  zArrayDelete( &array, int, 1 );
  result1 = zArraySize(&array) == size-1 && *zArrayElemNC(&array,0) == 0 && *zArrayElemNC(&array,1) == 2 && zArrayCapacity(&array) == size;
  zArrayDelete( &array, int, size );
  result2 = zArraySize(&array) == size-1 && *zArrayElemNC(&array,0) == 0 && *zArrayElemNC(&array,1) == 2 && zArrayCapacity(&array) == size;
  zArrayFree( &array );
  zArrayDelete( &array, int, 0 );
  result3 = zArraySize(&array) == 0 && zArrayCapacity(&array) == 0;
  zArrayFree( &array );

  zAssert( zArrayDelete, result1 );
  zAssert( zArrayDelete (invalid case), result2 );
  zAssert( zArrayDelete (empty case), result3 );
}

void assert_array_insert(void)
{
  int_array_t array;
  const int size = 5;
  int i, insertvalue = 100;

  zArrayAlloc( &array, int, size );
  for( i=0; i<size; i++ ){
    *zArrayElemNC(&array,i) = i;
  }
  zArrayInsert( &array, int, 1, &insertvalue );
  zAssert( zArrayInsert, zArraySize(&array) == size+1 && *zArrayTail(&array) == 0 && *zArrayElemNC(&array,1) == insertvalue );
  zArrayInsert( &array, int, 0, &insertvalue );
  zAssert( zArrayInsert (insertion to tail), zArraySize(&array) == size+2 && *zArrayTail(&array) == insertvalue );
  zArrayInsert( &array, int, zArraySize(&array), &insertvalue );
  zAssert( zArrayInsert (insertion to head), zArraySize(&array) == size+3 && *zArrayHead(&array) == insertvalue );
  zArrayInsert( &array, int, zArraySize(&array)+1, &insertvalue );
  zAssert( zArrayInsert (invalid case), zArraySize(&array) == size+3 );
  zArrayFree( &array );
}

void assert_array_clone(void)
{
  const int n = 1000;
  int i;
  int_array_t array, array_clone;
  int val;
  bool result = true;

  zArrayAlloc( &array, int, n );
  for( i=0; i<n; i++ ){
    val = zRandI(-100,100);
    zArraySetElemNC( &array, i, &val );
  }
  zArrayClone( int, &array, &array_clone );
  for( i=0; i<n; i++ ){
    if( *(int *)zArrayElemNC(&array,i) != *(int *)zArrayElemNC(&array_clone,i) ) result = false;
  }
  zArrayFree( &array );
  zArrayFree( &array_clone );
  zAssert( zArrayClone, result );
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
  const int n = 10;
  int i;
  int_array_t array;
  int val;
  bool result = true;

  zArrayAlloc( &array, int, n );
  zArrayResize( &array, 0 );
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArrayInsertSort( &array, &val, cmp, NULL );
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
  zArrayClone( int, &array, &array_clone );
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

zArray2Class( int_array2_t, int );

void assert_array2_elemsize(void)
{
  int_array2_t array;
  bool result1, result2;

  zArray2Alloc( &array, int, 0, 0 );
  result1 = zArrayElemSize(&array) == 0;
  zArray2Alloc( &array, int, 1, 1 );
  result2 = zArrayElemSize(&array) == sizeof(int);
  zArray2Free( &array );
  zAssert( zArray2ElemSize (invalid case), result1 );
  zAssert( zArray2ElemSize (valid case), result2 );
}

void assert_array2_resize(void)
{
  int_array2_t array;
  const int rowsize = 5, colsize = 3;
  bool result1, result2, result3, result4, result5;

  /* rowsize */
  zArray2Alloc( &array, int, rowsize, colsize );
  zArray2RowResize( &array, -1 );
  result1 = zArray2RowSize(&array) == zArray2RowCapacity(&array) &&
            zArray2ColSize(&array) == zArray2ColCapacity(&array);
  zArray2RowResize( &array, 0 );
  result2 = zArray2RowSize(&array) == 0;
  zArray2RowResize( &array, rowsize );
  result3 = zArray2RowSize(&array) == rowsize;
  zArray2RowResize( &array, rowsize*2 );
  result4 = zArray2RowSize(&array) == rowsize;
  zArray2RowResize( &array, 0 );
  zArray2ResetRowSize( &array );
  result5 = zArray2RowSize(&array) == zArray2RowCapacity(&array);
  zArray2Free( &array );
  zAssert( zArray2RowResize (negative case), result1 );
  zAssert( zArray2RowResize (zero case), result2 );
  zAssert( zArray2RowResize (up-to-capacity case), result3 );
  zAssert( zArray2RowResize (exceeded case), result4 );
  zAssert( zArray2ResetRowSize, result5 );
  /* colsize */
  zArray2Alloc( &array, int, rowsize, colsize );
  zArray2ColResize( &array, -1 );
  result1 = zArray2RowSize(&array) == zArray2RowCapacity(&array) &&
            zArray2ColSize(&array) == zArray2ColCapacity(&array);
  zArray2ColResize( &array, 0 );
  result2 = zArray2ColSize(&array) == 0;
  zArray2ColResize( &array, colsize );
  result3 = zArray2ColSize(&array) == colsize;
  zArray2ColResize( &array, colsize*2 );
  result4 = zArray2ColSize(&array) == colsize;
  zArray2ColResize( &array, 0 );
  zArray2ResetColSize( &array );
  result5 = zArray2ColSize(&array) == zArray2ColCapacity(&array);
  zArray2Free( &array );
  zAssert( zArray2ColResize (negative case), result1 );
  zAssert( zArray2ColResize (zero case), result2 );
  zAssert( zArray2ColResize (up-to-capacity case), result3 );
  zAssert( zArray2ColResize (exceeded case), result4 );
  zAssert( zArray2ResetColSize, result5 );
  /* rowsize & colsize */
  zArray2Alloc( &array, int, rowsize, colsize );
  zArray2Resize( &array, rowsize-1, colsize+1 );
  result1 = zArray2RowSize(&array) == rowsize && zArray2ColSize(&array) == colsize;
  zArray2Resize( &array, rowsize-1, colsize-1 );
  result2 = zArray2RowSize(&array) == rowsize-1 && zArray2ColSize(&array) == colsize-1;
  zArray2Resize( &array, 0, 0 );
  zArray2ResetSize( &array );
  result3 = zArray2RowSize(&array) == zArray2RowCapacity(&array) &&
            zArray2ColSize(&array) == zArray2ColCapacity(&array);
  zArray2Free( &array );
  zAssert( zArray2Resize (up-to-capacity case), result1 );
  zAssert( zArray2Resize (exceeded case), result2 );
  zAssert( zArray2ResetSize, result3 );
}

void assert_array2_pos_is_valid(void)
{
  int_array2_t array;
  int rowsize = 5, colsize = 3;
  bool result1, result2, result3, result4;

  zArray2Alloc( &array, int, rowsize, colsize );
  result1 = !zArray2PosIsValid( &array, -1, 0 );
  result2 = zArray2PosIsValid( &array, 0, 0 );
  result3 = zArray2PosIsValid( &array, rowsize-1, colsize-1 );
  result4 = !zArray2PosIsValid( &array, rowsize, colsize );
  zArray2Free( &array );
  zAssert( zArray2PosIsValid (negative case), result1 );
  zAssert( zArray2PosIsValid (zero case), result2 );
  zAssert( zArray2PosIsValid (maximum case), result3 );
  zAssert( zArray2PosIsValid (exceeded case), result4 );
}

int main(void)
{
  zRandInit();
  assert_array_elemsize();
  assert_array_resize();
  assert_array_pos_is_valid();
  assert_array_move();
  assert_array_add();
  assert_array_append();
  assert_array_delete();
  assert_array_insert();
  assert_array_clone();
  assert_quicksort();
  assert_insertsort();
  assert_select();
  assert_median();
  assert_array2_elemsize();
  assert_array2_resize();
  assert_array2_pos_is_valid();
  return EXIT_SUCCESS;
}
