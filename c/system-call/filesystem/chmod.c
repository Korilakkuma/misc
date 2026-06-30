#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./chmod filename permission\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename   = argv[1];
  char *permission = argv[2];

  mode_t mode = (mode_t)strtoul(permission, NULL, 8);

  if (chmod(filename, mode) == -1) {
    perror("chmod");
    exit(EXIT_FAILURE);
  }

  return 0;
}
