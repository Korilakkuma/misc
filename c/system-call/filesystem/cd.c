#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./cd pathname\n", stderr);
    exit(EXIT_FAILURE);
  }

  const char *pathname = argv[1];

  char dirname[PATH_MAX + 1];

  if (chdir(pathname) == -1) {
    perror("chdir");
    exit(EXIT_FAILURE);
  }

  if (getcwd(dirname, sizeof(dirname)) == NULL) {
    perror("getcwd");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "cd to %s\n", dirname);

  return 0;
}
