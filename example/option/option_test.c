#include <zeda/zeda_option.h>

zOption option[] = {
  { "c",  NULL, NULL, "c switch", NULL, false },
  { "x",  "xarg", "<x val>", "x value", NULL, false },
  { "y",    NULL, "<y val>", "y value", NULL, false },
  { NULL, "zarg", "<z val>", "z value", NULL, false },
  { NULL,   NULL,      NULL,      NULL, NULL, false },
};

int main(int argc, char *argv[])
{
  zStrList arglist;

  zOptionHelp( option );
  zOptionRead( option, argv+1, &arglist );
  zStrListWrite( &arglist );
  zStrListDestroy( &arglist, false );
  zOptionDiag( option );
  return 0;
}
