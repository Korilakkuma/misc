#include <stdio.h>
#include <stdlib.h>
#include "VirtualMachine.h"

#define SIZE_OF_MEMORY 100

static char memory[SIZE_OF_MEMORY];

static int memory_index;
static int program_counter;
static int stack_pointer;

static void execute(void);

static void load(char *filename);
static void push(char data);
static char pop(void);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./virtual_stack_machine filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  load(argv[1]);

  program_counter = 0;
  stack_pointer = SIZE_OF_MEMORY - 1;

  execute();

  fprintf(stdout, "result = %d\n", pop());

  return 0;
}

static void execute(void) {
  while (program_counter < memory_index) {
    if (memory[program_counter] == PUSH) {
      push(memory[++program_counter]);
    } else {
      char operand2 = pop();
      char operand1 = pop();

      switch (memory[program_counter]) {
        case ADD: {
          push((char)(operand1 + operand2));
          break;
        }

        case SUB: {
          push((char)(operand1 - operand2));
          break;
        }

        case MUL: {
          push((char)(operand1 * operand2));
          break;
        }

        case DIV: {
          if (operand2 == 0) {
            fputs("Zero Division Error !\n", stderr);
            exit(EXIT_FAILURE);
          }

          push((char)(operand1 / operand2));
          break;
        }
      }
    }

    ++program_counter;
  }
}

static void load(char *filename) {
  FILE *fp = fopen(filename, "r");

  int ch;

  memory_index = 0;

  while ((ch = fgetc(fp)) != EOF) {
    memory[memory_index++] = (char)ch;
  }

  fclose(fp);
}

static void push(char data) {
  memory[stack_pointer--] = data;
}

static char pop(void) {
  return memory[++stack_pointer];
}
