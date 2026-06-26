#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./mkdir dirname\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *dirname = argv[1];

  if (mkdir(dirname, 0777) == -1) {
    perror("mkdir");
    exit(EXIT_FAILURE);
  }

  return 0;
}
