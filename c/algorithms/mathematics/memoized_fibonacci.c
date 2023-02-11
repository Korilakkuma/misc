#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT *m;

static INT fibonacci(INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./memoized_fibonacci [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  m = (INT *)calloc(n, sizeof(INT));

  printf("Fibonacci:\n");

  for (INT i = 1; i <= n; i++) {
    printf("%lld ", fibonacci(i));
  }

  free(m);

  return 0;
}

static INT fibonacci(INT n) {
  if (n <= 0) {
    exit(EXIT_FAILURE);
  }

  if (m[n] != 0) {
    return m[n];
  }

  if ((n == 1) || (n == 2)) {
    m[n] = 1;

    return m[n];
  }

  m[n] = fibonacci(n - 1) + fibonacci(n - 2);

  return m[n];
}
