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

void insert(coord_array_t *arr, coord *c, int pos)
{
  int i;

  printf( "<inserted array at %d>\n", pos );
  zArrayInsert( arr, coord, pos, c );
  for( i=0; i<zArrayNum(arr); i++ ){
    printf( "[%02d] ", i ); coord_write( zArrayElemNC( arr, i ) );
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
    printf( "[%02d] ", i ); coord_write( zArrayElemNC( &array, i ) );
  }

  coord_create( &c, 9, 9, 9 ); insert( &array, &c, 2 );
  coord_create( &c, 8, 8, 8 ); insert( &array, &c, 4 );
  coord_create( &c,-7,-7,-7 ); insert( &array, &c, 7 );
  coord_create( &c,99,99,99 ); insert( &array, &c, 100 );

  zArrayFree( &array );
  return 0;
}
