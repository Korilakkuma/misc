#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./calculate_pi_by_monte_carlo [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT m = 0;

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    double x = (double)rand() / (RAND_MAX + 1.0);
    double y = (double)rand() / (RAND_MAX + 1.0);
    double r = (x * x) + (y * y);

    if (r <= 1.0) {
      m++;
    }
  }

  double pi = 4.0 * ((double)m / (double)n);

  printf("PI = %f\n", pi);

  return 0;
}
