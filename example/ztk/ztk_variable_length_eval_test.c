/* how to use ZTK parser and evaluator */
#include <zeda/zeda_ztk.h>

/* definition of a calculation expression. */
typedef struct{
  int var_tag_num;
  int var_key_num;
} expression_t;

#define exp_obj ((expression_t *)obj)

void *eval_variable(void *obj, int i, void *arg, ZTK *ztk)
{
  int c = ZTKInt(ztk);
  printf("key cnt=%d value=%d --- read\n", i, c);
  return obj;
}

void print_variable(FILE*fp, int i, void *obj)
{
  int j;

  for( j=0; j < exp_obj->var_key_num; j++ ){
    fprintf( fp, "variable : %d\n", j );
  }
}

/* a list of keys to which callback functions are associated. */
ZTKPrp ztk_prp_job[] = {
  { "variable", -1, eval_variable, print_variable },
};

void *eval_varibale_tag(void *obj, int i, void *arg, ZTK *ztk)
{
  printf("tag cnt=%d\n", i);
  return ZTKEvalKey( obj, NULL, ztk, ztk_prp_job );
}

void print_varibale_tag(FILE*fp, int i, void *obj)
{
  int j;

  for( j=0; j < exp_obj->var_tag_num; j++ ){
    fprintf( fp, "[variable_tag]\n" );
    ZTKPrpKeyFPrint( fp, obj, ztk_prp_job );
    fprintf( fp, "\n");
  }
}

/* a list of tags to which callback functions are associated. */
ZTKPrp ztk_prp_calculator[] = {
  { "variable_tag", -1, eval_varibale_tag, print_varibale_tag}
};

/* evaluate a ZTK processor. */
void eval_test(ZTK *ztk)
{
  expression_t expression;
  /* ZTKEvalTag( &expression, NULL, ztk, ztk_prp_calculator ); */
  ZTKEvalTag( &expression, NULL, ztk, ztk_prp_calculator );
  expression.var_tag_num = 7;
  expression.var_key_num = 3;
  FILE* fp = stdout;
  fprintf( fp, "\n");
  ZTKPrpTagFPrint( fp, &expression, ztk_prp_calculator );
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
