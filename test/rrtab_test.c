#include <zeda/zeda.h>

#define N 5

int main(void)
{
  zRRBool tab;

  zRRBoolAlloc( &tab, N );

  zRRBoolMark( &tab, 1, 2 );
  zRRBoolMark( &tab, 3, 4 );
  zAssert( zRRBoolMark, zRRBoolCheck( &tab, 2, 1 ) && zRRBoolCheck( &tab, 4, 3 ) );

  zRRBoolUnmark( &tab, 1, 2 );
  zRRBoolUnmark( &tab, 3, 4 );
  zAssert( zRRBoolUnmark, !zRRBoolCheck( &tab, 2, 1 ) && !zRRBoolCheck( &tab, 4, 3 ) );

  zRRBoolFree( &tab );
  return 0;
}
