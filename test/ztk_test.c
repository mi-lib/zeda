#include <zeda/zeda.h>

void *assert_ztk_prp_eval_test(void *obj, int i, void *arg, ZTK *ztk){ return NULL; /* dummy */ }
bool assert_ztk_prp_print_test(FILE *fp, int i, void *obj){ return true; }

bool assert_ztk_prp_cmp(const ZTKPrp *prp1, const ZTKPrp *prp2)
{
  return strcmp( prp1->str, prp2->str ) == 0 &&
         prp1->num == prp2->num &&
         prp1->_eval == prp2->_eval &&
         prp1->_fprint == prp2->_fprint ? true : false;
}

void assert_ztk_prp_set_num(void)
{
  const ZTKPrp ztk_prp[] = {
    { "prp1", 1, assert_ztk_prp_eval_test, assert_ztk_prp_print_test },
    { "prp2", -1, assert_ztk_prp_eval_test, assert_ztk_prp_print_test },
  };
  ZTKPrp *prp;
  int n;

  n = sizeof(ztk_prp) / sizeof(ZTKPrp);
  if( !( prp = ZTKPrpDup( ztk_prp ) ) ) return;
  zAssert( ZTKPrpDup,
    assert_ztk_prp_cmp( &ztk_prp[0], &prp[0] ) &&
    assert_ztk_prp_cmp( &ztk_prp[1], &prp[1] ) );
  _ZTKPrpSetNum( prp, n, "prp1", 2 );
  _ZTKPrpSetNum( prp, n, "prp2", 2 );
  zAssert( ZTKPrpSetNum (permitted case), prp[0].num == 1 );
  zAssert( ZTKPrpSetNum (unpermitted case), prp[1].num == 2 );
  free( prp );
}

int main(void)
{
  assert_ztk_prp_set_num();
  return 0;
}
