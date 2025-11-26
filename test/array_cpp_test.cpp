#include <zeda/zeda.h>

zArrayClass( int_array_t, int );

void assert_array_elemsize(void)
{
  int_array_t array{ 0 };

  zAssert( C++::zArray.elemSize (invalid case), array.elemSize() == 0 );
  array.alloc( 1 );
  zAssert( C++::zArray.elemSize (valid case), array.elemSize() == sizeof(int) );
}

void assert_array_resize(void)
{
  const int size = 5;
  int_array_t array{ size };

  array.resize( -1 );
  zAssert( C++::zArray.resize (negative case), array.size == array.capacity );
  array.resize( 0 );
  zAssert( C++::zArray.resize (zero case), array.size == 0 );
  array.resize( size );
  zAssert( C++::zArray.resize (up-to-capacity case), array.size == size );
  array.resize( size*2 );
  zAssert( C++::zArray.resize (exceeded case), array.size == size );
}

void assert_array_pos_is_valid(void)
{
  int size = 5;
  int_array_t array{ size };

  size--;
  array.resize( size );
  zAssert( C++::zArray.posIsValid (negative case),  !array.posIsValid( -1 ) );
  zAssert( C++::zArray.posIsValid (zero case), array.posIsValid( 0 ) );
  zAssert( C++::zArray.posIsValid (maximum case), array.posIsValid( size-1 ) );
  zAssert( C++::zArray.posIsValid (exceeded case), !array.posIsValid( size ) );
}

void assert_array_move(void)
{
  const int size = 5;
  int_array_t array_src{ size }, array_dest;
  array_dest.move( array_src );
  zAssert( C++::zArray.move, array_src.size == 0 && array_dest.size == size );
}

void assert_array_add(void)
{
  int_array_t array{ 5 };
  array.resize( 0 );

  int val;
  val = 1; array.add( &val );
  val = 2; array.add( &val );
  val = 3; array.add( &val );
  zAssert( C++::zArray.add (up-to-capacity case), array.size == 3 && array.capacity == 5 );
  val = 4; array.add( &val );
  val = 5; array.add( &val );
  val = 6; array.add( &val );
  zAssert( C++::zArray.add (over-capacity case), array.size == 6 && array.capacity == 6 );
}

void assert_array_append(void)
{
  int_array_t array1, array2;
  const int size = 5;
  int c;
  for(int i=0; i<size; i++ ){
    c = i;        array1.add( &c );
    c = size + i; array2.add( &c );
  }
  array1.append( array2 );
  bool result = true;
  for(int i=0; i<size*2; i++ ){
    c = *array1.get( i );
    if( c != i ) result = false;
  }
  zAssert( C++::zArray.append, result );
}

void assert_array_delete(void)
{
  const int size = 5;
  int_array_t array{ size };

  for(int i=0; i<size; i++ ) array.set( i, &i );
  array._delete( 1 );
  zAssert( C++::zArray._delete, array.size == size-1 && *array.get(0) == 0 && *array.get(1) == 2 && array.capacity == size );
  array._delete( size );
  zAssert( C++::zArray._delete (invalid case), array.size == size-1 && *array.get(0) == 0 && *array.get(1) == 2 && array.capacity == size );
  array._free();
  array._delete( 0 );
  zAssert( C++::zArray._delete (empty case), array.size == 0 && array.capacity == 0 );
}

void assert_array_insert(void)
{
  const int size = 5;
  int_array_t array{ size };
  int insertvalue = 100;

  for(int i=0; i<size; i++ ) array.set( i, &i );
  array.insert( 1, &insertvalue );
  zAssert( C++::zArray.insert, array.size == size+1 && *array.tail() == 0 && *array.get(1) == insertvalue );
  array.insert( 0, &insertvalue );
  zAssert( C++::zArray.insert (insertion to tail), array.size == size+2 && *array.tail() == insertvalue );
  array.insert( array.size, &insertvalue );
  zAssert( C++::zArray.insert (insertion to head), array.size == size+3 && *array.head() == insertvalue );
  array.insert( array.size + 1, &insertvalue );
  zAssert( C++::zArray.insert (invalid case), array.size == size+3 );
}

void assert_array_clone(void)
{
  const int n = 1000;
  int_array_t array{ n }, array_clone;

  for(int i=0; i<n; i++ )
    *array.get( i ) = zRandI(-100,100);
  array_clone.clone( array );
  bool result = true;
  for(int i=0; i<n; i++ )
    if( *array.get( i ) != *array_clone.get( i ) ) result = false;
  zAssert( C++::zArray.clone, result );
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
  int_array_t array{ n };

  zArrayAlloc( &array, int, n );
  for(int i=0; i<n; i++ )
    *array.get( i ) = zRandI(-10,10);
  array.sort( cmp, NULL );
  bool result = true;
  for(int i=1; i<n; i++ )
    if( *array.get(i-1) > *array.get(i) ) result = false;
  zAssert( C++::zArray.sort, result );
}

void assert_insertsort(void)
{
  const int n = 100;
  int_array_t array{ n };
  int val;

  array.resize( 0 );
  for(int i=0; i<n; i++ ){
    val = zRandI(-10,10);
    array.insertsort( &val, cmp, NULL );
  }
  bool result = true;
  for(int i=1; i<n; i++ )
    if( *array.get(i-1) > *array.get(i) ) result = false;
  zAssert( C++::zArray.insertsort, result );
}

