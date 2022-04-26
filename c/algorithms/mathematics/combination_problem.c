#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long

enum {
  RED,
  GREEN,
  BLUE
};

static INT permutation(INT n, INT r);
static INT combination(INT n, INT r);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./combination_problem [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *a = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    a[i] = (INT)(3 * ((double)rand() / (RAND_MAX + 1.0)));
  }

  INT r = 0;
  INT g = 0;
  INT b = 0;

  for (INT i = 0; i < n; i++) {
    switch (a[i]) {
      case RED:
        r++;
        break;
      case GREEN:
        g++;
        break;
      case BLUE:
        b++;
        break;
      default:
        break;
    }
  }

  INT pr = combination(r, 2);
  INT pg = combination(g, 2);
  INT pb = combination(b, 2);

  printf("Pattern is %lld (Red %lld, Green %lld, Blue %lld)\n", (pr + pg + pb), pr, pg, pb);

  free(a);

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
