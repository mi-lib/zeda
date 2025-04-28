/* how to use ZTK parser and evaluator for arbitrary number of fields */
#include <zeda/zeda_index.h>
#include <zeda/zeda_ztk.h>

zArrayClass( _TagArray, zIndex );
class TagArray : public _TagArray{
 public:
  TagArray(){ zArrayInit( this ); }
  ~TagArray(){
    for(int i=0; i<zArraySize(this); i++ )
      zIndexFree( *zArrayElemNC(this,i) );
    zArrayFree( this );
  }
};

void *eval_key(void *obj, int i, void *arg, ZTK *ztk)
{
  int val;

  val = ztk->getInt();
  printf( " reading key (%d/%d) ... value=%d ", i, zArraySize(static_cast<zIndex>(obj)), val );
  zIndexSetElemNC( static_cast<zIndex>(obj), i, val );
  printf( "set.\n" );
  return obj;
}

bool print_key(FILE*fp, int i, void *obj)
{
  fprintf( fp, "%d\n", zIndexElemNC(static_cast<zIndex>(obj),i) );
  return true;
}

ZTKPrp ztk_prp_key[] = {
  { "key", -1, eval_key, print_key },
};

void *eval_tag(void *obj, int i, void *arg, ZTK *ztk)
{
  zIndex index;

  printf( "reading tag (%d/%d) ...\n", i, zArraySize(static_cast<TagArray*>(obj)) );
  index = *zArrayElemNC(static_cast<TagArray*>(obj),i) = zIndexAlloc( ztk->countKey( "key" ) );
  printf( " tag #%d has %d key fields\n", i, zIndexSize(index) );
  return ZTKEvalKey( index, NULL, ztk, ztk_prp_key );
}

bool print_tag(FILE*fp, int i, void *obj)
{
  ZTKPrp *prp;
  int size;
  zIndex index;

  prp = ZTKPrpDup( ztk_prp_key );
  size = sizeof(ztk_prp_key) / sizeof(ZTKPrp);
  index = *zArrayElemNC(static_cast<TagArray*>(obj),i);
  _ZTKPrpSetNum( prp, size, "key", zIndexSize(index) );
  _ZTKPrpKeyFPrint( fp, index, prp, size );
  fprintf( fp, "\n");
  free( prp );
  return true;
}

ZTKPrp ztk_prp_tag[] = {
  { "tag", -1, eval_tag, print_tag },
};

void test_eval_and_print(ZTK *ztk)
{
  TagArray array;
  ZTKPrp *prp;
  int size;

  zArrayAlloc( &array, zIndex, ztk->countTag( "tag" ) );
  printf( "ZTK file has %d tag fields.\n", zArraySize(&array) );
  ZTKEvalTag( &array, NULL, ztk, ztk_prp_tag );
  printf( "done.\n" );
  printf( "output ZTK format\n--\n" );
  prp = ZTKPrpDup( ztk_prp_tag );
  size = sizeof(ztk_prp_tag) / sizeof(ZTKPrp);
  _ZTKPrpSetNum( prp, size, "tag", zArraySize(&array) );
  _ZTKPrpTagFPrint( stdout, &array, prp, size );
  free( prp );
}

int main(int argc, char *argv[])
{
  ZTK ztk;

  ztk.parse( "print_test.ztk" );
  test_eval_and_print( &ztk );
  return 0;
}
