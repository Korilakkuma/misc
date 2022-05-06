#include <stdio.h>
#include <stdlib.h>

static long long factorial(long long n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./factorial_by_recursive [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);
  long long r = factorial(n);

  printf("%lld! = %lld\n", n, r);

  return 0;
}

long long factorial(long long n) {
  if (n == 1) {
    return 1;
  }

  return n * factorial(n - 1);
}
