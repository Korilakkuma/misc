#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../wave/wave.h"

enum {
  N = 15
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Require amplitude and f0" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  MONO_PCM pcm;

  double a  = std::stod(argv[1]);
  double f0 = std::stod(argv[2]);

  pcm.fs = 8000;
  pcm.bits = 16;
  pcm.length = 8000;

  pcm.s.reserve(pcm.length);

  for (int n = 0; n < pcm.length; n++) {
    for (int i = 0; i < N; i++) {
      if ((i % 2) != 0) {
        pcm.s[n] += (a / std::pow(i, 2)) * std::sin(2.0 * M_PI * (i * f0) * n / pcm.fs);
      }
    }
  }

  WAVE::wave_write(&pcm, "triangle.wav");
}
