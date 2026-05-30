#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./setenv env_name env_value\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *name = argv[1];
  char *value = argv[2];

  fprintf(stdout, "%s=%s\n", name, getenv(name));

  if (setenv(name, value, 1) == -1) {
    perror("setenv");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "%s=%s\n", name, getenv(name));

  return 0;
}
