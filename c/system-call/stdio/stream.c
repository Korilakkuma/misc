#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./stream filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char *filename = argv[1];

  if ((fp = fopen(filename, "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "`File` pointer is %p.\n", fp);
  fprintf(stdout, "`_file` field (file descriptor) is %d.\n", fp->_file);

  if (fclose(fp) == -1) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
