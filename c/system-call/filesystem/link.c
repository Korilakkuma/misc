#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./link src dest\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *src  = argv[1];
  char *dest = argv[2];

  if (link(src, dest) == -1) {
    perror("link");
    exit(EXIT_FAILURE);
  }

  return 0;
}
