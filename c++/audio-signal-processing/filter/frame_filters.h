#include <vector>
#include <complex>
#include "../window/window_functions.h"
#include "../fft/fft.h"

void fir_frame_filter(
  std::vector<double> &s0,
  std::vector<double> &s1,
  std::vector<double> &b0,
  int length,
  int J,
  int L,
  int N) {
  int number_of_frames = length / L;

  std::vector<std::complex<double>> b(N);
  std::vector<std::complex<double>> x(N);
  std::vector<std::complex<double>> y(N);

  for (int frame = 0; frame < number_of_frames; frame++) {
    int offset = L * frame;

    for (int n = 0; n < N; n++) {
      x[n] = std::complex<double>(0.0, 0.0);
    }

    for (int n = 0; n < L; n++) {
      x[n] = std::complex<double>(s0[offset + n], 0.0);
    }

    FFT(x, N);

    for (int m = 0; m < N; m++) {
      b[m] = std::complex<double>(0.0, 0.0);
    }

    for (int m = 0; m < J; m++) {
      b[m] = std::complex<double>(b0[m], 0.0);
    }

    FFT(b, N);

    for (int k = 0; k < N; k++) {
      y[k] = std::complex<double>(((b[k].real() * x[k].real()) - (b[k].imag() * x[k].imag())), ((b[k].real() * x[k].imag()) + (b[k].imag() * x[k].real())));
    }

    IFFT(y, N);

    for (int n = 0; n < (2 * L); n++) {
      if ((offset + n) < length) {
        s1[offset + n] += y[n].real();
      }
    }
  }
}

void iir_frame_filter(
  std::vector<double> &s0,
  std::vector<double> &s1,
  std::vector<double> &a,
  std::vector<double> &b,
  int length,
  int J,
  int I,
  int L) {
  int number_of_frames = length / L;

  std::vector<double> x(L + J);
  std::vector<double> y(L + I);

  for (int frame = 0; frame < number_of_frames; frame++) {
    int offset = L * frame;

    for (int n = 0; n < L + J; n++) {
      if ((offset - J + n) < 0) {
        x[n] = 0.0;
      } else {
        x[n] = s0[offset - J + n];
      }
    }

    for (int n = 0; n < L + I; n++) {
      if ((offset - I + n) < 0) {
        y[n] = 0.0;
      } else {
        y[n] = s1[offset - I + n];
      }
    }

    for (int n = 0; n < L; n++) {
      for (int m = 0; m <= J; m++) {
        y[I + n] += b[m] * x[J + (n - m)];
      }

      for (int m = 1; m <= I; m++) {
        y[I + n] += -a[m] * y[I + (n - m)];
      }
    }

    for (int n = 0; n < L; n++) {
      s1[offset + n] = y[I + n];
    }
  }
}
