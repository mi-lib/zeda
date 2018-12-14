#include <zeda/zeda.h>

bool assert_parse_int_range(char *expr, int i1, int i2)
{
  int from, to;

  zParseIntRange( expr, &from, &to );
  return from == i1 && to == i2 ? true : false;
}

int main(void)
{
  zAssert( zParseIntRange, assert_parse_int_range( "3", 3, 3 ) &&
                           assert_parse_int_range( "3-7", 3, 7 ) &&
                           assert_parse_int_range( "7-3", 7, 3 ) &&
                           assert_parse_int_range( "-7", -1, 7 ) &&
                           assert_parse_int_range( "3-", 3, -1 ) &&
                           assert_parse_int_range( "-", -1, -1 ) &&
                           assert_parse_int_range( "hoge1-hoge2", 0, 0 ) &&
                           assert_parse_int_range( "1hoge-2hoge", 1, 2 ) );
  return EXIT_SUCCESS;
}
