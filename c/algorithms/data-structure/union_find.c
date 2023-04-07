#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long
#define BUF_SIZE 16

#define INIT_VALUE -1

typedef enum {
  UNITE,
  ISSAME,
  SIZE,
  EXIT
} UNION_FIND_OP;

struct UnionFind {
  INT *parents;
  INT *number_of_vertices;
};

static void init_union_find(struct UnionFind *union_find, INT N);
static INT root(INT *parents, INT x);
static bool issame(INT *parents, INT x, INT y);
static bool unite(INT *parents, INT *number_of_vertices, INT x, INT y);
static INT size(INT *parents, INT *number_of_vertices, INT x);

static inline void swap(INT *x, INT *y);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./union_find N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  struct UnionFind union_find;

  union_find.parents            = (INT *)malloc(N * sizeof(INT));
  union_find.number_of_vertices = (INT *)malloc(N * sizeof(INT));

  init_union_find(&union_find, N);

  char op_buf[BUF_SIZE];
  char x_buf[BUF_SIZE];
  char y_buf[BUF_SIZE];

  while (true) {
    printf("Type operation 0|1|2|3 > ");
    fgets(op_buf, BUF_SIZE, stdin);
    UNION_FIND_OP op = (UNION_FIND_OP)strtoll(op_buf, NULL, 10);

    if (op == EXIT) {
      free(union_find.parents);
      free(union_find.number_of_vertices);
      exit(EXIT_SUCCESS);
    }

    printf("Type x > ");
    fgets(x_buf, BUF_SIZE, stdin);
    INT x = strtoll(x_buf, NULL, 10);

    switch (op) {
      case UNITE: {
        printf("Type y > ");
        fgets(y_buf, BUF_SIZE, stdin);
        INT y = strtoll(y_buf, NULL, 10);

        unite(union_find.parents, union_find.number_of_vertices, x, y);
        break;
      }

      case ISSAME: {
        printf("Type y > ");
        fgets(y_buf, BUF_SIZE, stdin);
        INT y = strtoll(y_buf, NULL, 10);

        printf("issame(%2lld, %2lld) is %s\n", x, y, (issame(union_find.parents, x, y) ? "true" : "false"));
        break;
      }

      case SIZE: {
        printf("%2lld size is %2lld\n", x, size(union_find.parents, union_find.number_of_vertices, x));
        break;
      }

      case EXIT: {
      }

      default: {
        break;
      }
    }
  }

  free(union_find.parents);
  free(union_find.number_of_vertices);

  return 0;
}

static void init_union_find(struct UnionFind *union_find, INT N) {
  for (INT i = 0; i < N; i++) {
    union_find->parents[i] = INIT_VALUE;
    union_find->number_of_vertices[i] = 1;
  }
}

static INT root(INT *parents, INT x) {
  if (parents[x] == INIT_VALUE) {
    return x;
  }

  return parents[x] = root(parents, parents[x]);
}

static bool issame(INT *parents, INT x, INT y) {
  return root(parents, x) == root(parents, y);
}

static bool unite(INT *parents, INT *number_of_vertices, INT x, INT y) {
  INT rx = root(parents, x);
  INT ry = root(parents, y);

  if (rx == ry) {
    return false;
  }

  if (number_of_vertices[rx] < number_of_vertices[ry]) {
    swap(&rx, &ry);
  }

  parents[y] = x;

  return true;
}

static INT size(INT *parents, INT *number_of_vertices, INT x) {
  return number_of_vertices[root(parents, x)];
}

static inline void swap(INT *x, INT *y) {
  INT t;

  t  = *x;
  *x = *y;
  *y = t;
}
