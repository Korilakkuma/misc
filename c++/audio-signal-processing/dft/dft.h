#include <vector>
#include <cmath>
#include <complex>

void DFT(std::vector<std::complex<double>> &x, std::vector<std::complex<double>> &X, int N) {
  for (int k = 0; k < N; k++) {
    double real_sum = 0.0;
    double imag_sum = 0.0;

    for (int n = 0; n < N; n++) {
      double real =  std::cos((2 * M_PI * k * n) / N);
      double imag = -std::sin((2 * M_PI * k * n) / N);

      real_sum += (real * x[n].real()) - (imag * x[n].imag());
      imag_sum += (real * x[n].imag()) + (imag * x[n].real());
    }

    X[k] = std::complex<double>(real_sum, imag_sum);
  }
}

void IDFT(std::vector<std::complex<double>> &x, std::vector<std::complex<double>> &X, int N) {
  for (int n = 0; n < N; n++) {
    double real_sum = 0.0;
    double imag_sum = 0.0;

    for (int k = 0; k < N; k++) {
      double real = std::cos((2 * M_PI * k * n) / N);
      double imag = std::sin((2 * M_PI * k * n) / N);

      real_sum += ((real * X[k].real()) - (imag * X[k].imag()));
      imag_sum += ((real * X[k].imag()) + (imag * X[k].real()));
    }

    x[n] = std::complex<double>((real_sum / N), (imag_sum / N));
  }
}
