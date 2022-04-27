#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"
#include "../fir-filter/fir_filters.h"
#include "frame_filters.h"

enum {
  L = 256,
  N = 512
};

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Require fe, delta\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  double *b;
  double *w;
  double fe;
  double delta;
  int J;

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm1.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm1.length, sizeof(double));

  fe    = strtod(argv[1], NULL) / pcm0.fs;
  delta = strtod(argv[2], NULL) / pcm0.fs;
  J     = (int)((3.1 / delta) + 0.5) - 1;

  if ((J % 2) == 1) {
    J++;
  }

  b = (double *)calloc((J + 1), sizeof(double));
  w = (double *)calloc((J + 1), sizeof(double));

  hanning_window(w, (J + 1));

  FIR_LPF(fe, J, b, w);

  fir_frame_filter(pcm0.sL, pcm1.sL, b, pcm0.length, J, L, N);
  fir_frame_filter(pcm0.sR, pcm1.sR, b, pcm0.length, J, L, N);

  stereo_wave_write(&pcm1, "weekend.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);
  free(b);
  free(w);

  return 0;
}
