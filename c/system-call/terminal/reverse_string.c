#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./reverse_string string\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *string = argv[1];

  fprintf(stdout, "[7m%s[0m\n", string);

  return 0;
}
