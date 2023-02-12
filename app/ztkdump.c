#include <zeda/zeda_ztk.h>

int main(int argc, char *argv[])
{
  ZTK ztk;

  if( argc <= 1 ){
    eprintf( "Usage: %s <.ztk file>\n", argv[0] );
    return 0;
  }
  ZTKInit( &ztk );
  ZTKParse( &ztk, argv[1] );
  ZTKPrint( &ztk );
  ZTKDestroy( &ztk );
  return 0;
}
