#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10

static void init_list(INT *a, INT N);
static void merge_sort(INT *a, INT N, bool is_asc, bool verbose);
static void divide_and_merge(INT *a, INT left, INT right, bool is_asc, bool verbose);

static inline void dump(INT *a, INT left, INT right, char *label);

int main (int argc, char **argv) {
  if (argc < 2) {
    fputs("./merge_sort N [asc|desc] [verbose]\n", stderr);
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
    fputs("./merge_sort N [asc|desc] [verbose]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT *a = (INT *)malloc(N * sizeof(INT));

  init_list(a, N);

  dump(a, 0, (N - 1), "Before sort:");

  merge_sort(a, N, is_asc, verbose);

  dump(a, 0, (N - 1), "After sort :");

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

static void merge_sort(INT *a, INT N, bool is_asc, bool verbose) {
  if (N <= 1) {
    return;
  }

  divide_and_merge(a, 0, (N - 1), is_asc, verbose);
}

static void divide_and_merge(INT *a, INT left, INT right, bool is_asc, bool verbose) {
  if (left >= right) {
    return;
  }

  INT middle = ((right - left) / 2) + left;

  divide_and_merge(a, 0, middle, is_asc, verbose);
  divide_and_merge(a, (middle + 1), right, is_asc, verbose);

  INT *t = (INT *)malloc(((right - left) + 1) * sizeof(INT));

  INT i = 0;

  for (INT j = left; j <= middle; j++) {
    t[i++] = a[j];
  }

  for (INT j = right; j >= (middle + 1); j--) {
    t[i++] = a[j];
  }

  INT l = 0;
  INT r = right - left;

  for (INT i = left; i <= right; i++) {
    if (is_asc) {
      // Contains `=` for stable sort
      if (t[l] <= t[r]) {
        a[i] = t[l++];
      } else {
        a[i] = t[r--];
      }
    } else {
      // Contains `=` for stable sort
      if (t[l] >= t[r]) {
        a[i] = t[l++];
      } else {
        a[i] = t[r--];
      }
    }
  }

  if (verbose) {
    printf("dump (%lld, %lld):", left, right);
    dump(a, left, right, "");
  }

  free(t);
}

static inline void dump(INT *a, INT left, INT right, char *label) {
  printf("%s", label);

  for (INT i = left; i <= right; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
