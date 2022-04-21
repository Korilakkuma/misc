#include <stdio.h>
#include <stdlib.h>

long long euclidean_algorithm(long long n1, long long n2);

int main(int argc, char **argv) {
  if (argc < 3) {
    fputs("./euclidean_algorithm [natural number] [natural number] ...", stderr);
    exit(EXIT_FAILURE);
  }

  long long r = strtol(argv[1], NULL, 10);

  for (int i = 0, end = argc - 2; i < end; i++) {
    long long n = strtol(argv[i + 2], NULL, 10);

    r = euclidean_algorithm(r, n);
  }

  printf("greatest common divisor is %lld.\n", r);

  return 0;
}

long long euclidean_algorithm(long long n1, long long n2) {
  long long n = (n1 >= n2) ? n1 : n2;  // numerator
  long long d = (n2 < n1) ? n2 : n1;  // denominator
  long long e;  // extra

  do {
    e = n % d;
    n = (d >= e) ? d : e;
    d = (e < d) ? e : d;
  } while (e != 0);

  return n;
}

