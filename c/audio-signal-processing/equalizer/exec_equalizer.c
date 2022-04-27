#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"
#include "equalizer.h"

enum {
  I = 2,
  J = 2
};

int main(int argc, char **argv) {
  if (argc != 4) {
    fputs("Require bass, middle, treble\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double bass   = strtod(argv[1], NULL);
  double middle = strtod(argv[2], NULL);
  double treble = strtod(argv[3], NULL);
  double Q      = 1.0 / sqrt(2.0);

  stereo_wave_read(&pcm0, "stereo.wav");

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm1.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm1.length, sizeof(double));

  Equalizer(pcm0.sL, pcm1.sL, pcm1.fs, pcm1.length, I, J, bass, middle, treble, Q);
  Equalizer(pcm0.sR, pcm1.sR, pcm1.fs, pcm1.length, I, J, bass, middle, treble, Q);

  stereo_wave_write(&pcm1, "equalizer.wav");

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}
