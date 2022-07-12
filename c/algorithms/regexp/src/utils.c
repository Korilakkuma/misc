#include "includes.h"

void *alloc_memory(const size_t size) {
  void *p;

  if ((p = malloc(size)) == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  memset(p, 0, (unsigned long)size);

  return p;
}

void syntax_error(const char *s) {
  fprintf(stderr, "Syntax error: %s\n", s);
  exit(EXIT_FAILURE);
}
