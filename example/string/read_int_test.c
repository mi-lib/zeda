#include <zeda/zeda_string.h>

int main(void)
{
  char buf[BUFSIZ];
  int val[3];

  printf( "enter a string: " );
  if( !fgets( buf, BUFSIZ, stdin ) ) return 1;
  while( *zSInt(buf, &val[0]) )
    printf( "%d\n", val[0]);

  printf( "enter 3 numbers: " );
  if( !zFInt( stdin, &val[0] ) );
  if( !zFInt( stdin, &val[1] ) );
  if( !zFInt( stdin, &val[2] ) );
  printf( "val[0]=%d, val[1]=%d, val[2]=%d\n", val[0], val[1], val[2] );
  return 0;
}
