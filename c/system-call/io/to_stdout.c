#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./to_stdout filename\n", stdout);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd;

  size_t size = 64;

  char buf[size];

  ssize_t byte;

  if ((fd = open(filename, O_RDONLY)) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  while ((byte = read(fd, buf, size)) > 0) {
    if (write(STDOUT_FILENO, buf, size) != size) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  if (byte == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
