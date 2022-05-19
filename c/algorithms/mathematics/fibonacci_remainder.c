#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT fibonacci_remainder(INT n, INT d);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./fibonacci_remainder [natural number] [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT d = strtoll(argv[2], NULL, 10);

  printf("Fibonacci Remainder %lld\n", fibonacci_remainder(n, d));

  return 0;
}

static INT fibonacci_remainder(INT n, INT d) {
  if ((n <= 0) || (d <= 0)) {
    exit(EXIT_FAILURE);
  }

  if ((n == 1) || (n == 2)) {
    return 1 % d;
  }

  INT c;

  INT p_2 = 1;
  INT p_1 = 1;

  for (INT i = 3; i <= n; i++) {
    c = (p_1 % d) + (p_2 % d);

    p_2 = p_1 % d;
    p_1 = c;
  }

  return c % d;
}
