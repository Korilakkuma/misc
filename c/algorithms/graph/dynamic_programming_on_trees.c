#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long
#define EMPTY -1

typedef int Graph[];

static void breadth_first_search(Graph *g, INT number_of_vertices, INT s);

static inline void init_list(bool *seen, INT N);
static inline void enqueue(INT *todo, INT *rear, INT v);
static inline INT dequeue(INT *todo, INT *front);
static inline bool empty(INT *todo, INT front, INT rear);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./bfs N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  Graph *g = (Graph *)malloc(N);

  free(g);

  return 0;
}

static void breadth_first_search(Graph *g, INT number_of_vertices, INT s) {
  bool *seen = (bool *)malloc(number_of_vertices * sizeof(INT));
  INT *todo  = (INT *)malloc(number_of_vertices * sizeof(INT));

  INT front = 0;
  INT rear  = 0;

  init_list(seen, number_of_vertices);

  seen[s] = true;
  enqueue(todo, &rear, s);

  while (!empty(todo, front, rear)) {
    INT c = dequeue(todo, &front);

    for (INT i = 0; i < g[c]; i++) {
      INT n = g[c][i];

      if (seen[n]) {
        continue;
      }

      seen[n] = true;
      enqueue(todo, &rear, n);
    }
  }

  free(seen);
  free(todo);
}

static inline void init_list(bool *seen, INT N) {
  for (INT i = 0; i < N; i++) {
    seen[i] = false;
  }
}

static inline void enqueue(INT *todo, INT *rear, INT v) {
  todo[(*rear)++] = v;
}

static inline INT dequeue(INT *todo, INT *front) {
  return todo[(*front)++];
}

static inline bool empty(INT *todo, INT front, INT rear) {
  return front == rear;
}
