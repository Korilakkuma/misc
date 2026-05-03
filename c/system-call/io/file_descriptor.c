#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./file_descriptor filename", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd;

  if ((fd = open(filename, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "file descriptor is %d.\n", fd);

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  fputs("close file descriptor.\n", stdout);

  return 0;
}
