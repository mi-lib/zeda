#include <zeda/zeda_name.h>

struct test_t{
  Z_NAMED_CLASS
  int val;
} array[] = {
  { "a", 0 },
  { "b", 1 },
  { "c", 2 },
  { "d", 3 },
  { "e", 4 },
  { "f", 5 },
  { "g", 6 },
  { "h", 7 },
  { "i", 8 },
  { "j", 9 },
};

int main(void)
{
  struct test_t *p1, *p2, *p3;
  int num;

  num = sizeof(array) / sizeof(struct test_t);
  zNameFind( array, num, "a", p1 );
  zNameFind( array, num, "f", p2 );
  zNameFind( array, num, "k", p3 );
  zAssert( zNameFind, p1 && p1->val == 0 && p2 && p2->val == 5 && !p3 );
  return 0;
}
