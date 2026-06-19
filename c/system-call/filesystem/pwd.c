#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char dirname[PATH_MAX + 1];

  if (getcwd(dirname, sizeof(dirname)) == NULL) {
    perror("getcwd");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "%s", dirname);

  return 0;
}
