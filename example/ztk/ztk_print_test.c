/* how to use ZTK parser and evaluator for arbitrary number of fields */
#include <zeda/zeda_array.h>
#include <zeda/zeda_ztk.h>

zArrayClass( int_array_t, int );
zArrayClass( tag_array_t, int_array_t );

void tag_array_free(tag_array_t *array)
{
  int i;

  for( i=0; i<zArraySize(array); i++ )
    zArrayFree( zArrayElemNC(array,i) );
  zArrayFree( array );
}

void *eval_key(void *obj, int i, void *arg, ZTK *ztk)
{
  int val;

  val = ZTKInt(ztk);
  printf( " reading key (%d/%d) ... value=%d ", i, zArraySize((int_array_t*)obj), val );
  zArraySetElemNC( (int_array_t*)obj, i, &val );
  printf( "set.\n" );
  return obj;
}

bool print_key(FILE*fp, int i, void *obj)
{
  fprintf( fp, "%d\n", *zArrayElemNC((int_array_t*)obj,i) );
  return true;
}

ZTKPrp ztk_prp_key[] = {
  { "key", -1, eval_key, print_key },
};

void *eval_tag(void *obj, int i, void *arg, ZTK *ztk)
{
  int_array_t *array;

  printf( "reading tag (%d/%d) ...\n", i, zArraySize((tag_array_t*)obj) );
  array = zArrayElemNC((tag_array_t*)obj,i);
  zArrayAlloc( array, int, ZTKCountKey( ztk, "key" ) );
  printf( " tag #%d has %d key fields\n", i, zArraySize(array) );
  return _ZTKEvalKey( array, NULL, ztk, ztk_prp_key );
}

bool print_tag(FILE*fp, int i, void *obj)
{
  ZTKPrp *prp;
  int size;
  int_array_t *array;

  size = _ZTKPrpNum( ztk_prp_key );
  prp = ZTKPrpDup( ztk_prp_key, size );
  array = zArrayElemNC((tag_array_t*)obj,i);
  ZTKPrpSetNum( prp, size, "key", zArraySize(array) );
  ZTKPrpKeyFPrint( fp, array, prp, size );
  fprintf( fp, "\n");
  free( prp );
  return true;
}

ZTKPrp ztk_prp_tag[] = {
  { "tag", -1, eval_tag, print_tag },
};

void test_eval_and_print(ZTK *ztk)
{
  tag_array_t array;
  ZTKPrp *prp;
  int size;

  size = _ZTKPrpNum( ztk_prp_tag );
  zArrayAlloc( &array, int_array_t, ZTKCountTag( ztk, "tag" ) );
  printf( "ZTK file has %d tag fields.\n", zArraySize(&array) );
  ZTKEvalTag( &array, NULL, ztk, ztk_prp_tag, size );
  printf( "done.\n" );
  printf( "output ZTK format\n--\n" );
  prp = ZTKPrpDup( ztk_prp_tag, size );
  ZTKPrpSetNum( prp, size, "tag", zArraySize(&array) );
  ZTKPrpTagFPrint( stdout, &array, prp, size );
  free( prp );
  tag_array_free( &array );
}

int main(int argc, char *argv[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, "print_test.ztk" );
  test_eval_and_print( &ztk );
  ZTKDestroy( &ztk );
  return 0;
}
