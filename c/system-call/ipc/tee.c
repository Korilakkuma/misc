#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./stream 'tee file0 > file1' 'message'\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char *command = argv[1];
  char *message = argv[2];

  if ((fp = popen(command, "w")) == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }

  fprintf(fp, "%s\n", message);

  if (pclose(fp) == -1) {
    perror("pclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
