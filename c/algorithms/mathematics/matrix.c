#include <stdio.h>
#include <stdlib.h>

#define INT long long
#define SIZE_OF_MATRIX 2

typedef struct {
  INT m[SIZE_OF_MATRIX][SIZE_OF_MATRIX];
} Matrix;

static void multiply(Matrix *m_1, Matrix *m_2);
static void pow(Matrix *m, INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./matrix [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  Matrix c;

  c.m[0][0] = 1;
  c.m[0][1] = 1;
  c.m[1][0] = 1;
  c.m[1][1] = 0;

  INT n = strtoll(argv[1], NULL, 10);

  pow(&c, n);

  for (INT i = 0; i < SIZE_OF_MATRIX; i++) {
    for (INT j = 0; j < SIZE_OF_MATRIX; j++) {
      printf("%4lld ", c.m[i][j]);
    }

    printf("\n");
  }

  printf("Fibonacci [%lld] = %lld\n", n, (c.m[1][0] + c.m[1][1]));

  return 0;
}

static void multiply(Matrix *m_1, Matrix *m_2) {
  Matrix t;

  for (INT i = 0; i < SIZE_OF_MATRIX; i++) {
    for (INT j = 0; j < SIZE_OF_MATRIX; j++) {
      t.m[i][j] = (m_1->m[i][0] * m_2->m[0][j]) + (m_1->m[i][1] * m_2->m[1][j]);
    }
  }

  for (INT i = 0; i < SIZE_OF_MATRIX; i++) {
    for (INT j = 0; j < SIZE_OF_MATRIX; j++) {
      m_1->m[i][j] = t.m[i][j];
    }
  }
}

static void pow(Matrix *m, INT n) {
  Matrix t;

  for (INT i = 0; i < SIZE_OF_MATRIX; i++) {
    for (INT j = 0; j < SIZE_OF_MATRIX; j++) {
      t.m[i][j] = m->m[i][j];
    }
  }

  for (INT i = 2; i < n; i++) {
    multiply(m, &t);
  }
}
