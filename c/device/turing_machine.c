#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  INPUT_0,
  INPUT_1
} INPUTS;

typedef enum {
  STATE_0,
  STATE_1,
  STATE_2,
  STATE_3,
  STATE_4
} MACHINE_STATE;

typedef long INT;

static INT tape[100];
static MACHINE_STATE current_state = STATE_0;
static int head_position;
static int store_size;

static void print_machine_state(void);

int main(int argc, char **argv) {
  int tape_position = 0;

  char buf[80];

  fputs("Type the 1st operand > ", stdout);
  fgets(buf, sizeof(buf), stdin);

  INT operand1 = strtol(buf, NULL, 10);

  fputs("Type the 2nd operand > ", stderr);
  fgets(buf, sizeof(buf), stdin);

  INT operand2 = strtol(buf, NULL, 10);

  tape[tape_position] = INPUT_0;

  for (tape_position = 1; tape_position <= (operand1 + 1); tape_position++) {
    tape[tape_position] = INPUT_1;
  }

  tape[tape_position] = INPUT_0;

  for (tape_position = (operand1 + 3); tape_position <= (operand1 + 3 + operand2); tape_position++) {
    tape[tape_position] = INPUT_1;
  }

  tape[tape_position] = INPUT_0;

  current_state = STATE_0;
  head_position = 1;
  store_size    = tape_position + 1;

  print_machine_state();
  fgetc(stdin);

  while (current_state != STATE_4) {
    switch (current_state) {
      case STATE_0: {
        switch (tape[head_position]) {
          case INPUT_0: {
            current_state = STATE_1;
            tape[head_position] = INPUT_1;

            break;
          }

          case INPUT_1: {
            ++head_position;

            break;
          }
        }

        break;
      }

      case STATE_1: {
        switch (tape[head_position]) {
          case INPUT_0: {
            current_state = STATE_2;
            ++head_position;

            break;
          }

          case INPUT_1: {
            --head_position;

            break;
          }
        }

        break;
      }

      case STATE_2: {
        switch (tape[head_position]) {
          case INPUT_0: {
            current_state = STATE_3;
            ++head_position;

            break;
          }

          case INPUT_1: {
            tape[head_position] = INPUT_0;

            break;
          }
        }

        break;
      }

      case STATE_3: {
        switch (tape[head_position]) {
          case INPUT_0: {
            current_state = STATE_4;
            ++head_position;

            break;
          }

          case INPUT_1: {
            tape[head_position] = INPUT_0;

            break;
          }
        }

        break;
      }

      case STATE_4: {
        break;
      }
    }

    print_machine_state();
    fgetc(stdin);
  }

  print_machine_state();
  fgetc(stdin);

  return 0;
}

static void print_machine_state(void) {
  fprintf(stdout, "state = %d\n", current_state);

  for (int i = 0; i < store_size; i++) {
    fprintf(stdout, "[%ld]", tape[i]);
  }

  fputs("\n", stdout);

  for (int i = 0; i < store_size; i++) {
    if (head_position == i) {
      fputs(" | ", stdout);
    } else {
      fputs("   ", stdout);
    }
  }

  fputs("\n", stdout);
}
