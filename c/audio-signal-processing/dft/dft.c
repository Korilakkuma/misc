#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "dft.h"

enum {
  N = 8
};

int main(int argc, char **argv) {
  MONO_PCM pcm;

  double *x_real, *x_imag;
  double *X_real, *X_imag;
  double *w;

  mono_wave_read(&pcm, "sample.wav");

  x_real = (double *)calloc(N, sizeof(double));
  x_imag = (double *)calloc(N, sizeof(double));
  X_real = (double *)calloc(N, sizeof(double));
  X_imag = (double *)calloc(N, sizeof(double));
  w      = (double *)calloc(N, sizeof(double));

  hanning_window(w, N);

  for (int n = 0; n < N; n++) {
    if (argc > 1) {
      x_real[n] = w[n] * pcm.s[n];
    } else {
      x_real[n] = pcm.s[n];
    }

    x_imag[n] = 0.0;
  }

  fputs("x(n)\n", stdout);

  for (int n = 0; n < N; n++) {
    printf("[%d] %f\n", n, x_real[n]);
  }

  DFT(x_real, x_imag, X_real, X_imag, N);

  fputs("X(k)\n", stdout);

  for (int k = 0; k < N; k++) {
    if (X_imag[k] >= 0) {
      printf("[%d] %f + j%f\n", k, X_real[k], X_imag[k]);
    } else {
      printf("[%d] %f - j%f\n", k, X_real[k], fabs(X_imag[k]));
    }
  }

  for (int n = 0; n < N; n++) {
    x_real[n] = 0.0;
    x_imag[n] = 0.0;
  }

  IDFT(x_real, x_imag, X_real, X_imag, N);

  fputs("x(n)\n", stdout);

  for (int n = 0; n < N; n++) {
    printf("[%d] %f\n", n, x_real[n]);
  }

  free(pcm.s);
  free(x_real);
  free(x_imag);
  free(X_real);
  free(X_imag);
  free(w);

  return 0;
}
