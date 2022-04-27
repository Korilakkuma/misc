#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"
#include "time_stretch.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Require rate\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double rate = strtod(argv[1], NULL);

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = (int)(pcm0.length / rate) + 1;
  pcm1.sL     = (double *)calloc(pcm1.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm1.length, sizeof(double));

  TimeStretch(rate, pcm0.sL, pcm1.sL, pcm1.fs, pcm0.length);
  TimeStretch(rate, pcm0.sR, pcm1.sR, pcm1.fs, pcm0.length);

  stereo_wave_write(&pcm1, "time-stretch.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}
