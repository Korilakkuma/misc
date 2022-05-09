#include <stdio.h>
#include <stdlib.h>

#define INT long long
#define NOT_PRIME_NUMBER -1

static void eratosthenes_sieve(INT a[], INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./eratosthenes_sieve [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *a = (INT *)malloc(n * sizeof(INT));

  for (INT i = 0; i < n; i++) {
    a[i] = i + 1;
  }

  eratosthenes_sieve(a, n);

  printf("Prime number until %lld\n", n);

  for (INT i = 0; i < n; i++) {
    if (a[i] != NOT_PRIME_NUMBER) {
      printf("%lld ", a[i]);
    }
  }

  free(a);

  return 0;
}

static void eratosthenes_sieve(INT a[], INT n) {
  a[0] = NOT_PRIME_NUMBER;

  for (INT i = 2; (i * i) <= n; i++) {
    for (INT j = i - 1; j < n; j++) {
      if ((i != a[j]) && ((a[j] % i) == 0)) {
        a[j] = NOT_PRIME_NUMBER;
      }
    }
  }
}
