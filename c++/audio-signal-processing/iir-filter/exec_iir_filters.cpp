#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include "../wave/wave.h"
#include "iir_filters.h"

enum {
  I = 2,
  J = 2
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Require type, fc" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string type = argv[1];

  STEREO_PCM pcm0, pcm1;
  std::vector<double> a(3);
  std::vector<double> b(3);

  WAVE::wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;

  double fc = std::stod(argv[2]) / pcm0.fs;
  double Q  = 1.0 / std::sqrt(2.0);

  if (type == "lpf") {
    IIR_LPF(fc, Q, a, b);
  } else if (type == "hpf") {
    IIR_LPF(fc, Q, a, b);
  } else if (type == "bpf") {
    IIR_BPF(fc, Q, a, b);
  } else if (type == "bef") {
    IIR_BEF(fc, Q, a, b);
  }

  pcm1.sL.resize(pcm1.length);
  pcm1.sR.resize(pcm1.length);

  for (int n = 0; n < pcm1.length; n++) {
    for (int m = 0; m <= J; m++) {
      if ((n - m) >= 0) {
        pcm1.sL[n] += b[m] * pcm0.sL[n - m];
        pcm1.sR[n] += b[m] * pcm0.sR[n - m];
      }
    }

    for (int m = 1; m <= I; m++) {
      if ((n - m) >= 0) {
        pcm1.sL[n] += -a[m] * pcm1.sL[n - m];
        pcm1.sR[n] += -a[m] * pcm1.sR[n - m];
      }
    }
  }

  WAVE::wave_write(&pcm1, "weekend.wav");
}
