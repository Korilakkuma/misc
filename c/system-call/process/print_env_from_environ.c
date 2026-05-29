#include <stdio.h>

extern char **environ;

int main(void) {
  char **p = environ;

  while (*p != NULL) {
    fprintf(stdout, "%s\n", *p++);
  }

  return 0;
}
