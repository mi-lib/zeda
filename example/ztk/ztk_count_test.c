#include <zeda/zeda_ztk.h>

void count_tag(ZTK *ztk, char *tag)
{
  printf( "tag: %s ... %d\n", tag, ZTKCountTag( ztk, tag ) );
}

void count_key(ZTK *ztk, char *key)
{
  printf( "(tag) %s: %s ... %d\n", ZTKTag(ztk), key, ZTKCountKey( ztk, key ) );
}

int main(int argc, char *argv[])
{
  ZTK ztk;
  char *key[] = {
    "key1", "key2", "key3",
  };

  if( argc == 1 ) return 0;
  ZTKInit( &ztk );
  ZTKDefReg( &ztk, "tag1", key );
  ZTKDefReg( &ztk, "tag2", key );
  ZTKParse( &ztk, argv[1] );

  ZTKPrint( &ztk );

  count_tag( &ztk, "tag1" );
  count_tag( &ztk, "tag2" );
  count_tag( &ztk, "tag3" );

  if( ZTKTagRewind( &ztk ) ) do{
    count_key( &ztk, "key1" );
    count_key( &ztk, "key2" );
    count_key( &ztk, "key3" );
    count_key( &ztk, "key4" );
  } while( ZTKTagNext(&ztk) );

  ZTKDestroy( &ztk );
  return 0;
}
