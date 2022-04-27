#include <stdio.h>
#include <stdlib.h>
#include "../wave/wave.h"
#include "resampling.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Require pitch\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double pitch = strtod(argv[1], NULL);

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = (int)(pcm0.length / pitch);
  pcm1.sL     = (double *)calloc(pcm1.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm1.length, sizeof(double));

  Resampling(pitch, pcm0.sL, pcm1.sL, pcm0.length);
  Resampling(pitch, pcm0.sR, pcm1.sR, pcm0.length);

  stereo_wave_write(&pcm1, "resampling.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}

