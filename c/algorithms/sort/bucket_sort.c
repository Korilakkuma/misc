#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10
#define BUCKET_SIZE 20

static void init_list(INT *a, INT *c, INT *s, INT N);
static void bucket_sort(INT *a, INT *c, INT *s, INT N, bool is_asc, bool verbose);

static inline void dump(INT *a, INT N, char *label);

int main (int argc, char **argv) {
  if (argc < 2) {
    fputs("./bucket_sort N [asc|desc] [verbose]\n", stderr);
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
    fputs("./bucket_sort N [asc|desc] [verbose]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT *a = (INT *)malloc(N * sizeof(INT));
  INT *c = (INT *)malloc(BUCKET_SIZE * sizeof(INT));
  INT *s = (INT *)malloc(BUCKET_SIZE * sizeof(INT));

  init_list(a, c, s, N);

  dump(a, N, "Before sort:");

  bucket_sort(a, c, s, N, is_asc, verbose);

  dump(a, N, "After sort :");

  free(a);
  free(c);
  free(s);

  return 0;
}

static void init_list(INT *a, INT *c, INT *s, INT N) {
  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    a[i] = (INT)(MAX_VALUE * ((double)rand() / (RAND_MAX + 1.0)));
  }

  for (INT i = 0; i < BUCKET_SIZE; i++) {
    c[i] = 0;
    s[i] = 0;
  }
}

static void bucket_sort(INT *a, INT *c, INT *s, INT N, bool is_asc, bool verbose) {
  for (INT i = 0; i < N; i++) {
    ++c[a[i]];
  }

  if (verbose) {
    printf("Count      :");

    for (INT i = 0; i < N; i++) {
      printf(" %2lld", c[a[i]]);
    }

    printf("\n");
  }

  if (is_asc) {
    for (INT i = 0; i < BUCKET_SIZE; i++) {
      if (i == 0) {
        s[i] = c[i];
        continue;
      }

      s[i] = s[i - 1] + c[i];
    }
  } else {
    for (INT i = (BUCKET_SIZE - 1); i >= 0; i--) {
      if (i == (BUCKET_SIZE - 1)) {
        s[i] = c[i];
        continue;
      }

      s[i] = s[i + 1] + c[i];
    }
  }

  if (verbose) {
    printf("Sum        :");

    for (INT i = 0; i < BUCKET_SIZE; i++) {
      printf(" %2lld", s[i]);
    }

    printf("\n");
  }

  INT *t = (INT *)malloc(N * sizeof(INT));

  for (INT i = (N - 1); i >= 0; i--) {
    t[--s[a[i]]] = a[i];
  }

  // copy
  for (INT i = 0; i < N; i++) {
    a[i] = t[i];
  }

  free(t);
}

static inline void dump(INT *a, INT N, char *label) {
  printf("%s", label);

  for (INT i = 0; i < N; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
