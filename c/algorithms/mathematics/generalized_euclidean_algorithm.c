#include <stdio.h>
#include <stdlib.h>

#define INT long long

static INT euclidean_algorithm(INT n1, INT n2);

int main(int argc, char **argv) {
  if (argc < 3) {
    fputs("./euclidean_algorithm [natural number] [natural number] ...", stderr);
    exit(EXIT_FAILURE);
  }

  INT r = strtoll(argv[1], NULL, 10);

  for (INT i = 0, end = argc - 2; i < end; i++) {
    INT n = strtoll(argv[i + 2], NULL, 10);

    r = euclidean_algorithm(r, n);
  }

  printf("greatest common divisor is %lld.\n", r);

  return 0;
}

static INT euclidean_algorithm(INT n1, INT n2) {
  INT n = (n1 >= n2) ? n1 : n2;  // numerator
  INT d = (n2 < n1) ? n2 : n1;  // denominator
  INT e;  // extra

  do {
    e = n % d;
    n = (d >= e) ? d : e;
    d = (e < d) ? e : d;
  } while (e != 0);

  return n;
}
