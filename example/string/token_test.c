#include <zeda/zeda_string.h>

int main(int argc, char *argv[])
{
  char tkn[BUFSIZ];
  char str[] = "token1\tTOKEN2 \"separated token 3\"    token4\n";
  char expstr[] = "a=1, aa=2.0, XYZ=-1.0e-16";
  size_t size;
  FILE *fp;

  size = argc > 1 ? atoi( argv[1] ) : BUFSIZ;
  printf( "[file]\n" );
  fp = fopen( "test.txt", "r" );
  while( 1 ){
    if( !zFSkipDefaultComment( fp ) ||
        !zFToken( fp, tkn, size ) ) break;
    printf( "token-> %s\n", tkn );
  }
  fclose( fp );

  printf( "[string]\n" );
  while( zSToken( str, tkn, size ) )
    printf( "token-> %s\n", tkn );

  printf( "[expression string]\n" );
  while( zSToken( expstr, tkn, size ) )
    printf( "token-> %s\n", tkn );
  return 0;
}
