#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./isatty filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd;

  char *filename = argv[1];

  if ((fd = open(filename, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  int is_tty = isatty(fd);

  if (is_tty) {
    fprintf(stdout, "%s is tty.", filename);
  } else {
    fprintf(stdout, "%s is not tty (regular file).", filename);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
