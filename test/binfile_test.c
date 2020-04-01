#include <unistd.h>
#include <zeda/zeda.h>

#define N 100

#define FILENAME "endian_test.dat"

void read_write_float_test(void)
{
  FILE *fp;
  float data[N], val;
  register int i;
  bool result = true;

  if( !( fp = fopen( FILENAME, "w" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    data[i] = zRandF(-1.0e6,1.0e6);
    fwrite_float( fp, data[i] );
  }
  fclose( fp );

  if( !( fp = fopen( FILENAME, "r" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    val = fread_float( fp );
    if( data[i] != val ) result = false;
  }
  fclose( fp );

  unlink( FILENAME );
  zAssert( fread_float + fwrite_float, result );
}

void read_write_rev_float_test(void)
{
  FILE *fp;
  float data[N], val;
  register int i;
  bool result = true;

  if( !( fp = fopen( FILENAME, "w" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    data[i] = zRandF(-1.0e6,1.0e6);
    fwrite_float_rev( fp, data[i] );
  }
  fclose( fp );

  if( !( fp = fopen( FILENAME, "r" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    val = fread_float_rev( fp );
    if( data[i] != val ) result = false;
  }
  fclose( fp );

  unlink( FILENAME );
  zAssert( fread_float_rev + fwrite_float_rev, result );
}

void read_write_double_test(void)
{
  FILE *fp;
  double data[N], val;
  register int i;
  bool result = true;

  if( !( fp = fopen( FILENAME, "w" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    data[i] = zRandF(-1.0e10,1.0e10);
    fwrite_double( fp, data[i] );
  }
  fclose( fp );

  if( !( fp = fopen( FILENAME, "r" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    val = fread_double( fp );
    if( data[i] != val ) result = false;
  }
  fclose( fp );

  unlink( FILENAME );
  zAssert( fread_double + fwrite_double, result );
}

void read_write_rev_double_test(void)
{
  FILE *fp;
  double data[N], val;
  register int i;
  bool result = true;

  if( !( fp = fopen( FILENAME, "w" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    data[i] = zRandF(-1.0e10,1.0e10);
    fwrite_double_rev( fp, data[i] );
  }
  fclose( fp );

  if( !( fp = fopen( FILENAME, "r" ) ) ){
    ZOPENERROR( FILENAME );
    abort();
  }
  for( i=0; i<N; i++ ){
    val = fread_double_rev( fp );
    if( data[i] != val ) result = false;
  }
  fclose( fp );

  unlink( FILENAME );
  zAssert( fread_double_rev + fwrite_double_rev, result );
}

void assert_read_write_float_double(void)
{
  zRandInit();
  read_write_float_test();
  read_write_rev_float_test();
  read_write_double_test();
  read_write_rev_double_test();
}

#define TEST_ZBD_FILE "test.zbd"

void binfile_write_test(zBinFile *bf, int ival[], long lval[], float fval[], double dval[])
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

void binfile_read_test(zBinFile *bf, int ival[], long lval[], float fval[], double dval[])
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

bool binfile_check(int ival_src[], int ival_out[], long lval_src[], long lval_out[], float fval_src[], float fval_out[], double dval_src[], double dval_out[])
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

bool assert_binfile_IO(zBinFile *bf)
{
  int ival_src[N], ival_out[N];
  long lval_src[N], lval_out[N];
  float fval_src[N], fval_out[N];
  double dval_src[N], dval_out[N];

  zBinFileOpen( bf, TEST_ZBD_FILE, "wb" );
  zBinFileHeaderFWrite( bf );
  binfile_write_test( bf, ival_src, lval_src, fval_src, dval_src );
  zBinFileClose( bf );

  zBinFileOpen( bf, TEST_ZBD_FILE, "rb" );
  zBinFileHeaderFRead( bf );
  binfile_read_test( bf, ival_out, lval_out, fval_out, dval_out );
  zBinFileClose( bf );

  unlink( TEST_ZBD_FILE );
  return binfile_check( ival_src, ival_out, lval_src, lval_out, fval_src, fval_out, dval_src, dval_out );
}

int main(int argc, char *argv[])
{
  zBinFile bf;

  assert_read_write_float_double();

  zBinFileInfoSetThis( &bf );
  zAssert( zBinFile (default), assert_binfile_IO( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_BIG, 4, 8 );
  zAssert( zBinFile (big endian: 32bit int: 64bit long), assert_binfile_IO( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_BIG, 2, 4 );
  zAssert( zBinFile (big endian: 16bit int: 32bit long), assert_binfile_IO( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_LITTLE, 4, 8 );
  zAssert( zBinFile (little endian: 32bit int: 64bit long), assert_binfile_IO( &bf ) );

  zBinFileInfoSet( &bf, 1, Z_ENDIAN_LITTLE, 2, 4 );
  zAssert( zBinFile (little endian: 16bit int: 32bit long), assert_binfile_IO( &bf ) );

  return 0;
}
