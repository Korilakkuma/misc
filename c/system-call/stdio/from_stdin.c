#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./from_stdin filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char *filename = argv[1];

  if ((fp = fopen(filename, "w")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  int ch;

  while ((ch = getchar()) != EOF) {
    if (fputc(ch, fp) == EOF) {
      perror("fputc");
      exit(EXIT_FAILURE);
    }
  }

  if (ferror(stdin)) {
    perror("getchar");
  }

  if (fclose(fp) == -1) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  return 0;
}
