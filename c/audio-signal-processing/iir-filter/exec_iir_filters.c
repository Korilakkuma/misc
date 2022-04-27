#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../wave/wave.h"
#include "iir_filters.h"

enum {
  I = 2,
  J = 2
};

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Require type, fc\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  double a[3];
  double b[3];

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm0.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm0.length, sizeof(double));

  double fc = strtod(argv[2], NULL) / pcm0.fs;
  double Q  = 1.0 / sqrt(2.0);

  if (strncasecmp("lpf", argv[1], 3) == 0) {
    IIR_LPF(fc, Q, a, b);
  } else if (strncasecmp("hpf", argv[1], 3) == 0) {
    IIR_LPF(fc, Q, a, b);
  } else if (strncasecmp("bpf", argv[1], 3) == 0) {
    IIR_BPF(fc, Q, a, b);
  } else if (strncasecmp("bef", argv[1], 3) == 0) {
    IIR_BEF(fc, Q, a, b);
  }

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

  stereo_wave_write(&pcm1, "weekend.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}
