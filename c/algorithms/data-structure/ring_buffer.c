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

static bool enqueue(INT queue[], INT front, INT *rear, INT N, INT value);
static INT dequeue(INT queue[], INT *front, INT rear, INT N);
static bool is_empty(INT front, INT rear);
static bool is_full(INT front, INT rear, INT N);

static inline void dump(INT queue[], INT front, INT rear);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./ring_buffer N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *queue = (INT *)malloc(N * sizeof(INT));
  INT front  = 0;  // front
  INT rear   = 0;  // rear

  for (INT i = 0; i < N; i++) {
    queue[i] = EMPTY;
  }

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

        if (!enqueue(queue, front, &rear, N, value)) {
          fputs("Queue is full\n", stderr);
        }

        break;
      }

      case DEQUEUE: {
        INT v;

        if ((v = dequeue(queue, &front, rear, N)) == EMPTY) {
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

static bool enqueue(INT queue[], INT front, INT *rear, INT N, INT value) {
  if (is_full(front, *rear, N)) {
    return false;
  }

  queue[(*rear)++] = value;

  if (*rear >= N) {
    *rear = 0;
  }

  return true;
}

static INT dequeue(INT queue[], INT *front, INT rear, INT N) {
  if (is_empty(*front, rear)) {
    return EMPTY;
  }

  INT v = queue[(*front)++];

  if (*front >= N) {
    *front = 0;
  }

  return v;
}

static bool is_empty(INT front, INT rear) {
  return front == rear;
}

static bool is_full(INT front, INT rear, INT N) {
  return front == ((rear + 1) % N);
}

static inline void dump(INT queue[], INT front, INT rear) {
  printf("dump ring buffer\n");

  if (is_empty(front, rear)) {
    fputs("Queue is empty\n", stderr);
    return;
  }

  INT start = (front < rear) ? front : rear;
  INT end   = (front < rear) ? rear  : front;

  for (INT i = start; i <= end; i++) {
    if (i == front) {
      printf("front --> [%lld] %2lld\n", i, queue[i]);
    } else if (i == rear) {
      printf("rear  --> [%lld]      \n", i);
    } else {
      printf("          [%lld] %2lld\n", i, queue[i]);
    }
  }

  printf("\n");
}
