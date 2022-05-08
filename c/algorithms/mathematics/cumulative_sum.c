#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long

static INT cumulative_sum(INT a[], INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./cumulative_sum [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *a = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    a[i] = 10 * ((double)rand() / (RAND_MAX + 1.0));

    printf("[%lld] = %lld\n", i, a[i]);
  }

  printf("Cumulative sum = %lld\n", cumulative_sum(a, n));

  free(a);

  return 0;
}

static INT cumulative_sum(INT a[], INT n) {
  INT c[n];

  c[0] = a[0];

  for (INT i = 1; i < n; i++) {
    c[i] = a[i] + c[i - 1];
  }

  return c[n - 1];
}
