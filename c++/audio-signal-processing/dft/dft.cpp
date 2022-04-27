#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "dft.h"

enum {
  N = 8
};

int main(int argc, char **argv) {
  MONO_PCM pcm;

  std::vector<std::complex<double>> x(N);
  std::vector<std::complex<double>> X(N, 0.0);
  std::vector<double> w(N);

  WAVE::wave_read(&pcm, "sample.wav");

  hanning_window(w, N);

  for (int n = 0; n < N; n++) {
    double real;

    if (argc > 1) {
      real = w[n] * pcm.s[n];
    } else {
      real = pcm.s[n];
    }

    x[n] = std::complex<double>(real, 0.0);
  }

  std::cout << std::fixed;

  std::cout << "x(n)" << std::endl;

  for (int n = 0; n < N; n++) {
    std::cout << "[" << n << "]" << " " << x[n].real() << std::endl;
  }

  DFT(x, X, N);

  std::cout << "X(k)" << std::endl;

  for (int k = 0; k < N; k++) {
    std::cout << "[" << k << "]" << " " <<  X[k].real() << " + j" << X[k].imag() << std::endl;
  }

  x.assign(N, 0.0);

  IDFT(x, X, N);

  std::cout << "x(n)" << std::endl;

  for (int n = 0; n < N; n++) {
    std::cout << "[" << n << "]" << " " << x[n].real() << std::endl;
  }
}
