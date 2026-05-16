#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./file_access_mode filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd;
  int mode;

  char *filename = argv[1];

  if ((fd = open(filename, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  mode = fcntl(fd, F_GETFL);

  if ((mode & O_ACCMODE) == O_RDONLY) {
    fprintf(stdout, "file access mode is O_RDONLY (%d).\n", O_RDONLY);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if ((fd = open(filename, O_WRONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  mode = fcntl(fd, F_GETFL);

  if ((mode & O_ACCMODE) == O_WRONLY) {
    fprintf(stdout, "file access mode is O_WRONLY (%d).\n", O_WRONLY);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if ((fd = open(filename, O_RDWR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  mode = fcntl(fd, F_GETFL);

  if ((mode & O_ACCMODE) == O_RDWR) {
    fprintf(stdout, "file access mode is O_RDWR (%d).\n", O_RDWR);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
