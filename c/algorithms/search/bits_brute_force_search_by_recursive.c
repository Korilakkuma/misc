#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long

static bool search(INT a[], INT n, INT w);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./bits_brute_force_search_by_recursive [0-9]+ [0-9]+\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT w = strtoll(argv[2], NULL, 10);

  srand((unsigned int)time(NULL));

  INT *a = (INT *)malloc((size_t)(n * sizeof(INT)));

  for (INT i = 0; i < n; i++) {
    a[i] = (INT)(n * ((double)rand() / (RAND_MAX + 1.0)));
    printf("%lld ", a[i]);
  }

  printf("\n");


  if (search(a, n, w)) {
    printf("%lld is found\n", w);
  } else {
    printf("%lld is not found\n", w);
  }

  free(a);

  return 0;
}

static bool search(INT a[], INT n, INT w) {
  if (n == 0) {
    if (w == 0) {
      return true;
    }

    return false;
  }

  if (search(a, (n - 1), (w - 0))) {
    return true;
  }

  if (search(a, (n - 1), (w - (a[n - 1])))) {
    return true;
  }

  return false;
}
