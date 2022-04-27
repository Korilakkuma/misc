#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../wave/wave.h"
#include "equalizer.h"

enum {
  I = 2,
  J = 2
};

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Require bass, middle, treble" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double bass   = std::stod(argv[1]);
  double middle = std::stod(argv[2]);
  double treble = std::stod(argv[3]);
  double Q      = 1.0 / std::sqrt(2.0);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  Equalizer(pcm0.sL, pcm1.sL, pcm1.fs, pcm1.length, I, J, bass, middle, treble, Q);
  Equalizer(pcm0.sR, pcm1.sR, pcm1.fs, pcm1.length, I, J, bass, middle, treble, Q);

  WAVE::wave_write(&pcm1, "equalizer.wav");
}
