/* how to use ZTK parser and evaluator */
#include <zeda/zeda_ztk.h>

/* definition of a calculation expression. */
typedef struct{
  int var_tag_num;
  int* var_key_num; /* [tag] array */
  int tag_cnt;
  int** variable; /* [tag][key] array */
} expression_t;

#define exp_obj ((expression_t *)obj)

void *eval_variable_key(void *obj, int i, void *arg, ZTK *ztk)
{
  int c;

  c = ZTKInt(ztk);
  printf("key cnt=%d/%d value=%d --- read\n", i, exp_obj->var_key_num[exp_obj->tag_cnt]-1, c);
  exp_obj->variable[exp_obj->tag_cnt][i] = c;
  return obj;
}

bool print_variable_key(FILE*fp, int i, void *obj)
{
  fprintf( fp, "%d\n", exp_obj->variable[exp_obj->tag_cnt][i] );

  return true;
}

/* a list of keys to which callback functions are associated. */
ZTKPrp ztk_prp_key_job[] = {
  { "variable", -1, eval_variable_key, print_variable_key },
};

void *eval_varibale_tag(void *obj, int i, void *arg, ZTK *ztk)
{
  exp_obj->var_key_num[i] = ZTKCountKey( ztk, "variable" );
  exp_obj->variable[i] = zAlloc( int, exp_obj->var_key_num[i] );
  exp_obj->tag_cnt = i;
  printf("tag cnt=%d/%d\n", i, exp_obj->var_tag_num-1 );
  return ZTKEvalKey( obj, NULL, ztk, ztk_prp_key_job );
}

bool print_varibale_tag(FILE*fp, int i, void *obj)
{
  ZTKPrp *copy_prp_key;
  int prp_size;

  copy_prp_key = ZTKPrpDup( ztk_prp_key_job );
  prp_size = sizeof(ztk_prp_key_job)/sizeof(ztk_prp_key_job);
  _ZTKPrpSetNum( copy_prp_key, prp_size, "variable", exp_obj->var_key_num[i] );
  exp_obj->tag_cnt = i;
  _ZTKPrpKeyFPrint( fp, obj, copy_prp_key, prp_size );
  fprintf( fp, "\n");
  free( copy_prp_key );
  free( exp_obj->variable[i] );
  return true;
}

/* a list of tags to which callback functions are associated. */
ZTKPrp ztk_prp_tag_calculator[] = {
  { "variable_tag", -1, eval_varibale_tag, print_varibale_tag}
};

/* evaluate a ZTK processor. */
void eval_test(ZTK *ztk)
{
  expression_t expression;
  FILE *fp;
  ZTKPrp *copy_prp_tag;
  int prp_size;

  expression.var_tag_num = ZTKCountTag( ztk, "variable_tag" );
  expression.variable = zAlloc( int*, expression.var_tag_num );
  expression.var_key_num = zAlloc( int, expression.var_tag_num );
  ZTKEvalTag( &expression, NULL, ztk, ztk_prp_tag_calculator );
  fp = stdout;
  fprintf( fp, "\n");
  copy_prp_tag = ZTKPrpDup( ztk_prp_tag_calculator );
  prp_size = sizeof(ztk_prp_tag_calculator)/sizeof(ztk_prp_tag_calculator);
  _ZTKPrpSetNum( copy_prp_tag, prp_size, "variable_tag", expression.var_tag_num );
  _ZTKPrpTagFPrint( fp, &expression, copy_prp_tag, prp_size );
  free( copy_prp_tag );
  free( expression.var_key_num );
  free( expression.variable );
}

int main(int argc, char *argv[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, "varialbe_length.ztk" );
  eval_test( &ztk );
  ZTKDestroy( &ztk );

  return 0;
}
