#include <cmath>
#include <complex>
#include "../window/window_functions.h"
#include "../fft/fft.h"

enum {
  N = 1024
};

void NoiseSuppressor(
  double threshold,
  std::vector<double> &in,
  std::vector<double> &out,
  int fs,
  int length
) {
  int number_of_frame = (length - (N / 2)) / (N / 2);

  std::vector<std::complex<double>> x(N);
  std::vector<std::complex<double>> y(N);

  std::vector<double> w(N);
  std::vector<double> x_real(N);
  std::vector<double> x_imag(N);
  std::vector<double> A(N);
  std::vector<double> T(N);
  std::vector<double> y_real(N);
  std::vector<double> y_imag(N);

  hanning_window(w, N);

  for (int frame = 0; frame < number_of_frame; frame++) {
    int offset = (N / 2) * frame;

    for (int n = 0; n < N; n++) {
      x_real[n] = in[offset + n] * w[n];
      x_imag[n] = 0.0;

      x[n] = std::complex<double>(x_real[n], x_imag[n]);
    }

    FFT(x, N);

    for (int k = 0; k < N; k++) {
      A[k] = std::sqrt((x[k].real() * x[k].real()) + (x[k].imag() * x[k].imag()));

      if ((x[k].imag() != 0.0) && (x[k].real() != 0.0)) {
        T[k] = std::atan2(x[k].imag(), x[k].real());
      }
    }

    for (int k = 0; k < N; k++) {
      A[k] -= threshold;

      if (A[k] < 0.0) {
        A[k] = 0.0;
      }
    }

    for (int k = 0; k < N; k++) {
      y_real[k] = A[k] * cos(T[k]);
      y_imag[k] = A[k] * sin(T[k]);

      y[k] = std::complex<double>(y_real[k], y_imag[k]);
    }

    IFFT(y, N);

    for (int n = 0; n < N; n++) {
      out[offset + n] += y[n].real();
    }
  }
}
