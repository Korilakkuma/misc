#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define INT long long
#define BUF_SIZE 16

#define EMPTY -1

typedef enum {
  INSERT,
  DELETE,
  EXIT
} HEAP_OP;

static void push(INT *heap, INT x, INT *heap_sp);
static void pop(INT *heap, INT *heap_sp);

static inline INT count_nodes(INT h);
static inline void dump(INT *heap, INT *heap_sp);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./heap N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *heap = (INT *)malloc(N * sizeof(INT));

  INT heap_sp = 0;

  char buf[BUF_SIZE];

  while (true) {
    printf("Type operation 0|1|2 > ");
    fgets(buf, BUF_SIZE, stdin);
    HEAP_OP op = (HEAP_OP)strtoll(buf, NULL, 10);

    switch (op) {
      case INSERT: {
        printf("Type key > ");
        fgets(buf, BUF_SIZE, stdin);
        INT key = strtoll(buf, NULL, 10);

        push(heap, key, &heap_sp);

        dump(heap, &heap_sp);
        break;
      }

      case DELETE: {
        pop(heap, &heap_sp);

        dump(heap, &heap_sp);
        break;
      }

      case EXIT: {
        free(heap);
        exit(EXIT_SUCCESS);
      }

      default: {
        break;
      }
    }
  }

  free(heap);

  return 0;
}

static void push(INT *heap, INT x, INT *heap_sp) {
  heap[(*heap_sp)++] = x;

  INT i = *heap_sp - 1;

  while (i > 0) {
    INT p = (i - 1) / 2;

    if (heap[p] >= x) {
      break;
    }

    heap[i] = heap[p];
    i = p;
  }

  heap[i] = x;
}

static void pop(INT *heap, INT *heap_sp) {
  if (*heap_sp == 0) {
    return;
  }

  INT x = heap[--(*heap_sp)];

  heap[*heap_sp] = EMPTY;

  INT i = 0;

  while (((i * 2) + 1) < *heap_sp) {
    INT child1 = (i * 2) + 1;
    INT child2 = (i * 2) + 2;

    if ((child2 < *heap_sp) && (heap[child2] > heap[child1])) {
      child1 = child2;
    }

    if (heap[child1] <= x) {
      break;
    }

    heap[i] = heap[child1];

    i = child1;
  }

  heap[i] = x;
}

static inline INT count_nodes(INT h) {
  // 2^(h + 1) - 1
  return (INT)(pow(2.0, (double)(h + 1.0)) - 1.0);
}

static inline void dump(INT *heap, INT *heap_sp) {
  if (*heap_sp == 0) {
    printf("Heap is empty.\n");
    return;
  }

  printf("Heap as Array: ");

  INT h = 0;

  for (INT i = 0; i < *heap_sp; i++) {
    if (i == 0) {
      // Root
      printf("[{%2lld}", heap[i]);
    } else if ((i & 0x01) == 1) {
      // Left subtree
      printf("{%2lld, ", heap[i]);
    } else {
      // Right subtree
      printf("%2lld}", heap[i]);
    }

    if ((i == 0) || (i == (count_nodes(h) - 1))) {
      ++h;
      printf("], [");
    }
  }

  printf("\n");
}
