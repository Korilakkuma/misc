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
    for (int i = 1; i <= N; i++) {
      pcm.s[n] += a * sin(2.0 * M_PI * (i * f0) * n / pcm.fs);
    }
  }

  WAVE::wave_write(&pcm, "sawtooth.wav");
}
