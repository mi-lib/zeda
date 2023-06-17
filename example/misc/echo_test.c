#include <zeda/zeda_misc.h>

int main(int argc, char *argv[])
{
  eprintf( "echo off\n" );
  zEchoOff();
  ZECHO( "Hello." );
  eprintf( "echo on\n" );
  zEchoOn();
  ZECHO( "Hello." );
  return 0;
}
