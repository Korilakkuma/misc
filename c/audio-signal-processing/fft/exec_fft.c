#include <stdio.h>
#include <stdlib.h>
#include "../wave/wave.h"
#include "fft.h"

enum {
  FFT_SIZE = 8
};

int main(void) {
  MONO_PCM pcm;

  double *x_real, *x_imag;

  char filename[] = "sample.wav";

  mono_wave_read(&pcm, filename);

  x_real = (double *)calloc(FFT_SIZE, sizeof(double));
  x_imag = (double *)calloc(FFT_SIZE, sizeof(double));

  for (int n = 0; n < FFT_SIZE; n++) {
    x_real[n] = pcm.s[n];
    x_imag[n] = 0.0;
  }

  FFT(x_real, x_imag, FFT_SIZE);

  for (int k = 0; k < FFT_SIZE; k++) {
    printf("[%d] %f + j%f\n", k, x_real[k], x_imag[k]);
  }

  free(pcm.s);
  free(x_real);
  free(x_imag);

  return 0;
}
