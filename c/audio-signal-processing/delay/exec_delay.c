#include <stdio.h>
#include <stdlib.h>
#include "../wave/wave.h"
#include "delay.h"

int main(int argc, char **argv) {
  if (argc != 6) {
    fputs("Reqiure input filename, output filename, wet, time, feedback\n", stderr);
    exit(EXIT_FAILURE);
  }

  STEREO_PCM pcm0, pcm1;

  double wet   = strtod(argv[3], NULL);
  double time  = strtod(argv[4], NULL);
  int feedback = strtol(argv[5], NULL, 10);

  stereo_wave_read(&pcm0, argv[1]);

  pcm1.fs     = pcm0.fs;
  pcm1.bits   = pcm0.bits;
  pcm1.length = pcm0.length;
  pcm1.sL     = (double *)calloc(pcm0.length, sizeof(double));
  pcm1.sR     = (double *)calloc(pcm0.length, sizeof(double));

  Delay(pcm0.sL, pcm1.sL, pcm0.sR, pcm1.sR, pcm1.length, wet, (pcm1.fs * time), feedback);

  stereo_wave_write(&pcm1, argv[2]);

  free(pcm0.sL);
  free(pcm1.sL);
  free(pcm0.sR);
  free(pcm1.sR);

  return 0;
}
