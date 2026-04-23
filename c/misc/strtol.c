#include <stdio.h>
#include <stdlib.h>
// #include <limits.h>
#include <errno.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./strtol number_string\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *end;

  errno = 0;

  long number = strtol(argv[1], &end, 10);

  if (errno != 0) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }

  if (end[0] != '\0') {
    fprintf(stderr, "Non-digit character is '%c'.\n", end[0]);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "number is %ld.\n", number);

  return 0;
}
