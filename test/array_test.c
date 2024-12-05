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

int assert_quicksort(int n)
{
  int i;
  int_array_t array;
  int val;

  zArrayAlloc( &array, int, n );
  zRandInit();
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArraySetElemNC( &array, i, &val );
  }
  zArrayQuickSort( &array, cmp, NULL );
  for( i=1; i<n; i++ )
    if( *zArrayElemNC(&array,i-1) > *zArrayElemNC(&array,i) ) return 0;
  zArrayFree( &array );
  return 1;
}

int assert_insertsort(int n)
{
  int i;
  int_array_t array;
  int val;

  zArrayAlloc( &array, int, n );
  zRandInit();
  for( i=0; i<n; i++ ){
    val = zRandI(-10,10);
    zArrayInsertSort( &array, &val, i, cmp, NULL );
  }
  for( i=1; i<n; i++ )
    if( *zArrayElemNC(&array,i-1) > *zArrayElemNC(&array,i) ) return 0;
  zArrayFree( &array );
  return 1;
}

#define N 100

int main(void)
{
  assert_array_append();
  assert_array_delete();
  assert_array_insert();

  zAssert( zArrayQuickSort, assert_quicksort( N ) );
  zAssert( zAssertInsertSort, assert_insertsort( N ) );

  return EXIT_SUCCESS;
}
