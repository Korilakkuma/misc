#include <vector>
#include <cmath>

void Gaussian(std::vector<double> &dist, double mean, double variance, int N) {
  for (int n = 0; n < N; n++) {
    dist[n] = (1 / std::sqrt(2 * M_PI * variance)) * std::exp(-1 * (std::pow((n - mean), 2) / (2 * variance)));
  }
}
