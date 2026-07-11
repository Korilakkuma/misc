#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    fputs("./redirect_to_file filename command ...\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd;

  if ((fd = open(filename, (O_WRONLY | O_CREAT | O_TRUNC), 0666)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if (dup2(fd, STDOUT_FILENO) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }

  execvp(argv[2], &argv[2]);

  return 0;
}
