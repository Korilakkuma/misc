#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT long long
#define BUF_SIZE 16

#define EMPTY -1

typedef enum {
  PUSH,
  POP,
  EXIT
} STACK_OP;

static bool push(INT stack[], INT *sp, INT v, INT N);
static INT pop(INT stack[], INT *sp);
static bool is_empty(INT sp);
static bool is_full(INT sp, INT N);

static inline void dump(INT stack[], INT sp);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./stack N", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *stack = (INT *)malloc(N * sizeof(INT));
  INT sp     = 0;

  char buf[BUF_SIZE];

  while (true) {
    printf("Type operation 0|1|2 > ");
    fgets(buf, BUF_SIZE, stdin);
    STACK_OP op = (STACK_OP)strtoll(buf, NULL, 10);

    if (op > EXIT) {
      continue;
    }

    if (op == EXIT) {
      free(stack);
      exit(EXIT_SUCCESS);
    }

    switch (op) {
      case PUSH: {
        printf("Type value > ");
        fgets(buf, BUF_SIZE, stdin);
        INT value = strtoll(buf, NULL, 10);

        if (!push(stack, &sp, value, N)) {
          fputs("Stack is full\n", stderr);
        }

        break;
      }

      case POP: {
        if (is_empty(sp)) {
          break;
        }

        INT v;

        if ((v = pop(stack, &sp)) == EMPTY) {
          fputs("Stack is empty\n", stderr);
        } else {
          printf("Popped value is %lld\n", v);
        }

        break;
      }

      case EXIT: {
        free(stack);
        exit(EXIT_SUCCESS);
      }

      default: {
        break;
      }
    }

    dump(stack, sp);
  }

  free(stack);

  return 0;
}

static bool push(INT stack[], INT *sp, INT v, INT N) {
  if (is_full(*sp, N)) {
    return false;
  }

  stack[(*sp)++] = v;

  return true;
}

static INT pop(INT stack[], INT *sp) {
  if (is_empty(*sp)) {
    return EMPTY;
  }

  INT v = stack[--(*sp)];

  stack[*sp] = EMPTY;

  return v;
}

static bool is_empty(INT sp) {
  return sp <= 0;
}

static bool is_full(INT sp, INT N) {
  return sp >= N;
}

static inline void dump(INT stack[], INT sp) {
  printf("dump stack:\n");

  printf("sp[%lld] --> empty\n", sp);

  for (INT p = sp - 1; p >= 0; p--) {
    printf("[%lld] %2lld\n", p, stack[p]);
  }

  printf("\n");
}
