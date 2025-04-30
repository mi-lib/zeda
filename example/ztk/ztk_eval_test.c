/* how to use ZTK parser, evaluator, and generator */
#include <zeda/zeda_ztk.h>

/* *** calculation expression class *** */
typedef enum{
  EXPRESSION_OP_NONE=-1,
  EXPRESSION_OP_ADD,
  EXPRESSION_OP_SUB,
  EXPRESSION_OP_MUL,
  EXPRESSION_OP_DIV,
} op_t;

typedef struct{
  double val1, val2;
  op_t op;
} expression_t;

/* refer the operator identifier by a charactor */
op_t expression_op(char op)
{
  const char op_list[] = "+-*/";
  const char *op_pos;
  return ( op_pos = strchr( op_list, op ) ) ? (op_t)( op_pos - op_list ) : EXPRESSION_OP_NONE;
}

/* refer the operator charactor by an identifier */
char expression_op_char(expression_t *expression)
{
  const char op_list[] = "+-*/";
  return expression->op != EXPRESSION_OP_NONE ? op_list[expression->op] : '\0';
}

/* initialize an expression */
expression_t *expression_init(expression_t *expression)
{
  expression->val1 = expression->val2 = 0;
  expression->op = EXPRESSION_OP_NONE;
  return expression;
}

/* calculate a value of an expression */
double expression_calculate(expression_t *expression)
{
  switch( expression->op ){
  case EXPRESSION_OP_ADD: return expression->val1 + expression->val2;
  case EXPRESSION_OP_SUB: return expression->val1 - expression->val2;
  case EXPRESSION_OP_MUL: return expression->val1 * expression->val2;
  case EXPRESSION_OP_DIV: return expression->val1 / expression->val2;
  default: eprintf( "unknown operator.\n" );
  }
  return 0;
}

/* print an expression with the calculation result */
void print_expression(expression_t *expression)
{
  printf( "%g %c %g = %g\n", expression->val1, expression_op_char(expression), expression->val2, expression_calculate( expression ) );
}

/* *** ZTK manipulation *** */

#define exp_obj ((expression_t *)obj)

/* callback function to pick up an operator and operands */
void *eval_expression(void *obj, int i, void *arg, ZTK *ztk)
{
  exp_obj->val1 = ZTKDouble(ztk);
  exp_obj->op = expression_op( ZTKVal(ztk)[0] );
  ZTKValNext(ztk);
  exp_obj->val2 = ZTKDouble(ztk);
  return obj;
}

/* callback function to do an arithmatic operation */
void *eval_operation(void *obj, int i, void *arg, ZTK *ztk)
{
  char op;

  if( ZTKValCmp( ztk, "print" ) ){
    print_expression( exp_obj );
  } else
  if( ZTKValCmp( ztk, "scan" ) ){
    expression_init( exp_obj );
    printf( "enter an expression > " );
    if( scanf( "%lf %c %lf", &exp_obj->val1, &op, &exp_obj->val2 ) == 3 ){
      exp_obj->op = expression_op( op );
      print_expression( exp_obj );
    }
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
  return _ZTKEvalKey( obj, NULL, ztk, ztk_prp_job );
}

/* a list of tags to which callback functions are associated. */
ZTKPrp ztk_prp_calculator[] = {
  { "job", -1, job_exec, NULL },
};

/* evaluate a ZTK processor. */
void eval_test(ZTK *ztk, expression_t *expression)
{
  _ZTKEvalTag( expression, NULL, ztk, ztk_prp_calculator );
}

int main(int argc, char *argv[])
{
  ZTK ztk;
  expression_t expression;

  ZTKInit( &ztk );
  ZTKParse( &ztk, "calcurator.ztk" );
  /* activate the following line in order to see an image of the parsed tree. */
  ZTKPrint( &ztk );
  eval_test( &ztk, &expression );
  ZTKDestroy( &ztk );
  return 0;
}
