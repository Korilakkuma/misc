#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
  RED,
  GREEN,
  BLUE
};

static long long permutation(long long n, long long r);
static long long combination(long long n, long long r);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./combination_problem [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtol(argv[1], NULL, 10);

  long long *a = (long long *)malloc(n);

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    a[i] = (long long)(3 * ((double)rand() / RAND_MAX));
  }

  long long r = 0;
  long long g = 0;
  long long b = 0;

  for (long long i = 0; i < n; i++) {
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

  long long pr = combination(r, 2);
  long long pg = combination(g, 2);
  long long pb = combination(b, 2);

  printf("Pattern is %lld (Red %lld, Green %lld, Blue %lld)\n", (pr + pg + pb), pr, pg, pb);

  free(a);

  return 0;
}

static long long permutation(long long n, long long r) {
  long long p = 1;

  for (long long i = n, nr = (n - r); i > nr; i--) {
    p *= i;
  }

  return p;
}

static long long combination(long long n, long long r) {
  return permutation(n, r) / permutation(r, r);
}

