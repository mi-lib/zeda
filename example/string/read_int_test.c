#include <zeda/zeda_string.h>

int main(void)
{
  char buf[BUFSIZ];

  printf( "enter a string: " );
  if( !fgets( buf, BUFSIZ, stdin ) ) return 1;
#ifdef __WINDOWS__
  int val = 0;
  while (buf[0])
  {
      char ret = *zSInt(buf, &val);
      if (ret != '\0')
      {
          printf("ret=%c\n", ret);
          printf("val=%d\n", val);
      }
  }
#else
  while (buf[0])
      printf("%d\n", zSInt(buf));
#endif

  printf( "enter 3 numbers: " );
#ifdef __WINDOWS__  
  int fval1=0, fval2=0, fval3=0;
  char ret1 = *zFInt(stdin, &fval1);
  char ret2 = *zFInt(stdin, &fval2);
  char ret3 = *zFInt(stdin, &fval3);
  printf("ret[1]=%c, ret[2]=%c, ret[3]=%c\n", ret1, ret2, ret3);
  printf("val[1]=%d, val[2]=%d, val[3]=%d\n", fval1, fval2, fval3);
#else
  printf("val[1]=%d, val[2]=%d, val[3]=%d\n", zFInt(stdin), zFInt(stdin), zFInt(stdin));
#endif
  printf( "(probably the result is different from what is expected due to the specification of printf.)\n" );
  return 0;
}
