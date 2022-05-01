#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INT long long
#define NMUBER_OF_ITEMS 10
#define MAX_VALUE 100

enum {
  ASC,
  DESC
};

INT a[NMUBER_OF_ITEMS];
INT tmp[NMUBER_OF_ITEMS];

static void merge_sort_asc(INT low, INT high);
static void merge_sort_desc(INT low, INT high);

int main(int argc, char **argv) {
  int order = 0;

  if ((argc == 2) && (strncmp(argv[1], "-d", 2) == 0)) {
    order = 1;
  }

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < NMUBER_OF_ITEMS; i++) {
    a[i] = (INT)(MAX_VALUE * ((double)rand() / (RAND_MAX + 1.0)));
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

  for (INT i = 0; i < NMUBER_OF_ITEMS; i++) {
    printf("[%lld] = %lld\n", i, a[i]);
  }

  return 0;
}

static void merge_sort_asc(INT low, INT high) {
  if (low >= high) {
    return;
  }

  INT middle = (low + high) / 2;

  merge_sort_asc(low, middle);
  merge_sort_asc((middle + 1), high);

  for (INT i = low; i <= middle; i++) {
    tmp[i] = a[i];
  }

  for (INT i = (middle + 1); i <= high; i++) {
    tmp[i] = a[i];
  }

  INT i = low;
  INT j = middle + 1;

  for (INT k = low; k <= high; k++) {
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

static void merge_sort_desc(INT low, INT high) {
  if (low >= high) {
    return;
  }

  INT middle = (low + high) / 2;

  merge_sort_desc(low, middle);
  merge_sort_desc((middle + 1), high);

  for (INT i = low; i <= middle; i++) {
    tmp[i] = a[i];
  }

  for (INT i = (middle + 1); i <= high; i++) {
    tmp[i] = a[i];
  }

  INT i = low;
  INT j = middle + 1;

  for (INT k = low; k <= high; k++) {
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
