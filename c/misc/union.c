#include <stdio.h>

typedef union {
  char c;
  int n1;
  int n2;
} TEST_UNION;

int main(void) {
  TEST_UNION u;

  u.c  = 'A';
  u.n1 = 127;

  printf("%d [address %p]\n", u.c, &u.c);
  printf("%d [address %p]\n", u.n1, &u.n1);
  printf("%d [address %p]\n", u.n2, &u.n2);

  u.c  = 'A';
  u.n1 = 128;

  printf("%d [address %p]\n", u.c, &u.c);
  printf("%d [address %p]\n", u.n1, &u.n1);
  printf("%d [address %p]\n", u.n2, &u.n2);

  return 0;
}
