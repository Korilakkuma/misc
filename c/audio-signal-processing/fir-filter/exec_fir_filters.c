#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "fir_filters.h"

int main(int argc, char **argv) {
  if ((argc != 4) && (argc != 5)) {
    fputs("Require type, fe, delta\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  double *b;
  double *w;
  int J;

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm0.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm0.length, sizeof(double));

  if (argc == 4) {
    double fe    = strtod(argv[2], NULL) / pcm0.fs;
    double delta = strtod(argv[3], NULL) / pcm0.fs;

    J = (int)((3.1 / delta) + 0.5) - 1;

    if ((J % 2) == 1) {
      J++;
    }

    b = (double *)calloc((J + 1), sizeof(double));
    w = (double *)calloc((J + 1), sizeof(double));

    hanning_window(w, (J + 1));

    if (strncasecmp("lpf", argv[1], 3) == 0) {
      FIR_LPF(fe, J, b, w);
    } else if (strncasecmp("hpf", argv[1], 3) == 0) {
      FIR_HPF(fe, J, b, w);
    }
  } else if (argc == 5) {
    double fe1   = strtod(argv[2], NULL) / pcm0.fs;
    double fe2   = strtod(argv[3], NULL) / pcm0.fs;
    double delta = strtod(argv[4], NULL) / pcm0.fs;

    J = (int)((3.1 / delta) + 0.5) - 1;

    if ((J % 2) == 1) {
      J++;
    }

    b = (double *)calloc((J + 1), sizeof(double));
    w = (double *)calloc((J + 1), sizeof(double));

    hanning_window(w, (J + 1));

    if (strncasecmp("bpf", argv[1], 3) == 0) {
      FIR_BPF(fe1, fe2, J, b, w);
    } else if (strncasecmp("bef", argv[1], 3) == 0) {
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

  stereo_wave_write(&pcm1, "weekend.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);
  free(b);
  free(w);

  return 0;
}
