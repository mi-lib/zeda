/* how to use ZTK parser and evaluator */
#include <zeda/zeda_ztk.h>

/* definition of a calculation expression. */
typedef struct{
  double val1, val2;
  char op;
} expression_t;

/* calculate a value based on a specified expression. */
double calculate(expression_t *expression)
{
  switch( expression->op ){
  case '+': return expression->val1 + expression->val2;
  case '-': return expression->val1 - expression->val2;
  case '*': return expression->val1 * expression->val2;
  case '/': return expression->val1 / expression->val2;
  default: ;
  }
  return 0;
}

/* print an expression with the calculation result. */
void print_expression(expression_t *expression)
{
  printf( "%g %c %g = %g\n", expression->val1, expression->op, expression->val2, calculate( expression ) );
}

#define exp_obj ((expression_t *)obj)

/* a callback function to pick up an operator and operands. */
void *eval_expression(void *obj, int i, void *arg, ZTK *ztk)
{
  exp_obj->val1 = ZTKDouble(ztk);
  exp_obj->op = ZTKVal(ztk)[0];
  ZTKValNext(ztk);
  exp_obj->val2 = ZTKDouble(ztk);
  return obj;
}

/* a callback function to do an arithmatic operation. */
void *eval_operation(void *obj, int i, void *arg, ZTK *ztk)
{
  if( ZTKValCmp( ztk, "print" ) ){
    print_expression( exp_obj );
  } else
  if( ZTKValCmp( ztk, "scan" ) ){
    printf( "enter an expression > " );
    if( scanf( "%lf %c %lf", &exp_obj->val1, &exp_obj->op, &exp_obj->val2 ) == 3 )
      print_expression( exp_obj );
  }
  return obj;
}

/* a list of keys to which callback functions are associated. */
ZTKPrp ztk_prp_job[] = {
  { "expression", 1, eval_expression, NULL },
  { "operation", 1, eval_operation, NULL },
};

/* a callback function to execute a job corresponding to each tag field. */
void *job_exec(void *obj, int i, void *arg, ZTK *ztk)
{
  printf( "job #%d\n", i );
  return ZTKEvalKey( obj, NULL, ztk, ztk_prp_job );
}

/* a list of tags to which callback functions are associated. */
ZTKPrp ztk_prp_calculator[] = {
  { "job", -1, job_exec, NULL },
};

/* evaluate a ZTK processor. */
void eval_test(ZTK *ztk)
{
  expression_t expression;
  ZTKEvalTag( &expression, NULL, ztk, ztk_prp_calculator );
}

int main(int argc, char *argv[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, "calcurator.ztk" );
  /* activate the following line in order to see an image of the parsed tree. */
  ZTKPrint( &ztk );
  eval_test( &ztk );
  ZTKDestroy( &ztk );
  return 0;
}
