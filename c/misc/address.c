#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int *a = (int *)malloc(10 * sizeof(int));

  printf("char       = %ld bytes\n", sizeof(char));
  printf("short      = %ld bytes\n", sizeof(short));
  printf("int        = %ld bytes\n", sizeof(int));
  printf("long       = %ld bytes\n", sizeof(long));
  printf("long long  = %ld bytes\n", sizeof(long long));

  fputs("Type Int > ", stdout);
  scanf("%d", a);

  int *b = a + 1;  // Next element but there is not value ...

  if (*a == *b) {
    printf("a = %p *a = %d\n", a, *a);
    printf("b = %p *b = %d\n", b, *b);
  }

  return 0;
}
