#include <iostream>
#include <vector>
#include <cmath>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "mdct.h"

enum {
  N = 8
};

int main() {
  MONO_PCM pcm;

  std::vector<double> x(2 * N);
  std::vector<double> X(N, 0.0);
  std::vector<double> w(2 * N);

  WAVE::wave_read(&pcm, "sample.wav");

  std::cout << std::fixed;

  std::cout << "x(n)" << std::endl;

  sine_window(w, (2 * N));

  for (int n = 0; n < (2 * N); n++) {
    x[n] = w[n] * pcm.s[n];
    std::cout << "[" << n << "]" << " " << x[n] << std::endl;
  }

  MDCT(x, X, N);

  std::cout << "X(k)" << std::endl;

  for (int k = 0; k < N; k++) {
    std::cout << "[" << k << "]" << " " << X[k] << std::endl;
  }

  x.assign((2 * N), 0.0);

  IMDCT(x, X, N);

  std::cout << "x(n)" << std::endl;

  for (int n = 0; n < (2 * N); n++) {
    x[n] *= w[n];
    std::cout << "[" << n << "]" << " " << x[n] << std::endl;
  }
}
