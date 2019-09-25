#include <zeda/zeda_misc.h>

int main(int argc, char *argv[])
{
  char buf[BUFSIZ];

  if( argc < 2 ) return 1;
  printf( "%s\n", itoa_ordinal( atoi(argv[1]), buf, BUFSIZ ) );
  return 0;
}
