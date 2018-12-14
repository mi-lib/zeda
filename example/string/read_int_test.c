#include <zeda/zeda_string.h>

int main(void)
{
  char buf[BUFSIZ];

  printf( "enter a string: " );
  if( !fgets( buf, BUFSIZ, stdin ) ) return 1;
  while( buf[0] )
    printf( "%d\n", zSInt( buf ) );

  printf( "enter 3 numbers: " );
  printf( "val[1]=%d, val[2]=%d, val[3]=%d\n", zFInt(stdin), zFInt(stdin), zFInt(stdin) );
  printf( "(probably the result is different from what is expected due to the specification of printf.)\n" );
  return 0;
}
