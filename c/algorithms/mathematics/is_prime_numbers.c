#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int is_prime_numbers(long long n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./is_prime_numbers [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  if (is_prime_numbers(n)) {
    printf("%lld is prime numbers.\n", n);
  } else {
    printf("%lld is not prime numbers.\n", n);
  }

  return 0;
}

int is_prime_numbers(long long n) {
  if (n == 1) {
    return FALSE;
  }

  for (long long i = 2; (i * i) <= n; i++) {
    if (i == n) {
      break;
    }

    if ((n % i) == 0) {
      return FALSE;
    }
  }

  return TRUE;
}
