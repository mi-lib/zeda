#include <zeda/zeda_ztk.h>

int main(void)
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKAddTag( &ztk, "tag1" );
    ZTKAddKey( &ztk, "key1" );
      ZTKAddVal( &ztk, "val1" );
      ZTKAddVal( &ztk, "val2" );
      ZTKAddVal( &ztk, "val3" );
    ZTKAddKey( &ztk, "key2" );
      ZTKAddInt( &ztk, 1 );
      ZTKAddInt( &ztk, 2 );
      ZTKAddInt( &ztk, 3 );
  ZTKAddTag( &ztk, "tag2" );
    ZTKAddKey( &ztk, "key3" );
      ZTKAddVal( &ztk, "val4" );
      ZTKAddVal( &ztk, "val5" );
      ZTKAddVal( &ztk, "val6" );
  ZTKAddTag( &ztk, "tag3" );
    ZTKAddKey( &ztk, "key4" );
      ZTKAddDouble( &ztk, 3.14159 );
      ZTKAddDouble( &ztk, 2.2360679 );
      ZTKAddDouble( &ztk, 1.23456789 );
    ZTKAddKey( &ztk, "key5" );
      ZTKAddVal( &ztk, "val7" );
      ZTKAddVal( &ztk, "val8" );
      ZTKAddVal( &ztk, "val9" );
  ZTKPrint( &ztk );

  ZTKDestroy( &ztk );
  return 0;
}
