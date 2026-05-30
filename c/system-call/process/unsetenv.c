#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./unsetenv env_name\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *name = argv[1];

  fprintf(stdout, "%s=%s\n", name, getenv(name));

  if (unsetenv(name) == -1) {
    perror("unsetenv");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "%s=%s\n", name, getenv(name));

  return 0;
}
