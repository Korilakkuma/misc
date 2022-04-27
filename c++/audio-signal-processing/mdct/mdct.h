#include <vector>
#include <cmath>

void MDCT(std::vector<double> &x, std::vector<double> &X, int N) {
  for (int k = 0; k < N; k++) {
    double s = 0.0;

    for (int n = 0; n <= ((2 * N) - 1); n++) {
      s += x[n] * std::cos((M_PI / N) * (n + (N / 2.0) + (1.0 / 2.0)) * (k + (1.0 / 2.0)));
    }

    X[k] = s;
  }
}

void IMDCT(std::vector<double> &x, std::vector<double> &X, int N) {
  for (int n = 0; n <= ((2 * N) - 1); n++) {
    double t = 0.0;

    for (int k = 0; k < N; k++) {
      t += X[k] * std::cos((M_PI / N) * (n + (N / 2.0) + (1.0 / 2.0)) * (k + (1.0 / 2.0)));
    }

    x[n] = t / N;
  }
}
