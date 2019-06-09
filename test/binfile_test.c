#include <unistd.h>
#include <zeda/zeda.h>

#define TEST_ZBD_FILE "test.zbd"

#define N 100

void write_test(zBinFile *bf, int ival[], long lval[], float fval[], double dval[])
{
  register int i;

  zRandInit();
  for( i=0; i<N; i++ ){
    ival[i] = zRandI( -32767, 32767 );
    zBinFileIntFWrite( bf, ival[i] );
  }
  for( i=0; i<N; i++ ){
    lval[i] = zRandI( -32767, 32767 );
    zBinFileLongFWrite( bf, lval[i] );
  }
  for( i=0; i<N; i++ ){
    fval[i] = zRandF( -1.0e10, 1.0e10 );
    zBinFileFloatFWrite( bf, fval[i] );
  }
  for( i=0; i<N; i++ ){
    dval[i] = zRandF( -1.0e10, 1.0e10 );
    zBinFileDoubleFWrite( bf, dval[i] );
  }
}

void read_test(zBinFile *bf, int ival[], long lval[], float fval[], double dval[])
{
  register int i;

  for( i=0; i<N; i++ ){
    ival[i] = zBinFileIntFRead( bf );
  }
  for( i=0; i<N; i++ ){
    lval[i] = zBinFileLongFRead( bf );
  }
  for( i=0; i<N; i++ ){
    fval[i] = zBinFileFloatFRead( bf );
  }
  for( i=0; i<N; i++ ){
    dval[i] = zBinFileDoubleFRead( bf );
  }
}

bool check(int ival_src[], int ival_out[], long lval_src[], long lval_out[], float fval_src[], float fval_out[], double dval_src[], double dval_out[])
{
  register int i;

  for( i=0; i<N; i++ ){
    if( ival_src[i] != ival_out[i] ||
        lval_src[i] != lval_out[i] ||
        fval_src[i] != fval_out[i] ||
        dval_src[i] != dval_out[i] ) return false;
  }
  return true;
}

bool test(zBinFile *bf)
{
  int ival_src[N], ival_out[N];
  long lval_src[N], lval_out[N];
  float fval_src[N], fval_out[N];
  double dval_src[N], dval_out[N];

  zBinFileOpen( bf, TEST_ZBD_FILE, "wb" );
  zBinFileHeaderFWrite( bf );
  write_test( bf, ival_src, lval_src, fval_src, dval_src );
  zBinFileClose( bf );

  zBinFileOpen( bf, TEST_ZBD_FILE, "rb" );
  zBinFileHeaderFRead( bf );
  read_test( bf, ival_out, lval_out, fval_out, dval_out );
  zBinFileClose( bf );

  unlink( TEST_ZBD_FILE );
  return check( ival_src, ival_out, lval_src, lval_out, fval_src, fval_out, dval_src, dval_out );
}

int main(int argc, char *argv[])
{
  zBinFile bf;

  zBinFileInfoSetThis( &bf );
  zAssert( zBinFile (default), test( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_BIG, 4, 8 );
  zAssert( zBinFile (big endian: 32bit int: 64bit long), test( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_BIG, 2, 4 );
  zAssert( zBinFile (big endian: 16bit int: 32bit long), test( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_LITTLE, 4, 8 );
  zAssert( zBinFile (little endian: 32bit int: 64bit long), test( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_LITTLE, 2, 4 );
  zAssert( zBinFile (little endian: 16bit int: 32bit long), test( &bf ) );

  return 0;
}
