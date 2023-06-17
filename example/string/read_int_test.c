#include <zeda/zeda_string.h>

int main(void)
{
  char buf[BUFSIZ];
  int bufsize;

  printf( "enter a string: " );
  if( !fgets( buf, BUFSIZ, stdin ) ) return 1;
  while( buf[0] )
  {
    char* ret = zSInt( buf, &bufsize );
    printf( "%d\n", bufsize );
  }

  int val1, val2, val3;
  printf( "enter 3 numbers: " );
  char* ret1 = zFInt(stdin, &val1);
  char* ret2 = zFInt(stdin, &val2);
  char* ret3 = zFInt(stdin, &val3);
  printf( "val[1]=%d, val[2]=%d, val[3]=%d\n", val1, val2, val3 );
  printf( "(probably the result is different from what is expected due to the specification of printf.)\n" );
  return 0;
}
