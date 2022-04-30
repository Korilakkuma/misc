#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT factorial(INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./factorial_by_recursive [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT r = factorial(n);

  printf("%lld! = %lld\n", n, r);

  return 0;
}

static INT factorial(INT n) {
  if (n == 1) {
    return 1;
  }

  return n * factorial(n - 1);
}
