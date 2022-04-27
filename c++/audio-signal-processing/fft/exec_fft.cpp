#include <iostream>
#include <vector>
#include <complex>
#include "../wave/wave.h"
#include "fft.h"

enum {
  FFT_SIZE = 8
};

int main(void) {
  MONO_PCM pcm;
  std::vector<std::complex<double>> x(FFT_SIZE);

  WAVE::wave_read(&pcm, "sample.wav");

  for (int n = 0; n < FFT_SIZE; n++) {
    x[n] = std::complex<double>(pcm.s[n], 0.0);
  }

  FFT(x, FFT_SIZE);

  for (int k = 0; k < FFT_SIZE; k++) {
    std::cout << "[" << k << "] " << x[k].real() << " + j" << x[k].imag() << std::endl;
  }

  return 0;
}
