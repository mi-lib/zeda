#include <zeda/zeda_array.h>

typedef struct{
  double x, y, z;
} coord;

void coord_create(coord *c, double x, double y, double z)
{
  c->x = x;
  c->y = y;
  c->z = z;
}

void coord_write(coord *c)
{
  printf( "( %f, %f, %f )\n", c->x, c->y, c->z );
}

zArrayClass( coord_array_t, coord );

void delete_test(coord_array_t *arr, int pos)
{
  int i;

  printf( "<delete %d>\n", pos );
  zArrayDelete( arr, coord, pos );
  for( i=0; i<zArrayNum(arr); i++ ){
    printf( "[%02d] ", i ); coord_write( zArrayElem( arr, i ) );
  }
}

int main(void)
{
  register int i;
  coord_array_t array;
  coord c;

  zArrayAlloc( &array, coord, 0 );
  for( i=0; i<5; i++ ){
    coord_create( &c, i, 0, 0 );
    zArrayAdd( &array, coord, &c );
  }
  printf( "<original array>\n" );
  for( i=0; i<zArrayNum(&array); i++ ){
    printf( "[%02d] ", i ); coord_write( zArrayElem( &array, i ) );
  }
  delete_test( &array, 2 );
  delete_test( &array, 3 );
  delete_test( &array, 0 );
  delete_test( &array, 2 );
  delete_test( &array, 0 );
  delete_test( &array, 0 );

  zArrayFree( &array );
  return 0;
}
