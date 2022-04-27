#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "../wave/wave.h"
#include "../iir-filter/iir_filters.h"
#include "frame_filters.h"

enum {
  L = 256,
  J = 2,
  I = 2
};

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Require fc" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  std::vector<double> a(3);
  std::vector<double> b(3);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  double fc = std::stod(argv[1]) / pcm0.fs;
  double Q  = 1.0 / std::sqrt(2.0);

  IIR_LPF(fc, Q, a, b);

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  iir_frame_filter(pcm0.sL, pcm1.sL, a, b, pcm0.length, J, I, L);
  iir_frame_filter(pcm0.sR, pcm1.sR, a, b, pcm0.length, J, I, L);

  WAVE::wave_write(&pcm1, "weekend.wav");
}
