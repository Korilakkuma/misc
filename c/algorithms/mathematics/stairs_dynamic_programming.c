#include <stdio.h>
#include <stdlib.h>

#define INT long long

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./stairs_dynamic_programming [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *m = (INT *)malloc(n * sizeof(INT));

  m[0] = 1;
  m[1] = 1;
  m[2] = 2;

  for (INT i = 3; i <= n; i++) {
    m[i] = m[i - 1] + m[i - 2];
  }

  printf("Answer = %lld\n", m[n]);

  free(m);

  return 0;
}
