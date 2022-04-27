#include <iostream>
#include <cstdlib>
#include <string>
#include "../wave/wave.h"
#include "compressor.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Require input filename, output filename, threshold, ratio" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0;

  double threshold = std::stod(argv[3]);
  double ratio     = std::stod(argv[4]);

  std::string infile  = argv[1];
  std::string outfile = argv[2];

  WAVE::wave_read(&pcm0, infile);

  Compressor(pcm0.sL, pcm0.sR, pcm0.length, threshold, ratio);

  WAVE::wave_write(&pcm0, outfile);
}
