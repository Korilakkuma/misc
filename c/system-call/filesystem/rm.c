#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./rm pathname\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *pathname = argv[1];

  if (unlink(pathname) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }

  return 0;
}
