#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long
#define NUMBER_OF_NODES 100

INT A[NUMBER_OF_NODES];
INT B[NUMBER_OF_NODES];
INT G[NUMBER_OF_NODES][NUMBER_OF_NODES];
INT D[NUMBER_OF_NODES];  // Dimension
INT visited[NUMBER_OF_NODES];

static void dfs(INT pos);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./depth_first_search [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT e = strtoll(argv[2], NULL, 10);

  for (INT i = 0; i < NUMBER_OF_NODES; i++) {
    D[i] = 0;
  }

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < e; i++) {
    A[i] = (INT)(e * ((double)rand() / (RAND_MAX + 1.0)));
    B[i] = (INT)(e * ((double)rand() / (RAND_MAX + 1.0)));

    G[A[i]][D[A[i]]++] = B[i];
    G[B[i]][D[B[i]]++] = A[i];
  }

  for (INT i = 0; i < n; i++) {
    printf("[%lld] (", i);

    for (INT j = 0; j < D[i]; j++) {
      printf(" %lld", G[i][j]);
    }

    printf(" )\n");
  }

  dfs(0);

  bool result = true;

  for (INT i = 0; i < n; i++) {
    if (!visited[i]) {
      result = false;
    }
  }

  if (result) {
    printf("Connected Graph\n");
  } else {
    printf("Not connected Graph\n");
  }

  return 0;
}

static void dfs(INT pos) {
  visited[pos] = true;

  for (INT i = 0; i < D[pos]; i++) {
    INT n = G[pos][i];

    if (!visited[n]) {
      dfs(n);
    }
  }
}
