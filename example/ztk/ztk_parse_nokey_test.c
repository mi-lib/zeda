#include <zeda/zeda_ztk.h>

void reg_tf_def(ZTK *ztk)
{
  ZTKDefRegTag( ztk, "tag1" );
  ZTKDefRegTag( ztk, "tag2" );
  ZTKDefRegTag( ztk, "tag3" );
  ZTKDefRegTag( ztk, "tag4" );
}

int main(int argc, char *argv[])
{
  ZTK ztk;

  if( argc <= 1 ) return 1;

  ZTKInit( &ztk );
  reg_tf_def( &ztk );

  eprintf("\nparsing...\n");
  ZTKParse( &ztk, argv[1] );
  eprintf("done.\n\n");

  ZTKPrint( &ztk );
  ZTKDestroy( &ztk );
  return 0;
}
