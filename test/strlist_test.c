#include <zeda/zeda.h>

int main(void)
{
  zStrList list;
  char *str1, *str2, *str3;

  zListInit( &list );
  zStrListInsert( &list, "aaa", true );
  zStrListInsert( &list, "bbb", true );
  zStrListInsert( &list, "ccc", true );
  zStrListInsert( &list, "ddd", true );
  zStrListInsert( &list, "eee", true );
  zAssert( zStrListInsert, zListNum(&list) == 5 );

  zStrListGetPtr( &list, 3, &str1, &str2, &str3 );
  zAssert( zStrListGetPtr, !strcmp( str1, "aaa" ) && !strcmp( str2, "bbb" ) & !strcmp( str3, "ccc" ) );

  zStrListDestroy( &list, true );
  return 0;
}
