#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  if (truncate(filename, (off_t)0) == -1) {
    perror("truncate");
    exit(EXIT_FAILURE);
  }

  return 0;
}
