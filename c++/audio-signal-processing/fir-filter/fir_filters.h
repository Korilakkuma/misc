#include <vector>
#include <cmath>

double sinc(double n) {
  if (n == 0) {
    return 1.0;
  }

  return std::sin(n) / n;
}

void FIR_LPF(double fe, int J, std::vector<double> &b, std::vector<double> &w) {
  int offset = J / 2;

  for (int m = -(J / 2); m <= (J / 2); m++) {
    b[offset + m] = 2.0 * fe * sinc(2.0 * M_PI * fe * m);
  }

  for (int m = 0; m < (J + 1); m++) {
    b[m] *= w[m];
  }
}

void FIR_HPF(double fe, int J, std::vector<double> &b, std::vector<double> &w) {
  int offset = J / 2;

  for (int m = -(J / 2); m <= (J / 2); m++) {
    b[offset + m] = sinc(M_PI * m) - (2.0 * fe * sinc(2.0 * M_PI * fe * m));
  }

  for (int m = 0; m < (J + 1); m++) {
    b[m] *= w[m];
  }
}

void FIR_BPF(double fe1, double fe2, int J, std::vector<double> &b, std::vector<double> &w) {
  int offset = J / 2;

  for (int m = -(J / 2); m <= (J / 2); m++) {
    b[offset + m] = (2 * fe2 * sinc(2 * M_PI * fe2 * m)) - (2.0 * fe1 * sinc(2.0 * M_PI * fe1 * m));
  }

  for (int m = 0; m < (J + 1); m++) {
    b[m] *= w[m];
  }
}

void FIR_BEF(double fe1, double fe2, int J, std::vector<double> &b, std::vector<double> &w) {
  int offset = J / 2;

  for (int m = -(J / 2); m <= (J / 2); m++) {
    b[offset + m] = sinc(M_PI * m) - (2 * fe2 * sinc(2 * M_PI * fe2 * m)) + (2.0 * fe1 * sinc(2.0 * M_PI * fe1 * m));
  }

  for (int m = 0; m < (J + 1); m++) {
    b[m] *= w[m];
  }
}
