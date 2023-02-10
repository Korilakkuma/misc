#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long
#define N 10
#define RANGE 50
#define NOT_FOUND -1

static int comp(const void *a, const void *b);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./binary_search [0-9]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT key = strtoll(argv[1], NULL, 10);

  INT a[N];

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    a[i] = RANGE * ((double)rand() / (RAND_MAX + 1.0));
  }

  qsort((INT *)a, N, sizeof(INT), comp);

  INT l = 0;
  INT h = N - 1;
  INT m = (l + h) / 2;

  INT fi = NOT_FOUND;

  while (l <= h) {
    if (key == a[m]) {
      fi = m;
      break;
    }

    if (key < a[m]) {
      h = m - 1;
    } else {
      l = m + 1;
    }

    m = (l + h) / 2;
  }

  if (fi != NOT_FOUND) {
    printf("%lld is found (a[%lld])\n\n", key, fi);
  } else {
    printf("%lld is not found\n\n", key);
  }

  printf("dump\n");

  for (INT i = 0; i < N; i++) {
    printf("a[%lld] = %lld\n", i, a[i]);
  }

  printf("\n");

  return 0;
}

static int comp(const void *a, const void *b) {
  return *(INT *)a - *(INT *)b;
}
