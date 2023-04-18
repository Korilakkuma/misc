#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10

static void init_list(INT *a, INT N);
static void heap_sort(INT *a, INT i, INT N, bool is_asc, bool verbose);
static void heap(INT *a, INT i, INT N);

static inline void swap(INT *a0, INT *a1);
static inline void dump(INT *a, INT N, char *label);

int main (int argc, char **argv) {
  if (argc < 2) {
    fputs("./heap_sort N [asc|desc] [verbose]\n", stderr);
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
    fputs("./heap_sort N [asc|desc] [verbose]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT *a = (INT *)malloc(N * sizeof(INT));

  init_list(a, N);

  dump(a, N, "Before sort   :");

  heap_sort(a, 0, (N - 1), is_asc, verbose);

  dump(a, N, "After sort    :");

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

static void heap_sort(INT *a, INT i, INT N, bool is_asc, bool verbose) {
  for (INT i = ((N / 2) - 1); i >= 0; i--) {
    heap(a, i, N);
  }

  for (INT i = (N - 1); i > 0; i--) {
    swap(&a[0], &a[i]);
    heap(a, 0, i);
  }
}

static void heap(INT *a, INT i, INT N) {
  INT left_child = (i * 2) + 1;

  if (left_child >= N) {
    return;
  }

  INT next = left_child + 1;

  if ((next < N) && a[next] > a[left_child]) {
    ++left_child;
  }

  if (a[left_child] <= a[i]) {
    return;
  }

  swap(&a[i], &a[left_child]);

  heap(a, left_child, N);
}

static inline void swap(INT *a0, INT *a1) {
  INT t;

  t   = *a1;
  *a1 = *a0;
  *a0 = t;
}

static inline void dump(INT *a, INT N, char *label) {
  printf("%s", label);

  for (INT i = 0; i < N; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
