#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define INT long long
#define N 10
#define RANGE 10

static INT lower_bound(INT *a, INT k);
static bool predicate(INT a, INT b);
static int comp(const void *a, const void *b);
static inline void dump(INT *a, INT n, char *label);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./subset_sum_problem [0-9]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT K = strtoll(argv[1], NULL, 10);

  INT a[N];
  INT b[N];

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    a[i] = (INT)(RANGE * ((double)rand() / (RAND_MAX + 1.0)));
    b[i] = (INT)(RANGE * ((double)rand() / (RAND_MAX + 1.0)));
  }

  qsort((void *)b, (size_t)N, sizeof(INT), comp);

  INT i = 0;

  INT min_subset_sum = UINT32_MAX;
  INT min_a_index    = -1;
  INT min_b_index    = -1;

  for (i = 0; i < N; i++) {
    INT diff = K - a[i];

    if (diff < 0) {
      continue;
    }

    INT j = lower_bound(b, diff);

    INT sum = a[i] + b[j];

    if ((sum < min_subset_sum) && (sum >= K)) {
      min_subset_sum = sum;
      min_a_index    = i;
      min_b_index    = j;
    }
  }

  printf("(a[%lld], b[%lld]) = (%lld, %lld)\n", min_a_index, min_b_index, a[min_a_index], b[min_b_index]);

  dump(a, N, "a");
  dump(b, N, "b");

  return 0;
}

static INT lower_bound(INT *a, INT k) {
  INT left  = 0;
  INT right = N - 1;

  while ((right - left) > 1) {
    INT middle = (right + left) / 2;

    if (predicate(a[middle], k)) {
      left = middle;
    } else {
      right = middle;
    }
  }

  return right;
}

static bool predicate(INT a, INT b) {
  return a < b;
}

static int comp(const void *a, const void *b) {
  return *(INT *)a - *(INT *)b;
}

static inline void dump(INT *a, INT n, char *label) {
  printf("%s: ", label);

  for (INT i = 0; i < n; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
