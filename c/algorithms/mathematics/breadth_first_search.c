#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long
#define NUMBER_OF_NODES 100
#define UNREACHED -1
#define QUEUE_EMPTY -1

INT A[NUMBER_OF_NODES];
INT B[NUMBER_OF_NODES];
INT G[NUMBER_OF_NODES][NUMBER_OF_NODES];
INT D[NUMBER_OF_NODES];  // Dimension
INT distance[NUMBER_OF_NODES];

INT Q[NUMBER_OF_NODES];
INT EP = 0;  // for queue rear (enqueue)
INT DP = 0;  // for queue front (dequeue)

static void bfs(void);
static inline bool empty(void);
static inline void enqueue(INT n);
static inline INT dequeue(void);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./breadth_first_search [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);
  INT e = strtoll(argv[2], NULL, 10);

  for (INT i = 0; i < NUMBER_OF_NODES; i++) {
    D[i]        = 0;
    Q[i]        = QUEUE_EMPTY;
    distance[i] = UNREACHED;
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

  INT s = 0;

  enqueue(s);
  distance[s] = 0;

  bfs();

  for (INT i = 0; i < n; i++) {
    printf("[%lld] %lld\n", i, distance[i]);
  }

  return 0;
}

static void bfs(void) {
  if (empty()) {
    return;
  }

  INT p = dequeue();

  for (INT i = 0; i < D[p]; i++) {
    INT n = G[p][i];

    if (distance[n] == UNREACHED) {
      distance[n] = distance[p] + 1;
      enqueue(n);
    }
  }

  bfs();
}

static inline bool empty(void) {
  return DP == EP;
}

static inline void enqueue(INT n) {
  Q[EP++] = n;
}

static inline INT dequeue(void) {
  return Q[DP++];
}
