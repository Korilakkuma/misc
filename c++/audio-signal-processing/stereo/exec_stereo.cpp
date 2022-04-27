#include <iostream>
#include <cstdlib>
#include "../wave/wave.h"
#include "stereo.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Require delay" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double delay = std::stod(argv[1]);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  Stereo(
    delay,
    pcm0.sL,
    pcm0.sR,
    pcm1.sL,
    pcm1.sR,
    pcm1.fs,
    pcm1.length
  );

  WAVE::wave_write(&pcm1, "pesudo-stereo.wav");
}
