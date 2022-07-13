#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  double n = strtod(argv[1], NULL);

  printf("%lld\n", (long long)(1 / n));

  return 0;
}
