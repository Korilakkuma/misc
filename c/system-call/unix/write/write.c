#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./write ttyname\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd;

  ssize_t read_bytes;

  char buf[1024];

  char *ttyname = argv[1];

  if ((fd = open(ttyname, O_WRONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while ((read_bytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    write(fd, buf, read_bytes);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
