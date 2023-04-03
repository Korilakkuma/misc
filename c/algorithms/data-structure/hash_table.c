#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long
#define BUF_SIZE 16

#define EMPTY -1

typedef enum {
  INSERT,
  GET,
  UPDATE,
  DELETE,
  EXIT
} CRUD;

static INT hash(INT key, INT N);
static void init_hash_table(INT hash_table[], INT N);

static inline void dump(INT hash_table[], INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./hash_table N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *hash_table = (INT *)malloc(N * sizeof(INT));

  init_hash_table(hash_table, N);

  char buf[BUF_SIZE];

  while (true) {
    printf("Type operation 0|1|2|3|4 > ");
    fgets(buf, BUF_SIZE, stdin);
    CRUD op = (CRUD)strtoll(buf, NULL, 10);

    if (op > EXIT) {
      continue;
    }

    if (op == EXIT) {
      free(hash_table);
      exit(EXIT_SUCCESS);
    }

    printf("Type key > ");
    fgets(buf, BUF_SIZE, stdin);
    INT key = strtoll(buf, NULL, 10);

    switch (op) {
      case INSERT: {
        printf("Type inserted value > ");
        fgets(buf, BUF_SIZE, stdin);
        INT value = strtoll(buf, NULL, 10);

        INT k = hash(key, N);

        if (hash_table[k] == EMPTY) {
          hash_table[k] = value;
        } else {
          printf("%lld already exists (collision occurred)\n", key);
        }

        break;
      }

      case GET: {
        INT k = hash(key, N);

        if (hash_table[k] == EMPTY) {
          printf("%lld does not exist\n", key);
        } else {
          printf("%lld exists (value = %lld)\n", key, hash_table[k]);
        }

        break;
      }

      case UPDATE: {
        printf("Type update value > ");
        fgets(buf, BUF_SIZE, stdin);
        INT value = strtoll(buf, NULL, 10);

        INT k = hash(key, N);

        if (hash_table[k] == EMPTY) {
          printf("%lld does not exist\n", key);
        } else {
          hash_table[k] = value;
        }

        break;
      }

      case DELETE: {
        INT k = hash(key, N);

        if (hash_table[k] == EMPTY) {
          printf("%lld does not exist\n", key);
        } else {
          hash_table[k] = EMPTY;
        }

        break;
      }

      case EXIT: {
        free(hash_table);
        exit(EXIT_SUCCESS);
      }

      default: {
        break;
      }
    }

    dump(hash_table, N);
  }

  free(hash_table);

  return 0;
}

static INT hash(INT key, INT N) {
  return key % N;
}

static void init_hash_table(INT hash_table[], INT N) {
  for (INT i = 0; i < N; i++) {
    hash_table[i] = EMPTY;
  }
}

static inline void dump(INT hash_table[], INT N) {
  printf("dump hash table\n");

  for (INT i = 0; i < N; i++) {
    printf("[%2lld] -> %2lld\n", i, hash_table[i]);
  }

  printf("\n");
}
