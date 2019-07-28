#include <zeda/zeda.h>

#define assert_defs( type, size ) zAssert( type, sizeof(type) == size )

int main(void)
{
  assert_defs( int8_t, 1 );
  assert_defs( uint8_t, 1 );
  assert_defs( int16_t, 2 );
  assert_defs( uint16_t, 2 );
  assert_defs( int32_t, 4 );
  assert_defs( uint32_t, 4 );
  assert_defs( int64_t, 8 );
  assert_defs( uint64_t, 8 );
  assert_defs( byte, 1 );
  assert_defs( ubyte, 1 );
  assert_defs( word, 2 );
  assert_defs( uword, 2 );
  assert_defs( uint, sizeof(int) );
  assert_defs( ulong, sizeof(long) );

  zAssert( zBoolStr,
    strcmp( zBoolStr( true ), "true" ) == 0 &&
    strcmp( zBoolStr( false ), "false" ) == 0 );
  return EXIT_SUCCESS;
}
