#include <iostream>
#include <cstdlib>
#include "../wave/wave.h"
#include "resampling.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Require pitch" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double pitch = std::stod(argv[1]);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = static_cast<int>(pcm0.length / pitch);

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  Resampling(pitch, pcm0.sL, pcm1.sL, pcm0.length);
  Resampling(pitch, pcm0.sR, pcm1.sR, pcm0.length);

  WAVE::wave_write(&pcm1, "resampling.wav");
}
