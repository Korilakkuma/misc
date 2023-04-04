#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INT long long
#define BUF_SIZE 16

typedef INT **Graph;

static void push(Graph graph, INT s, INT d);

// static inline void dump(Graph graph, INT N, INT M, bool directed);

int main(int argc, char **argv) {
  if (argc < 3) {
    fputs("./graph_by_adjacency_list_representation N M", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);
  INT M = strtoll(argv[2], NULL, 10);

  bool directed = false;

  if (argc == 4) {
    directed = strncmp("directed", argv[3], 8) == 0 ? true : false;
  }

  Graph graph = (Graph)malloc(N * sizeof(INT *));

  for (INT i = 0; i < M; i++) {
    graph[i] = (INT *)malloc(M * sizeof(INT));

    char sbuf[BUF_SIZE];
    char dbuf[BUF_SIZE];

    printf("Type source edge > ");
    fgets(sbuf, BUF_SIZE, stdin);
    INT s = strtoll(sbuf, NULL, 10);

    printf("Type destination edge > ");
    fgets(dbuf, BUF_SIZE, stdin);
    INT d = strtoll(dbuf, NULL, 10);

    push(graph, s, d);

    if (!directed) {
      // Undirected Graph
      push(graph, d, s);
    }
  }

  // dump(graph, N, M, directed);

  free(graph);

  return 0;
}

static void push(Graph graph, INT s, INT d) {
  *graph[s] = d;
}

/*
static inline void dump(Graph graph, INT N, INT M, bool directed) {
  for (INT i = 0; i < N; i++) {
    if (directed) {
      if (i == 0) {
        printf("%2lld -> ", i);
      }

      printf("%2lld -> ", graph[i]);
    } else {
      if (i == 0) {
        printf("%2lld - ", i);
      }

      printf("%2lld - ", graph[i]);
    }
  }

*/
