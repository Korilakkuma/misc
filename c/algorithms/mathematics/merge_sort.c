#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NMUBER_OF_ITEMS 10
#define MAX_VALUE 100

enum {
  ASC,
  DESC
};

int a[NMUBER_OF_ITEMS];
int tmp[NMUBER_OF_ITEMS];

static void merge_sort_asc(long long low, long long high);
static void merge_sort_desc(long long low, long long high);

int main(int argc, char **argv) {
  int order = 0;

  if ((argc == 2) && (strncmp(argv[1], "-d", 2) == 0)) {
    order = 1;
  }

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < NMUBER_OF_ITEMS; i++) {
    a[i] = (int)(MAX_VALUE * ((double)rand() / RAND_MAX));
  }

  switch (order) {
    case ASC:
      merge_sort_asc(0, (NMUBER_OF_ITEMS - 1));
      break;
    case DESC:
      merge_sort_desc(0, (NMUBER_OF_ITEMS - 1));
      break;
    default:
      break;
  }

  for (long long i = 0; i < NMUBER_OF_ITEMS; i++) {
    printf("[%lld] = %d\n", i, a[i]);
  }

  return 0;
}

static void merge_sort_asc(long long low, long long high) {
  if (low >= high) {
    return;
  }

  long long middle = (low + high) / 2;

  merge_sort_asc(low, middle);
  merge_sort_asc((middle + 1), high);

  for (long long i = low; i <= middle; i++) {
    tmp[i] = a[i];
  }

  for (long long i = (middle + 1); i <= high; i++) {
    tmp[i] = a[i];
  }

  long long i = low;
  long long j = middle + 1;

  for (long long k = low; k <= high; k++) {
    if (tmp[i] <= tmp[j]) {
      a[k] = tmp[i++];
    } else {
      a[k] = tmp[j++];
    }

    if (i > middle) {
      for (; j <= high; j++) {
        a[++k] = tmp[j];
      }

      break;
    } else if (j > high) {
      for (; i <= middle; i++) {
        a[++k] = tmp[i];
      }

      break;
    }
  }
}

static void merge_sort_desc(long long low, long long high) {
  if (low >= high) {
    return;
  }

  long long middle = (low + high) / 2;

  merge_sort_desc(low, middle);
  merge_sort_desc((middle + 1), high);

  for (long long i = low; i <= middle; i++) {
    tmp[i] = a[i];
  }

  for (long long i = (middle + 1); i <= high; i++) {
    tmp[i] = a[i];
  }

  long long i = low;
  long long j = middle + 1;

  for (long long k = low; k <= high; k++) {
    if (tmp[i] >= tmp[j]) {
      a[k] = tmp[i++];
    } else {
      a[k] = tmp[j++];
    }

    if (i > middle) {
      for (; j <= high; j++) {
        a[++k] = tmp[j];
      }

      break;
    } else if (j > high) {
      for (; i <= middle; i++) {
        a[++k] = tmp[i];
      }

      break;
    }
  }
}
