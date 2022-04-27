#include <stdio.h>
#include <stdlib.h>
#include "../wave/wave.h"
#include "compressor.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    fputs("Require input filename, output filename, threshold, ratio\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0;

  double threshold = strtod(argv[3], NULL);
  double ratio     = strtod(argv[4], NULL);

  stereo_wave_read(&pcm0, argv[1]);

  Compressor(pcm0.sL, pcm0.sR, pcm0.length, threshold, ratio);

  stereo_wave_write(&pcm0, argv[2]);

  free(pcm0.sL);
  free(pcm0.sR);

  return 0;
}

