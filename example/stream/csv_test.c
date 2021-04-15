#include <zeda/zeda_csv.h>

#define N 500

void create_test_csv_file(void)
{
  FILE *fp;
  register int i;

  if( !( fp = fopen( "test.csv", "wt" ) ) ){
    ZOPENERROR( "test.csv" );
    exit( EXIT_FAILURE );
  }
  fprintf( fp, "%% test_header,time stamp,(void),x-value,y-value,z-value\n" );
  for( i=0; i<N; i++ ){
    fprintf( fp, "test,%d,,%f,%f,%f\n", i, (double)i/N, (double)i/N-0.5, -(double)i/N );
  }
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zCSV csv;
  int t;
  double x, y, z;
  char buf[BUFSIZ];

  if( argc < 2 ){
    eprintf( "create a CSV file for test.\n" );
    create_test_csv_file();
    return 0;
  }
  zCSVOpen( &csv, argv[1] );
  while( !zCSVIsEOF( &csv ) ){
    if( !zCSVGetField( &csv, buf, BUFSIZ ) ) break;
    t = zCSVGetInt( &csv );
    zCSVGetInt( &csv ); /* dummy */
    x = zCSVGetDouble( &csv );
    y = zCSVGetDouble( &csv );
    z = zCSVGetDouble( &csv );
    printf( "%d: x=%g, y=%g, z=%g\n", t, x, y, z );
  }
  zCSVClose( &csv );
  return 0;
}
