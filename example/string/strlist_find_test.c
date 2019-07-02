#include <zeda/zeda_strlist.h>

void find_test(zStrList *strlist, char *str)
{
  printf( "string %s ", str );
  if( zStrListFindStr( strlist, str ) ){
    printf( "found " );
  } else
    printf( "not found " );
  printf( "in the list.\n" );
}

int main(int argc, char *argv[])
{
  zStrList strlist;

  zListInit( &strlist );
  zStrListInsert( &strlist, "hoo" );
  zStrListInsert( &strlist, "baa" );

  find_test( &strlist, "hoo" );
  find_test( &strlist, "baa" );
  find_test( &strlist, "hee" );
  find_test( &strlist, NULL );

  zStrListDestroy( &strlist );
  return 0;
}
