#include <iostream>
#include <cstdlib>
#include <vector>
#include "gaussian.h"

enum {
  N = 10
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Require mean, variance" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::vector<double> dist(N);

  double mu     = std::stod(argv[1]);
  double delta2 = std::stod(argv[2]);

  Gaussian(dist, mu, delta2, N);

  for (int n = 0; n < N; n++) {
    std::cout << dist[n] << std::endl;
  }
}
