#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./append filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd;

  size_t size = 64;

  char buf[size + 1];

  ssize_t read_byte;

  off_t position;

  if ((fd = open(filename, (O_WRONLY | O_CREAT | O_TRUNC), 0666)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  position = lseek(fd, 0, SEEK_END);

  fprintf(stdout, "seek to %lld.\n", position);

  write(STDOUT_FILENO, "Type > ", 7);

  while ((read_byte = read(STDIN_FILENO, buf, size)) > 0) {
    if (write(fd, buf, read_byte) != read_byte) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    buf[size] = '\0';

    fprintf(stdout, "written is '%s' (%zu bytes)", buf, read_byte);
  }

  if (read_byte == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  position = lseek(fd, 0, SEEK_CUR);

  fprintf(stdout, "position is %lld.\n", position);

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
