#include <zeda/zeda_array.h>

#define NUM 10

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

int main(void)
{
  register int i;
  coord_array_t array1, array2;
  coord c;

  zArrayAlloc( &array1, coord, 0 );
  zArrayAlloc( &array2, coord, 0 );
  for( i=0; i<NUM; i++ ){
    coord_create( &c, i, i, i );
    zArrayAdd( &array1, coord, &c );
    coord_create( &c, 2*NUM-i, 2*NUM-i, 2*NUM-i );
    zArrayAdd( &array2, coord, &c );
  }
  printf( "<array1>\n" );
  for( i=0; i<NUM; i++ )
    coord_write( zArrayElem( &array1, i ) );
  printf( "<array2>\n" );
  for( i=0; i<NUM; i++ )
    coord_write( zArrayElem( &array2, i ) );

  zArrayAppend( &array1, &array2, coord );
  printf( "<array1+2>\n" );
  for( i=0; i<NUM*2; i++ )
    coord_write( zArrayElem( &array1, i ) );

  zArrayFree( &array1 );
  zArrayFree( &array2 );
  return 0;
}
