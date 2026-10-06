#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./cp src dest\n", stderr);
    exit(EXIT_FAILURE);
  }

  int src_fd;
  int dest_fd;

  char *src_filename  = argv[1];
  char *dest_filename = argv[2];

  if ((src_fd = open(src_filename, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if ((dest_fd = creat(dest_filename, 0644)) == -1) {
    perror("creat");
    exit(EXIT_FAILURE);
  }

  ssize_t read_byte;
  char buf[1024];

  while ((read_byte = read(src_fd, buf, sizeof(buf))) > 0) {
    if (write(dest_fd, buf, (size_t)read_byte) != read_byte) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  if (read_byte == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  if (close(src_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (close(dest_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
