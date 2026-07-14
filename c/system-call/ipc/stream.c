#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./stream 'command'\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char buf[1024];

  char *command = argv[1];

  if ((fp = popen(command, "r")) == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buf, sizeof(buf), fp) != NULL) {
    fprintf(stdout, "%s", buf);
  }

  if (pclose(fp) == -1) {
    perror("pclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
