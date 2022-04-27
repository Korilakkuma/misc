#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "../wave/wave.h"
#include "../fir-filter/fir_filters.h"
#include "frame_filters.h"

enum {
  L = 256,
  N = 512
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr <<"Require fe, delta" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  std::vector<double> b;
  std::vector<double> w;
  double fe;
  double delta;
  int J;

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  fe    = std::stod(argv[1]) / pcm0.fs;
  delta = std::stod(argv[2]) / pcm0.fs;
  J     = std::round(3.1 / delta);

  if ((J % 2) == 1) {
    J++;
  }

  b.resize(J + 1);
  w.resize(J + 1);

  hanning_window(w, (J + 1));

  FIR_LPF(fe, J, b, w);

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  fir_frame_filter(pcm0.sL, pcm1.sL, b, pcm0.length, J, L, N);
  fir_frame_filter(pcm0.sR, pcm1.sR, b, pcm0.length, J, L, N);

  WAVE::wave_write(&pcm1, "weekend.wav");
}
