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
      ZTKAddVal( &ztk, "val4" );
      ZTKAddVal( &ztk, "val5" );
      ZTKAddVal( &ztk, "val6" );
  ZTKAddTag( &ztk, "tag2" );
    ZTKAddKey( &ztk, "key3" );
      ZTKAddVal( &ztk, "val7" );
      ZTKAddVal( &ztk, "val8" );
      ZTKAddVal( &ztk, "val9" );
  ZTKAddTag( &ztk, "tag3" );
    ZTKAddKey( &ztk, "key4" );
      ZTKAddVal( &ztk, "val10" );
      ZTKAddVal( &ztk, "val11" );
      ZTKAddVal( &ztk, "val12" );
    ZTKAddKey( &ztk, "key5" );
      ZTKAddVal( &ztk, "val13" );
      ZTKAddVal( &ztk, "val14" );
      ZTKAddVal( &ztk, "val15" );
  ZTKPrint( &ztk );

  ZTKDestroy( &ztk );
  return 0;
}
