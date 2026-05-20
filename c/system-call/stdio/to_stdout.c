#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./to_stdout filename", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char *filename = argv[1];

  if ((fp = fopen(filename, "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  int ch;

  while ((ch = fgetc(fp)) != EOF) {
    if (putchar(ch) == EOF) {
      perror("putchar");
      exit(EXIT_FAILURE);
    }
  }

  if (fclose(fp) == EOF) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
