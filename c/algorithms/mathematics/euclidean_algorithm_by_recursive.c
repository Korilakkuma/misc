#include <stdio.h>
#include <stdlib.h>

#define INT long long

INT euclidean_algorithm(INT n1, INT n2);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./euclidean_algorithm_recursive [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n1 = strtoll(argv[1], NULL, 10);
  INT n2 = strtoll(argv[2], NULL, 10);

  INT r = euclidean_algorithm(n1, n2);

  printf("greatest common divisor (%lld, %lld) is %lld.\n", n1, n2, r);

  return 0;
}

INT euclidean_algorithm(INT n1, INT n2) {
  if (n2 == 0) {
    return n1;
  }

  return euclidean_algorithm(n2, (n1 % n2));
}
