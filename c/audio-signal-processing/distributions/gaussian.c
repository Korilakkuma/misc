#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"

enum {
  N = 10
};

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Require mean, variance\n", stderr);
    exit(EXIT_FAILURE);
  }

  double *dist = (double *)calloc(N, sizeof(double));

  double mu     = strtod(argv[1], NULL);
  double delta2 = strtod(argv[2], NULL);

  Gaussian(dist, mu, delta2, N);

  for (int n = 0; n < N; n++) {
    printf("%f\n", dist[n]);
  }

  free(dist);

  return 0;
}
