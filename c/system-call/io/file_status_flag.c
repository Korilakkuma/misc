#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./file_status_flag filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd;
  int flags;

  char *filename = argv[1];

  if ((fd = open(filename, (O_APPEND | O_NONBLOCK))) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  flags = fcntl(fd, F_GETFL);

  if (flags & O_APPEND) {
    fprintf(stdout, "file status flag is O_APPEND (%d).\n", O_APPEND);
  }

  if (flags & O_NONBLOCK) {
    fprintf(stdout, "file status flag is O_NONBLOCK (%d).\n", O_NONBLOCK);
  }

  fprintf(stdout, "Add O_NDELAY (%d) flag.\n", O_NDELAY);

  flags |= O_NDELAY;

  fcntl(fd, F_SETFL, flags);

  if (flags & O_NDELAY) {
    fprintf(stdout, "file status flag is O_NDELAY (%d).\n", O_NDELAY);
  }

  fprintf(stdout, "Remove O_NONBLOCK (%d) flag.\n", O_NONBLOCK);

  flags &= ~O_NONBLOCK;

  fcntl(fd, F_SETFL, flags);

  if (flags & O_NONBLOCK) {
    fprintf(stdout, "file status flag is O_NONBLOCK (%d).\n", O_NONBLOCK);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
