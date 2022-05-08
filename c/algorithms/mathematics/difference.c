#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static long long difference(long long a[], long long n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./difference [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  long long *a = (long long *)malloc(n);

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    a[i] = 10 * ((double)rand() / RAND_MAX);

    printf("[%lld] = %lld\n", i, a[i]);
  }

  printf("Difference = %lld\n", difference(a, n));

  free(a);

  return 0;
}

static long long difference(long long a[], long long n) {
  long long c[n];

  c[0] = a[0];

  for (long long i = 1; i < n; i++) {
    c[i] = a[i] - a[i - 1];
  }

  return c[n - 1];
}
