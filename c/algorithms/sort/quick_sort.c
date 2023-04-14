#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10

static void init_list(INT *a, INT N);
static void quick_sort(INT *a, INT left, INT right, bool is_asc, bool verbose);

static inline void swap(INT *a0, INT *a1);
static inline void dump(INT *a, INT left, INT right, char *label);

int main (int argc, char **argv) {
  if (argc < 2) {
    fputs("./quick_sort N [asc|desc] [verbose]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  bool is_asc  = true;
  bool verbose = false;

  if (argc == 3) {
    is_asc = strncmp(argv[2], "desc", 4) == 0 ? false : true;
  } else if (argc == 4) {
    is_asc  = strncmp(argv[2], "desc", 4) == 0 ? false : true;
    verbose = strncmp(argv[3], "verbose", 7) == 0 ? true : false;
  } else if (argc > 4) {
    fputs("./quick_sort N [asc|desc] [verbose]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT *a = (INT *)malloc(N * sizeof(INT));

  init_list(a, N);

  dump(a, 0, (N - 1), "Before sort   :");

  quick_sort(a, 0, (N - 1), is_asc, verbose);

  dump(a, 0, (N - 1), "After sort    :");

  free(a);

  return 0;
}

static void init_list(INT *a, INT N) {
  srand((unsigned int)time(NULL));

  INT s[] = { -1, 1 };

  for (INT i = 0; i < N; i++) {
    double r = (double)rand() / (RAND_MAX + 1.0);

    a[i] = s[(INT)(2 * r)] * (INT)(MAX_VALUE * r);
  }
}

static void quick_sort(INT *a, INT left, INT right, bool is_asc, bool verbose) {
  if (left >= right) {
    return;
  }

  INT pivot = ((right - left) / 2) + left;

  swap(&a[pivot], &a[right]);

  INT l = left;

  for (INT r = left; r < right; r++) {
    if (is_asc) {
      if (a[r] < a[right]) {
        swap(&a[l++], &a[r]);
      }
    } else {
      if (a[r] > a[right]) {
        swap(&a[l++], &a[r]);
      }
    }
  }

  swap(&a[l], &a[right]);

  if (verbose) {
    printf("dump (%lld, %lld, %lld):", left, l, right);
    dump(a, left, right, "");
  }

  quick_sort(a, left, l, is_asc, verbose);
  quick_sort(a, (l + 1), right, is_asc, verbose);
}

static inline void swap(INT *a0, INT *a1) {
  INT t;

  t   = *a1;
  *a1 = *a0;
  *a0 = t;
}

static inline void dump(INT *a, INT left, INT right, char *label) {
  printf("%s", label);

  for (INT i = left; i <= right; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
