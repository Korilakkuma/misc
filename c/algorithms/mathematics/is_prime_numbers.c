#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long

static bool is_prime_numbers(INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./is_prime_numbers [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  if (is_prime_numbers(n)) {
    printf("%lld is prime numbers.\n", n);
  } else {
    printf("%lld is not prime numbers.\n", n);
  }

  return 0;
}

static bool is_prime_numbers(INT n) {
  if (n == 1) {
    return false;
  }

  for (INT i = 2; (i * i) <= n; i++) {
    if (i == n) {
      break;
    }

    if ((n % i) == 0) {
      return false;
    }
  }

  return true;
}