void assert_select(void)
{
  const int n = 1000;
  int_array_t array{ n }, array_clone;
  bool result = true;

  for(int i=0; i<n; i++ )
    *array.get( i ) = zRandI(-100,100);
  array_clone.clone( array );
  array_clone.sort( cmp, NULL );

  for(int i=0; i<n; i++ ){
    if( *array.select( i, cmp, NULL ) != *array_clone.get( i ) ) result = false;
  }
  zAssert( C++::zArray.select, result );
  zAssert( C++::zArray.select (range over case), array.select( n, cmp, NULL ) == NULL );
}

void assert_median(void)
{
  const int n_max = 100;
  int_array_t array;
  bool result1, result2, result3, result4, result5, result6;

  result1 = result2 = result3 = result4 = result5 = result6 = true;
  /* odd members cases */
  for(int n=0; n<n_max; n++ ){
    array.alloc( 2*n+1 );
    for(int i=0; i<n; i++ ) *array.get( i ) = -1;
    *array.get( n ) = 0;
    for(int i=0; i<n; i++ ) *array.get( n+i+1 ) = 1;
    result1 = *array.median( cmp, NULL ) == 0;
    for(int i=0; i<array.size; i++ ) *array.get( i ) = 0;
    result2 = *array.median( cmp, NULL ) == 0;
    for(int i=0; i<array.size; i++ ) *array.get( i ) = i;
    result3 = *array.median( cmp, NULL ) == ( array.size - 1 ) / 2;
    array._free();
  }
  /* even members cases */
  for(int n=1; n<n_max; n++ ){
    array.alloc( 2*n );
    for(int i=0; i<n; i++ ) *array.get( i ) = -1;
    for(int i=0; i<n; i++ ) *array.get( n+i ) = 1;
    result4 = *array.median( cmp, NULL ) == 1;
    for(int i=0; i<array.size-1; i++ ) *array.get( i ) = 0;
    *array.get( array.size-1 ) = 1;
    result5 = *array.median( cmp, NULL ) == 0;
    for(int i=0; i<array.size; i++ ) *array.get( i ) = i;
    result6 = *array.median( cmp, NULL ) == zArraySize(&array) / 2;
    array._free();
  }
  zAssert( C++::zArray.median (odd members case), result1 );
  zAssert( C++::zArray.median (odd members all equal case), result2 );
  zAssert( C++::zArray.median (odd members ordered case), result3 );
  zAssert( C++::zArray.median (even members case), result4 );
  zAssert( C++::zArray.median (even members all equal case), result5 );
  zAssert( C++::zArray.median (even members ordered case), result6 );
}

zArray2Class( int_array2_t, int );

void assert_array2_elemsize(void)
{
  int_array2_t array{ 0, 0 };

  zAssert( C++::zArray2.elemSize (invalid case), array.elemSize() == 0 );
  array.alloc( 1, 1 );
  zAssert( C++::zArray2.elemSize (valid case), array.elemSize() == sizeof(int) );
}

void assert_array2_resize(void)
{
  const int rowsize = 5, colsize = 3;
  int_array2_t array{ rowsize, colsize };

  /* rowsize */
  array.rowResize( -1 );
  zAssert( C++::zArray2.rowResize (negative case), array.rowsize() == array.rowcapacity() && array.colsize() == array.colcapacity() );
  array.rowResize( 0 );
  zAssert( C++::zArray2.rowResize (zero case), array.rowsize() == 0 );
  array.rowResize( rowsize );
  zAssert( C++::zArray2.rowResize (up-to-capacity case), array.rowsize() == rowsize );
  array.rowResize( rowsize*2 );
  zAssert( C++::zArray2.rowResize (exceeded case), array.rowsize() == rowsize );
  array._free();
  /* colsize */
  array.alloc( rowsize, colsize );
  array.colResize( -1 );
  zAssert( C++::zArray2.colResize (negative case), array.rowsize() == array.rowcapacity() && array.colsize() == array.colcapacity() );
  array.colResize( 0 );
  zAssert( C++::zArray2.colResize (zero case), array.colsize() == 0 );
  array.colResize( colsize );
  zAssert( C++::zArray2.colResize (up-to-capacity case), array.colsize() == colsize );
  array.colResize( colsize*2 );
  zAssert( C++::zArray2.colResize (exceeded case), array.colsize() == colsize );
  array._free();
  /* rowsize & colsize */
  array.alloc( rowsize, colsize );
  array.resize( rowsize-1, colsize+1 );
  zAssert( C++::zArray2.resize (up-to-capacity case), array.rowsize() == rowsize && array.colsize() == colsize );
  array.resize( rowsize-1, colsize-1 );
  zAssert( C++::zArray2.resize (exceeded case), array.rowsize() == rowsize-1 && array.colsize() == colsize-1 );
  array._free();
}

void assert_array2_pos_is_valid(void)
{
  const int rowsize = 5, colsize = 3;
  int_array2_t array{ rowsize, colsize };

  zAssert( C++::zArray2.posIsValid (negative case), !array.posIsValid( -1, 0 ) );
  zAssert( C++::zArray2.posIsValid (zero case), array.posIsValid( 0, 0 ) );
  zAssert( C++::zArray2.posIsValid (maximum case), array.posIsValid( rowsize-1, colsize-1 ) );
  zAssert( C++::zArray2.posIsValid (exceeded case), !array.posIsValid( rowsize, colsize ) );
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
