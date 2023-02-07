#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./bits_brute_force_search [0-9]+ [0-9]+\n", stderr);
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

  bool found = false;

  for (INT bits = 0; bits < (1 << n); bits++) {
    INT sum = 0;

    for (INT i = 0; i < n; i++) {
      if (bits & (1 << i)) {
        sum += a[i];
      }
    }

    if (sum == w) {
      found = true;
      break;
    }
  }

  if (found) {
    printf("%lld is found\n", w);
  } else {
    printf("%lld is not found\n", w);
  }

  free(a);

  return 0;
}
