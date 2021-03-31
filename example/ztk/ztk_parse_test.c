#include <zeda/zeda_ztk.h>

int main(int argc, char *argv[])
{
  ZTK ztk;

  if( argc <= 1 ) return 1;

  ZTKInit( &ztk );

  eprintf("\nparsing...\n");
  ZTKParse( &ztk, argv[1] );
  eprintf("done.\n\n");

  ZTKPrint( &ztk );
  ZTKDestroy( &ztk );
  return 0;
}
