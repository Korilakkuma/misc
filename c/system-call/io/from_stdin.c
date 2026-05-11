#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./from_stdin filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd;

  size_t size = 64;

  char buf[size];

  ssize_t byte;

  if ((fd = open(filename, (O_WRONLY | O_CREAT | O_TRUNC), 0666)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while ((byte = read(STDIN_FILENO, buf, size)) > 0) {
    if (write(fd, buf, byte) != byte) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  if (byte == 0) {
    fputs("EOF (pressed 'Ctrl + D')\n", stdout);
  }

  if (byte == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
