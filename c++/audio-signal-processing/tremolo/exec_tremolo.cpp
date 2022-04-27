#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../wave/wave.h"
#include "tremolo.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Require depth, rate\n" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double depth = std::stod(argv[1]);
  double rate  = std::stod(argv[2]);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  Tremolo(depth, rate, pcm0.sL, pcm1.sL, pcm1.fs, pcm1.length);
  Tremolo(depth, rate, pcm0.sR, pcm1.sR, pcm1.fs, pcm1.length);

  WAVE::wave_write(&pcm1, "tremolo.wav");
}
