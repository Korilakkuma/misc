#include <iostream>
#include <cstdlib>
#include <string>
#include "../wave/wave.h"
#include "distortion.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Require input filename, output filename, gain, level\n" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double gain  = std::stod(argv[3]);
  double level = std::stod(argv[4]);

  std::string infile  = argv[1];
  std::string outfile = argv[2];

  WAVE::wave_read(&pcm0, infile);

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  Distortion(pcm0.sL, pcm0.sR, pcm0.length, gain, level);

  WAVE::wave_write(&pcm0, outfile);
}
