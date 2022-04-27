#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "fir_filters.h"

int main(int argc, char **argv) {
  if ((argc != 4) && (argc != 5)) {
    std::cerr << "Require type, fe, delta" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string type = argv[1];

  STEREO_PCM pcm0, pcm1;
  std::vector<double> b;
  std::vector<double> w;
  int J;

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  pcm1.sL.reserve(pcm0.length);
  pcm1.sR.reserve(pcm0.length);

  if (argc == 4) {
    double fe    = std::stod(argv[2]) / pcm0.fs;
    double delta = std::stod(argv[3]) / pcm0.fs;

    J = (int)((3.1 / delta) + 0.5) - 1;

    if ((J % 2) == 1) {
      J++;
    }

    b.resize(J);
    w.resize(J);

    hanning_window(w, (J + 1));

    if (type == "lpf") {
      FIR_LPF(fe, J, b, w);
    } else if (type == "hpf") {
      FIR_HPF(fe, J, b, w);
    }
  } else if (argc == 5) {
    double fe1   = std::stod(argv[2]) / pcm0.fs;
    double fe2   = std::stod(argv[3]) / pcm0.fs;
    double delta = std::stod(argv[4]) / pcm0.fs;

    J = (int)((3.1 / delta) + 0.5) - 1;

    if ((J % 2) == 1) {
      J++;
    }

    b.resize(J);
    w.resize(J);

    hanning_window(w, (J + 1));

    if (type == "bpf") {
      FIR_BPF(fe1, fe2, J, b, w);
    } else if (type == "bef") {
      FIR_BEF(fe1, fe2, J, b, w);
    }
  }

  for (int n = 0; n < pcm1.length; n++) {
    for (int m = 0; m <= J; m++) {
      if ((n - m) >= 0) {
        pcm1.sL[n] += b[m] * pcm0.sL[n - m];
        pcm1.sR[n] += b[m] * pcm0.sR[n - m];
      }
    }
  }

  WAVE::wave_write(&pcm1, "weekend.wav");
}
