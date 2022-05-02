#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long

static inline INT min(INT a, INT b);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./frog_dynamic_programming [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *d = (INT *)malloc(n * sizeof(INT));
  INT *h = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    h[i] = 10 * ((double)rand() / (RAND_MAX + 1.0));
  }

  for (INT i = 0; i < n; i++) {
    if (i == 0) {
      d[0] = 0;
    }

    if (i == 1) {
      d[1] = (INT)abs((int)(h[1] - h[0]));
    }

    if (i == 2) {
      INT d1 = d[1] + (INT)abs((int)(h[1] - h[0]));
      INT d2 = d[0] + (INT)abs((int)(h[2] - h[0]));

      d[2] = min(d1, d2);
    }

    if (i >= 3) {
      INT d1 = d[i - 1] + (INT)abs((int)(h[i - 1] - h[i - 2]));
      INT d2 = d[i - 2] + (INT)abs((int)(h[i - 0] - h[i - 2]));

      d[i] = min(d1, d2);
    }
  }

  printf("Minimum value is %lld\n", d[n - 1]);

  free(d);
  free(h);

  return 0;
}

static inline INT min(INT a, INT b) {
  return a < b ? a : b;
}
