#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./stairs_dynamic_programming [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  long long *m = (long long *)malloc(n);

  m[0] = 1;
  m[1] = 1;
  m[2] = 2;

  for (long long i = 3; i <= n; i++) {
    m[i] = m[i - 1] + m[i - 2];
  }

  printf("Answer = %lld\n", m[n]);

  free(m);

  return 0;
}
