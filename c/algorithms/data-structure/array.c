#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10LL

typedef enum {
  INSERT,
  GET,
  UPDATE,
  DELETE
} CRUD;

static INT *insert(INT a[], INT *N, INT index, INT value);
static void update(INT a[], INT index, INT value);
static INT *delete(INT a[], INT *N, INT index);

static inline void dump(INT a[], INT N, char *label);

int main(int argc, char **argv) {
  if (argc < 4) {
    fputs("./array N index 0|1|2|3 [value]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N     = strtoll(argv[1], NULL, 10);
  INT index = strtoll(argv[2], NULL, 10);
  CRUD op   = (CRUD)strtoll(argv[3], NULL, 10);

  if (index < 0) {
    fputs("Must index greater than or equal to 0\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT *array = (INT *)malloc(N * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    array[i] = (INT)((MAX_VALUE) * ((double)rand() / (RAND_MAX + 1.0)));
  }

  dump(array, N, "Before array:");

  switch (op) {
    case INSERT: {
      if (index > N) {
        fprintf(stderr, "Must index less than or equal to %lld\n", N);
        exit(EXIT_FAILURE);
      }

      if (argc != 5) {
        fputs("Require inserted value\n", stderr);
        exit(EXIT_FAILURE);
      }

      array = insert(array, &N, index, strtoll(argv[4], NULL, 10));
      break;
    }

    case GET: {
      if (index >= N) {
        fprintf(stderr, "Must index less than %lld\n", N);
        exit(EXIT_FAILURE);
      }

      printf("array[%lld] -> %lld\n", index, array[index]);
      break;
    }

    case UPDATE: {
      if (index >= N) {
        fprintf(stderr, "Must index less than %lld\n", N);
        exit(EXIT_FAILURE);
      }

      if (argc != 5) {
        fputs("Require updated value\n", stderr);
        exit(EXIT_FAILURE);
      }

      update(array, index, strtoll(argv[4], NULL, 10));
      break;
    }

    case DELETE: {
      if (index >= N) {
        fprintf(stderr, "Must index less than %lld\n", N);
        exit(EXIT_FAILURE);
      }

      array = delete(array, &N, index);
      break;
    }

    default: {
      fputs("./array N index 0|1|2|3 [value]\n", stderr);
      exit(EXIT_FAILURE);
    }
  }

  dump(array, N, "After array :");

  free(array);

  return 0;
}

static INT *insert(INT a[], INT *N, INT index, INT value) {
  a = realloc((void *)a, (++(*N) * sizeof(INT)));

  for (INT i = 0; i < *N; i++) {
    if (i < index) {
      continue;
    }

    if (i == index) {
      for (INT j = (*N - 1); j > index; j--) {
        a[j] = a[j - 1];
      }

      a[index] = value;
    }
  }

  return a;
}

static void update(INT a[], INT index, INT value) {
  a[index] = value;
}

static INT *delete(INT a[], INT *N, INT index) {
  for (INT i = 0; i < *N; i++) {
    if (i < index) {
      continue;
    }

    if (i == index) {
      a = realloc((void *)a, (--(*N) * sizeof(INT)));

      for (INT j = index; j < *N; j++) {
        a[j] = a[j + 1];
      }

      return a;
    }
  }

  return a;
}

static inline void dump(INT a[], INT N, char *label) {
  printf("%s", label);

  for (INT i = 0; i < N; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
