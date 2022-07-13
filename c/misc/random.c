#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./random [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  srand((unsigned int)time(NULL));

  printf("RAND_MAX = %d\n", RAND_MAX);

  for (int i = 0; i < 10; i++) {
    printf("rand() = %d\n", (int)(((double)rand() / (RAND_MAX + 1.0)) * strtol(argv[1], NULL, 10)));
  }

  return 0;
}
