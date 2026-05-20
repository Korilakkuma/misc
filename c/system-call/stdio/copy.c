#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./copy src dest", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp_src;
  FILE *fp_dest;

  char *filename_src  = argv[1];
  char *filename_dest = argv[2];

  if ((fp_src = fopen(filename_src, "r")) == NULL) {
    fprintf(stderr, "fopen: %s: %s\n", filename_src, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((fp_dest = fopen(filename_dest, "w")) == NULL) {
    fprintf(stderr, "fopen: %s: %s\n", filename_dest, strerror(errno));
    exit(EXIT_FAILURE);
  }

  int ch;

  while ((ch = getc(fp_src)) != EOF) {
    if (putc(ch, fp_dest) == EOF) {
      perror("putc");
      exit(EXIT_FAILURE);
    }
  }

  if (ferror(fp_src)) {
    perror("getc");
  }

  if (fclose(fp_src) == -1) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  if (fclose(fp_dest) == -1) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
