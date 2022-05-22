#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long

static inline bool determine(INT t, INT k);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./ [natural number] [natural nunmber]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT k = strtoll(argv[2], NULL, 10);

  if ((n <= 0) || (k <= 0)) {
    exit(EXIT_FAILURE);
  }

  INT *a = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    a[i] = 10 * ((double)rand() / (RAND_MAX + 1.0));

    printf("[%lld] = %lld\n", i, a[i]);
  }

  INT t = 0;

  for (INT i = 0; i < n; i++) {
    t += a[i];
  }

  printf("%s [total %lld]\n", (determine(t, k) ? "True" : "False"), t);

  free(a);

  return 0;
}

static inline bool determine(INT t, INT k) {
  if (k < t) {
    return false;
  }

  if ((t % 2) != (k % 2)) {
    return false;
  }

  return true;
}
