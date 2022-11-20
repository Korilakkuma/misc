#include <stdio.h>

int main(int argc, char **argv) {
  int v1 = 512;
  int v2 = 525;

  const int *cp = &v1;
  //int *const cp = &v1;

  // printf("const int *p = %d\n", *cp);

  // *cp = v2;

  printf("const int *p = %d\n", *cp);

  cp = &v2;

  printf("const int *p = %d\n", *cp);

  return 0;
}
