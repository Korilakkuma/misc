#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./rmdir dirname\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *dirname = argv[1];

  if (rmdir(dirname) == -1) {
    perror("rmdir");
    exit(EXIT_FAILURE);
  }

  return 0;
}

