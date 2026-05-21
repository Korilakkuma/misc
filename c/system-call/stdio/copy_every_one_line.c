#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

  char buf[BUFSIZ];

  while ((fgets(buf, BUFSIZ, fp_src)) != NULL) {
    if (fputs(buf, fp_dest) == EOF) {
      perror("fputs");
      exit(EXIT_FAILURE);
    }
  }

  if (feof(fp_src)) {
    clearerr(fp_src);
  } else if (ferror(fp_src)) {
    perror("fgets");
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
