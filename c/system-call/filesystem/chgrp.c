#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./chgrp group_id filename", stderr);
    exit(EXIT_FAILURE);
  }

  gid_t gid      = (unsigned int)strtoul(argv[1], NULL, 10);
  char *filename = argv[2];

  if (chown(filename, -1, gid) == -1) {
    perror("chown");
    exit(EXIT_FAILURE);
  }

  return 0;
}
