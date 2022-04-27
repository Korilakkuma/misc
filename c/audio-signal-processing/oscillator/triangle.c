#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"

enum {
  N = 15
};

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Require amplitude and f0\n", stderr);
    exit(EXIT_FAILURE);
  }

  MONO_PCM pcm;
  char *endptr;

  double a  = strtod(argv[1], &endptr);
  double f0 = strtod(argv[2], &endptr);

  pcm.fs = 8000;
  pcm.bits = 16;
  pcm.length = 8000;

  if ((pcm.s = (double *)calloc(pcm.length, sizeof(double))) == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  for (int n = 0; n < pcm.length; n++) {
    for (int i = 0; i < N; i++) {
      if ((i % 2) != 0) {
        pcm.s[n] += (a / pow(i, 2)) * sin(2.0 * M_PI * (i * f0) * n / pcm.fs);
      }
    }
  }

  mono_wave_write(&pcm, "triangle.wav");

  free(pcm.s);

  return 0;
}
