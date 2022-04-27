#include <stdlib.h>
#include "../window/window_functions.h"
#include "../fft/fft.h"

void fir_frame_filter(double *s0, double *s1, double *b, int length, int J, int L, int N) {
  int number_of_frames = length / L;

  double *b_real = (double *)calloc(N, sizeof(double));
  double *b_imag = (double *)calloc(N, sizeof(double));

  double *x_real = (double *)calloc(N, sizeof(double));
  double *x_imag = (double *)calloc(N, sizeof(double));

  double *y_real = (double *)calloc(N, sizeof(double));
  double *y_imag = (double *)calloc(N, sizeof(double));

  for (int frame = 0; frame < number_of_frames; frame++) {
    int offset = L * frame;

    for (int n = 0; n < N; n++) {
      x_real[n] = 0.0;
      x_imag[n] = 0.0;
    }

    for (int n = 0; n < L; n++) {
      x_real[n] = s0[offset + n];
    }

    FFT(x_real, x_imag, N);

    for (int m = 0; m < N; m++) {
      b_real[m] = 0.0;
      b_imag[m] = 0.0;
    }

    for (int m = 0; m < J; m++) {
      b_real[m] = b[m];
    }

    FFT(b_real, b_imag, N);

    for (int k = 0; k < N; k++) {
      y_real[k] = (b_real[k] * x_real[k]) - (b_imag[k] * x_imag[k]);
      y_imag[k] = (b_real[k] * x_imag[k]) + (b_imag[k] * x_real[k]);
    }

    IFFT(y_real, y_imag, N);

    for (int n = 0; n < (2 * L); n++) {
      if ((offset + n) < length) {
        s1[offset + n] += y_real[n];
      }
    }
  }

  free(b_real);
  free(b_imag);
  free(x_real);
  free(x_imag);
  free(y_real);
  free(y_imag);
}

void iir_frame_filter(double *s0, double *s1, double *a, double *b, int length, int J, int I, int L) {
  int number_of_frames = length / L;

  double *x = (double *)calloc((L + J), sizeof(double));
  double *y = (double *)calloc((L + I), sizeof(double));

  for (int frame = 0; frame < number_of_frames; frame++) {
    int offset = L * frame;

    for (int n = 0; n < L + J; n++) {
      if ((offset - J + n) < 0) {
        x[n] = 0.0;
      } else {
        x[n] = s0[offset - J + n];
      }
    }

    for (int n = 0; n < L + I; n++) {
      if ((offset - I + n) < 0) {
        y[n] = 0.0;
      } else {
        y[n] = s1[offset - I + n];
      }
    }

    for (int n = 0; n < L; n++) {
      for (int m = 0; m <= J; m++) {
        y[I + n] += b[m] * x[J + (n - m)];
      }

      for (int m = 1; m <= I; m++) {
        y[I + n] += -a[m] * y[I + (n - m)];
      }
    }

    for (int n = 0; n < L; n++) {
      s1[offset + n] = y[I + n];
    }
  }

  free(x);
  free(y);
}
