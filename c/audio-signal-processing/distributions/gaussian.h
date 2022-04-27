#include <math.h>

void Gaussian(double *dist, double mean, double variance, int N) {
  for (int n = 0; n < N; n++) {
    dist[n] = (1 / sqrt(2 * M_PI * variance)) * exp(-1 * (pow((n - mean), 2) / (2 * variance)));
  }
}
