#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT permutation(INT n, INT r);
static INT combination(INT n, INT r);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./combination [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT r = strtoll(argv[2], NULL, 10);

  INT c = combination(((n > r) ? n : r), ((r < n) ? r : n));

  if (n > r) {
    printf("%lld_C_%lld = %lld\n", n, r, c);
  } else {
    printf("%lld_C_%lld = %lld\n", r, n, c);
  }

  return 0;
}

static INT permutation(INT n, INT r) {
  INT p = 1;

  for (INT i = n, nr = (n - r); i > nr; i--) {
    p *= i;
  }

  return p;
}

static INT combination(INT n, INT r) {
  return permutation(n, r) / permutation(r, r);
}
