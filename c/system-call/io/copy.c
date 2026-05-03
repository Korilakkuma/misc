#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./copy src_file dest_file\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *src  = argv[1];
  char *dest = argv[2];

  int r_fd;
  int w_fd;

  size_t size = 8;
  ssize_t read_byte;
  char buf[size + 1];

  if ((r_fd = open(src, O_RDWR)) == -1) {
    perror("open");
    goto close;
  }

  if ((w_fd = open(dest, (O_WRONLY | O_CREAT | O_TRUNC), 0666)) == -1) {
    perror("open");
    goto close;
  }

  while ((read_byte = read(r_fd, buf, size)) > 0) {
    if (write(w_fd, buf, (size_t)read_byte) != read_byte) {
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
  if (close(r_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (close(w_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
