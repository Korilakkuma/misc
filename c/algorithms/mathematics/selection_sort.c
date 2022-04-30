#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 100

enum {
  ASC,
  DESC
};

static void selection_sort_asc(INT a[], INT n);
static void selection_sort_desc(INT a[], INT n);

int main(int argc, char **argv) {
  if (argc < 2) {
    fputs("./selection_sort [natural number] [-a|-d]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  int order = 0;

  if ((argc == 3) && (strncmp(argv[2], "-d", 2) == 0)) {
    order = 1;
  }

  INT *a = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    a[i] = (INT)(MAX_VALUE * ((double)rand() / (RAND_MAX + 1.0)));
  }

  switch (order) {
    case ASC:
      selection_sort_asc(a, n);
      break;
    case DESC:
      selection_sort_desc(a, n);
      break;
    default:
      break;
  }

  for (INT i = 0; i < n; i++) {
    printf("[%lld] = %lld\n", i, a[i]);
  }

  free(a);

  return 0;
}

static void selection_sort_asc(INT a[], INT n) {
  for (INT i = 0; i < n; i++) {
    INT min = a[i];

    for (INT j = i + 1; j < n; j++) {
      if (a[j] < min) {
        a[i] = a[j];
        a[j] = min;
        min  = a[i];
      }
    }
  }
}

static void selection_sort_desc(INT a[], INT n) {
  for (INT i = 0; i < n; i++) {
    INT max = a[i];

    for (INT j = i + 1; j < n; j++) {
      if (a[j] > max) {
        a[i] = a[j];
        a[j] = max;
        max  = a[i];
      }
    }
  }
}
