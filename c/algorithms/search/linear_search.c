#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long
#define N 10
#define RANGE 10

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./linear_search [0-9]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT key = strtoll(argv[1], NULL, 10);

  INT a[N];

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    a[i] = RANGE * ((double)rand() / (RAND_MAX + 1.0));
  }

  INT i = 0;

  while ((i < N) && (a[i] != key)) {
    i++;
  }

  if (i < N) {
    printf("%lld is found (a[%lld])\n\n", a[i], i);
  } else {
    printf("%lld is not found\n\n", key);
  }

  printf("dump\n");

  for (INT i = 0; i < N; i++) {
    printf("a[%lld] = %lld\n", i, a[i]);
  }

  printf("\n");

  return 0;
}
