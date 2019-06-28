#include <zeda/zeda_strlist.h>

typedef struct _zFileStack{
  char pathname[BUFSIZ];
  FILE *fp;
  struct _zFileStack *chain;
} zFileStack;

int main(int argc, char *argv[])
{
  return 0;
}
