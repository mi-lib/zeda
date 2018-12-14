#include <zeda/zeda_stream.h>
#include <zeda/zeda_string.h>

int main(void)
{
  zStream stream1, stream2;
  byte c;

  printf( ">> assign the original file to stream 1..." );
  zStreamOpenFile( &stream1, __FILE__, "r" );
  printf( ">> done.\n" );
  printf( ">> assign a memory space to stream 2..." );
  zStreamOpenMem( &stream2, 1000 );
  printf( ">> done.\n" );

  printf( ">> reading stream 1, and copying them to stream 2...\n" );
  while( zStreamRead( &stream1, &c, 1, 1 ) == 1 )
    zStreamWrite( &stream2, &c, 1, 1 );
  printf( ">> done.\n" );

  printf( ">> writing stream 2...\n" );
  zStreamRewind( &stream2 );
  while( zStreamRead( &stream2, &c, 1, 1 ) == 1 )
    printf( "%c", c );
  printf( ">> done.\n" );

  zStreamClose( &stream1 );
  zStreamClose( &stream2 );
  return 0;
}
