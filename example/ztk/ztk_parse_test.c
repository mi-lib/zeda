#include <zeda/zeda_ztk.h>

void reg_tf_def(ZTK *ztk)
{
  char *tag1key[] = {
    "key1", "key2", "key3", "key4",
  };
  char *tag2key[] = {
    "key1", "key2", "key3", "key_a", "key_b", "key_c",
  };
  char *tag3key[] = {
    "keyA", "keyB", "keyC", "keyD", "keyE", "key_a", "key_b", "key_c",
  };
  char *tag4key[] = {
    "keyA", "keyB", "keyC", "keyD", "keyE", "key_a", "key_b", "key_c",
  };
  ZTKDefReg( ztk, "tag1", tag1key );
  ZTKDefReg( ztk, "tag2", tag2key );
  ZTKDefReg( ztk, "tag3", tag3key );
  ZTKDefReg( ztk, "tag4", tag4key );
  ZTKDefRegKey( ztk, tag2key );
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
