#include <iostream>
#include <cstdlib>
#include <string>
#include "../wave/wave.h"
#include "delay.h"

int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr << "Reqiure input filename, output filename, wet, time, feedback" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double wet   = std::stod(argv[3]);
  double time  = std::stod(argv[4]);
  int feedback = std::stol(argv[5]);

  std::string infile  = argv[1];
  std::string outfile = argv[2];

  WAVE::wave_read(&pcm0, infile);

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  pcm1.sL.reserve(pcm0.length);
  pcm1.sR.reserve(pcm0.length);

  Delay(pcm0.sL, pcm1.sL, pcm0.sR, pcm1.sR, pcm1.length, wet, (pcm1.fs * time), feedback);

  WAVE::wave_write(&pcm1, outfile);
}
