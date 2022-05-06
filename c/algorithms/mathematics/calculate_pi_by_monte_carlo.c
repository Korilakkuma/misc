#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./calculate_pi_by_monte_carlo [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);

  long long m = 0;

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    double x = (double)rand() / (double)RAND_MAX;
    double y = (double)rand() / (double)RAND_MAX;
    double r = (x * x) + (y * y);

    if (r <= 1.0) {
      m++;
    }
  }

  double pi = 4.0 * ((double)m / (double)n);

  printf("PI = %f\n", pi);

  return 0;
}
