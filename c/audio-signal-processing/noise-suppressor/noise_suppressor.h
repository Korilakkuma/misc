#include <math.h>
#include "../window/window_functions.h"
#include "../fft/fft.h"

enum {
  N = 1024
};

void NoiseSuppressor(double threshold, double *in, double *out, int fs, int length) {
  int number_of_frame = (length - (N / 2)) / (N / 2);

  double *w      = (double *)calloc(N, sizeof(double));
  double *x_real = (double *)calloc(N, sizeof(double));
  double *x_imag = (double *)calloc(N, sizeof(double));
  double *A      = (double *)calloc(N, sizeof(double));
  double *T      = (double *)calloc(N, sizeof(double));
  double *y_real = (double *)calloc(N, sizeof(double));
  double *y_imag = (double *)calloc(N, sizeof(double));

  hanning_window(w, N);

  for (int frame = 0; frame < number_of_frame; frame++) {
    int offset = (N / 2) * frame;

    for (int n = 0; n < N; n++) {
      x_real[n] = in[offset + n] * w[n];
      x_imag[n] = 0.0;
    }

    FFT(x_real, x_imag, N);

    for (int k = 0; k < N; k++) {
      A[k] = sqrt((x_real[k] * x_real[k]) + (x_imag[k] * x_imag[k]));

      if ((x_imag[k] != 0.0) && (x_real[k] != 0.0)) {
        T[k] = atan2(x_imag[k], x_real[k]);
      }
    }

    for (int k = 0; k < N; k++) {
      A[k] -= threshold;

      if (A[k] < 0.0) {
        A[k] = 0.0;
      }
    }

    for (int k = 0; k < N; k++) {
      y_real[k] = A[k] * cos(T[k]);
      y_imag[k] = A[k] * sin(T[k]);
    }

    IFFT(y_real, y_imag, N);

    for (int n = 0; n < N; n++) {
      out[offset + n] += y_real[n];
    }
  }

  free(w);
  free(x_real);
  free(x_imag);
  free(A);
  free(T);
  free(y_real);
  free(y_imag);
}
