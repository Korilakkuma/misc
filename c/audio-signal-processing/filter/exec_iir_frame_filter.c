#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    fputs("Require fc\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;
  double a[3];
  double b[3];

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm1.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm1.length, sizeof(double));

  double fc = strtod(argv[1], NULL) / pcm0.fs;
  double Q  = 1.0 / sqrt(2.0);

  IIR_LPF(fc, Q, a, b);

  iir_frame_filter(pcm0.sL, pcm1.sL, a, b, pcm0.length, J, I, L);
  iir_frame_filter(pcm0.sR, pcm1.sR, a, b, pcm0.length, J, I, L);

  stereo_wave_write(&pcm1, "weekend.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}
