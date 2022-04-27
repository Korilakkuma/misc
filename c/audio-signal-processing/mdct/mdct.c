#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../wave/wave.h"
#include "../window/window_functions.h"
#include "mdct.h"

enum {
  N = 8
};

int main(void) {
  MONO_PCM pcm;

  double *x = (double *)calloc((2 * N), sizeof(double));
  double *X = (double *)calloc(N,       sizeof(double));
  double *w = (double *)calloc((2 * N), sizeof(double));

  mono_wave_read(&pcm, "sample.wav");

  fputs("x(n)\n", stdout);

  sine_window(w, (2 * N));

  for (int n = 0; n < (2 * N); n++) {
    x[n] = w[n] * pcm.s[n];
    printf("[%d] %f\n", n, x[n]);
  }

  MDCT(x, X, N);

  fputs("X(k)\n", stdout);

  for (int k = 0; k < N; k++) {
    printf("[%d] %f\n", k, X[k]);
  }

  for (int n = 0; n < (2 * N); n++) {
    x[n] = 0.0;
  }

  IMDCT(x, X, N);

  fputs("x(n)\n", stdout);

  for (int n = 0; n < (2 * N); n++) {
    x[n] *= w[n];
    printf("[%d] %f\n", n, x[n]);
  }

  free(pcm.s);
  free(x);
  free(X);
  free(w);

  return 0;
}
