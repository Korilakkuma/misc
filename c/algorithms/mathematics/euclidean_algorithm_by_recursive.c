#include <stdio.h>
#include <stdlib.h>

long long euclidean_algorithm(long long n1, long long n2);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./euclidean_algorithm_recursive [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n1 = strtol(argv[1], NULL, 10);
  long long n2 = strtol(argv[2], NULL, 10);

  long long r = euclidean_algorithm(n1, n2);

  printf("greatest common divisor (%lld, %lld) is %lld.\n", n1, n2, r);

  return 0;
}

long long euclidean_algorithm(long long n1, long long n2) {
  if (n2 == 0) {
    return n1;
  }

  return euclidean_algorithm(n2, (n1 % n2));
}
