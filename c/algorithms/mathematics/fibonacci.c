#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT fibonacci(INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./fibonacci [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  printf("Fibonacci:\n");

  for (INT i = 1; i <= n; i++) {
    printf("%lld ", fibonacci(i));
  }

  return 0;
}

static INT fibonacci(INT n) {
  if (n <= 0) {
    exit(EXIT_FAILURE);
  }

  if ((n == 1) || (n == 2)) {
    return 1;
  }

  return fibonacci(n - 1) + fibonacci(n - 2);
}
