#include <stdio.h>
#include <stdlib.h>

void enumerate_divisors(long long n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./enumerate_divisors [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  enumerate_divisors(n);

  return 0;
}

void enumerate_divisors(long long n) {
  for (long long i = 1; (i * i) <= n; i++) {
    if ((n % i) == 0) {
      printf("%lld\n", i);

      long long d = n / i;

      if (d != i) {
        printf("%lld\n", (n / i));
      }
    }
  }
}
