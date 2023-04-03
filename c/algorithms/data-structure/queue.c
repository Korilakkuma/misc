#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long
#define BUF_SIZE 16

#define EMPTY -1

typedef enum {
  ENQUEUE,
  DEQUEUE,
  EXIT
} QUEUE_OP;

static bool enqueue(INT queue[], INT *rear, INT N, INT value);
static INT dequeue(INT queue[], INT *front, INT rear);
static bool is_empty(INT front, INT rear);
static bool is_full(INT rear, INT N);

static inline void dump(INT queue[], INT front, INT rear);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./queue N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *queue = (INT *)malloc(N * sizeof(INT));
  INT front  = 0;
  INT rear   = 0;

  char buf[BUF_SIZE];

  while (true) {
    printf("Type operation 0|1|2 > ");
    fgets(buf, BUF_SIZE, stdin);
    QUEUE_OP op = (QUEUE_OP)strtoll(buf, NULL, 10);

    if (op > EXIT) {
      continue;
    }

    if (op == EXIT) {
      free(queue);
      exit(EXIT_SUCCESS);
    }

    switch (op) {
      case ENQUEUE: {
        printf("Type value > ");
        fgets(buf, BUF_SIZE, stdin);
        INT value = strtoll(buf, NULL, 10);

        if (!enqueue(queue, &rear, N, value)) {
          fputs("Queue is full\n", stderr);
        }

        break;
      }

      case DEQUEUE: {
        INT v;

        if ((v = dequeue(queue, &front, rear)) == EMPTY) {
          fputs("Queue is empty\n", stderr);
        } else {
          printf("Dequeued value is %lld\n", v);
        }

        break;
      }

      case EXIT: {
        free(queue);
        exit(EXIT_SUCCESS);
      }

      default: {
        break;
      }
    }

    dump(queue, front, rear);
  }

  free(queue);

  return 0;
}

static bool enqueue(INT queue[], INT *rear, INT N, INT value) {
  if (is_full(*rear, N)) {
    return false;
  }

  queue[(*rear)++] = value;

  return true;
}

static INT dequeue(INT queue[], INT *front, INT rear) {
  if (is_empty(*front, rear)) {
    return EMPTY;
  }

  return queue[(*front)++];
}

static bool is_empty(INT front, INT rear) {
  return front == rear;
}

static bool is_full(INT rear, INT N) {
  return rear >= N;
}

static inline void dump(INT queue[], INT front, INT rear) {
  printf("dump queue\n");

  for (INT i = front; i < rear; i++) {
    printf("[%lld] %2lld\n", i, queue[i]);
  }

  printf("\n");
}
