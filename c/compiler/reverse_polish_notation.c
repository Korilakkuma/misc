#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE_OF_STACK     20
#define SIZE_OF_STATEMENT 80

static void polish(char *s);

static void push(int n);
static int pop(void);

static int execute(void);
static int order(int ch);
static int get_value(int ch);

int stack[SIZE_OF_STACK];
int stack_pointer = 0;

char polish_as_string[SIZE_OF_STATEMENT];

int main(int argc, char **argv) {
  char statement[SIZE_OF_STATEMENT];

  fputs("Type statement > ", stdout);

  while (fgets(statement, SIZE_OF_STATEMENT, stdin) != NULL) {
    polish(statement);

    if (polish_as_string[0] == '\0') {
      exit(EXIT_FAILURE);
    }

    int r = execute();

    fprintf(stdout, "Reverse Polish Notation: %s = %d\n", polish_as_string, r);
    fputs("Type statement > ", stdout);
  }

  return 0;
}

static void polish(char *s) {
  char *out = polish_as_string;

  stack_pointer = 0;

  while (true) {
    while (isspace(*s)) {
      ++s;
    }

    if ((*s == '\0') || (*s == '\r') || (*s == '\n')) {
      while (stack_pointer > 0) {
        if ((*out++ = pop()) == '(') {
          fputs("'(' is invalid\n", stderr);
          exit(EXIT_FAILURE);
        }
      }

      break;
    }

    if (islower(*s) || isdigit(*s)) {
      *out++ = *s++;
      continue;
    }

    int w;

    switch (*s) {
      case '(':
        push(*s);
        break;
      case ')':
        while ((w = pop()) != '(') {
          *out++ = w;

          if (stack_pointer == 0) {
            fputs("'(' is required\n", stderr);
            exit(EXIT_FAILURE);
          }
        }

        break;
      default:
        if (order(*s) == -1) {
          fprintf(stderr, "Invalid character (%c)\n", *s);
          exit(EXIT_FAILURE);
        }

        while (stack_pointer > 0) {
          if (order(stack[stack_pointer - 1]) < order(*s)) {
            break;
          }

          *out++ = pop();
        }

        push(*s);
        break;
    }

    ++s;
  }

  *out = '\0';
}

static void push(int n) {
  if (stack_pointer >= SIZE_OF_STACK) {
    fputs("Stack overflow\n", stderr);
    exit(EXIT_FAILURE);
  }

  stack[stack_pointer++] = n;
}

static int pop(void) {
  if (stack_pointer <= 0) {
    fputs("Stack is empty\n", stderr);
    exit(EXIT_FAILURE);
  }

  return stack[--stack_pointer];
}

static int execute(void) {
  stack_pointer = 0;

  for (char *s = polish_as_string; *s; s++) {
    if (islower(*s)) {
      push(get_value(*s));
    } else if (isdigit(*s)) {
      push(*s - '0');
    } else {
      int op2 = pop();
      int op1 = pop();

      switch (*s) {
        case '+': push(op1 + op2); break;
        case '-': push(op1 - op2); break;
        case '*': push(op1 * op2); break;
        case '/': {
          if (op2 == 0) {
            fputs("Zero Division Error\n", stderr);
            exit(EXIT_FAILURE);
          }

          push(op1 / op2);
          break;
        }

        default:
          break;
      }
    }
  }

  if (stack_pointer != 1) {
    fprintf(stderr, "Error occurred (Notation is '%s'. Stack pointer is = %d)\n", polish_as_string, stack_pointer);
    exit(EXIT_FAILURE);
  }

  return pop();
}

static int order(int ch) {
  switch (ch) {
    case '*':
    case '/':
      return 3;
    case '+':
    case '-':
      return 2;
    case '(':
      return 1;
    default:
      return -1;
  }
}

static int get_value(int ch) {
  if (islower(ch)) {
    return (ch - 'a') + 1;
  }

  return 0;
}
