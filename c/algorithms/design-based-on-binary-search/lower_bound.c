#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long
#define N 10
#define RANGE 20
#define INIT_INDEX -1LL

static bool predicate(INT a, INT b);
static int comp(const void *a, const void *b);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./lower_bound [0-9]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT key = strtoll(argv[1], NULL, 10);

  INT a[N];

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    a[i] = RANGE * ((double)rand() / (RAND_MAX + 1.0));
  }

  qsort((void *)a, (size_t)N, sizeof(INT), comp);

  INT left  = 0;
  INT right = N - 1;

  if (key > a[right]) {
    printf("Not Found\n");
  } else {
    while ((right - left) > 1) {
      INT middle = (right + left) / 2;

      if (predicate(a[middle], key)) {
        left = middle;
      } else {
        right = middle;
      }
    }

    printf("%lld <= (lower_bound[%lld] = %lld)\n", key, right, a[right]);
  }

  printf("dump\n");

  for (INT i = 0; i < N; i++) {
    printf("a[%lld] = %lld\n", i, a[i]);
  }

  printf("\n");

  return 0;
}

static bool predicate(INT a, INT b) {
  return a < b;
}

static int comp(const void *a, const void *b) {
  return *(INT *)a - *(INT *)b;
}
