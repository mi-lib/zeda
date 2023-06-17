#include <zeda/zeda_ztk.h>

int main(int argc, char *argv[])
{
  ZTK ztk;

  if( argc == 1 ) return 0;
  ZTKInit( &ztk );
  ZTKParse( &ztk, argv[1] );

  ZTKPrint( &ztk );

  printf( ">> count\n" );
  if( ZTKTagRewind( &ztk ) ) do{
    printf( "[%s]\n", ZTKTag(&ztk) );
    if( ZTKKeyRewind(&ztk) ) do{
      printf( "key=%s, number of values=%d\n", ZTKKey(&ztk), ZTKKeyFieldSize(&ztk) );
    } while( ZTKKeyNext(&ztk) );
  } while( ZTKTagNext(&ztk) );

  ZTKDestroy( &ztk );
  return 0;
}
