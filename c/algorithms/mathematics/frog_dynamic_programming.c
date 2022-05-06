#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static inline long long min(long long a, long long b);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./frog_dynamic_programming [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  long long *d = (long long *)malloc(n);
  long long *h = (long long *)malloc(n);

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    h[i] = 10 * ((double)rand() / RAND_MAX);
  }

  for (long long i = 0; i < n; i++) {
    if (i == 0) {
      d[0] = 0;
    }

    if (i == 1) {
      d[1] = (long long)abs((int)(h[1] - h[0]));
    }

    if (i == 2) {
      long long d1 = d[1] + (long long)abs((int)(h[1] - h[0]));
      long long d2 = d[0] + (long long)abs((int)(h[2] - h[0]));

      d[2] = min(d1, d2);
    }

    if (i >= 3) {
      long long d1 = d[i - 1] + (long long)abs((int)(h[i - 1] - h[i - 2]));
      long long d2 = d[i - 2] + (long long)abs((int)(h[i - 0] - h[i - 2]));

      d[i] = min(d1, d2);
    }
  }

  printf("Minimum value is %lld\n", d[n - 1]);

  free(d);
  free(h);

  return 0;
}

static inline long long min(long long a, long long b) {
  return a < b ? a : b;
}
