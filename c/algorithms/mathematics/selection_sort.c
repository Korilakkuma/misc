#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VALUE 100

enum {
  ASC,
  DESC
};

static void selection_sort_asc(int a[], long long n);
static void selection_sort_desc(int a[], long long n);

int main(int argc, char **argv) {
  if (argc < 2) {
    fputs("./selection_sort [natural number] [-a|-d]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  int order = 0;

  if ((argc == 3) && (strncmp(argv[2], "-d", 2) == 0)) {
    order = 1;
  }

  int *a = (int *)malloc(n);

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    a[i] = (int)(MAX_VALUE * ((double)rand() / RAND_MAX));
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

  for (long long i = 0; i < n; i++) {
    printf("[%lld] = %d\n", i, a[i]);
  }

  free(a);

  return 0;
}

static void selection_sort_asc(int a[], long long n) {
  for (long long i = 0; i < n; i++) {
    int min = a[i];

    for (long long j = i + 1; j < n; j++) {
      if (a[j] < min) {
        a[i] = a[j];
        a[j] = min;
        min  = a[i];
      }
    }
  }
}

static void selection_sort_desc(int a[], long long n) {
  for (long long i = 0; i < n; i++) {
    int max = a[i];

    for (long long j = i + 1; j < n; j++) {
      if (a[j] > max) {
        a[i] = a[j];
        a[j] = max;
        max  = a[i];
      }
    }
  }
}
