#include <zeda/zeda.h>

int main(void)
{
  zStrList list;
  char *str1, *str2, *str3;

  zListInit( &list );
  zStrListAdd( &list, "aaa" );
  zStrListAdd( &list, "bbb" );
  zStrListAdd( &list, "ccc" );
  zStrListAdd( &list, "ddd" );
  zStrListAdd( &list, "eee" );
  zAssert( zStrListAdd, zListNum(&list) == 5 );

  zAssert( zStrListFind, zStrListFind(&list,"ccc") && !zStrListFind(&list,"fff") );

  zStrListGetPtr( &list, 3, &str1, &str2, &str3 );
  zAssert( zStrListGetPtr, !strcmp( str1, "aaa" ) && !strcmp( str2, "bbb" ) & !strcmp( str3, "ccc" ) );

  zStrListDestroy( &list );
  return 0;
}
