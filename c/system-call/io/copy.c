#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./copy src dest\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd_r;
  int fd_w;

  size_t size = 8;
  ssize_t read_byte;
  char buf[size + 1];

  char *filename_src  = argv[1];
  char *filename_dest = argv[2];

  if ((fd_r = open(filename_src, O_RDWR)) == -1) {
    fprintf(stderr, "open: %s: %s\n", filename_src, strerror(errno));
    goto close;
  }

  if ((fd_w = open(filename_dest, (O_WRONLY | O_CREAT | O_TRUNC), 0666)) == -1) {
    fprintf(stderr, "open: %s: %s\n", filename_dest, strerror(errno));
    goto close;
  }

  while ((read_byte = read(fd_r, buf, size)) > 0) {
    if (write(fd_w, buf, (size_t)read_byte) != read_byte) {
      perror("write");
      goto close;
    }

    buf[size] = '\0';

    fprintf(stdout, "written '%s' (%zu bytes)\n", buf, read_byte);
  }

  if (read_byte == -1) {
    perror("read");
    goto close;
  }

close:
  if (close(fd_r) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (close(fd_w) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
