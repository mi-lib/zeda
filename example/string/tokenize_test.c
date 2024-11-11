#include <zeda/zeda_string.h>

void test_ftoken(void)
{
  FILE *fp;
  char token[BUFSIZ];

  printf( "(tokens from a file)\n" );
  fp = fopen( "string_test.txt", "r" );
  while( !feof( fp ) ){
    if( zFToken( fp, token, BUFSIZ ) )
      printf( "token -> |%s|\n", token );
  }
  fclose( fp );
}

void test_stoken(void)
{
  char string[BUFSIZ];
  char token[BUFSIZ];

  printf( "\n(tokens from a string)\n" );
  strcpy( string, "test1 test2\ttest3|\"test4 test5\";test6:\'test7 test8 " );
  while( *string ){
    zSToken( string, token, BUFSIZ );
    printf( "token -> |%s|\n", token );
  }
}

int main(void)
{
  test_ftoken();
  test_stoken();
  return EXIT_SUCCESS;
}
