#include <stdio.h>
#include <stdlib.h>

#define INT long long

static void enumerate_divisors(INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./enumerate_divisors [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  enumerate_divisors(n);

  return 0;
}

static void enumerate_divisors(INT n) {
  for (INT i = 1; (i * i) <= n; i++) {
    if ((n % i) == 0) {
      printf("%lld\n", i);

      INT d = n / i;

      if (d != i) {
        printf("%lld\n", (n / i));
      }
    }
  }
}
