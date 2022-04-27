#include <math.h>

void DFT(double *x_real, double *x_imag, double *X_real, double *X_imag, int N) {
  for (int k = 0; k < N; k++) {
    for (int n = 0; n < N; n++) {
      double W_real =  cos((2 * M_PI * k * n) / N);
      double W_imag = -sin((2 * M_PI * k * n) / N);

      X_real[k] += (W_real * x_real[n]) - (W_imag * x_imag[n]);
      X_imag[k] += (W_real * x_imag[n]) + (W_imag * x_real[n]);
    }
  }
}

void IDFT(double *x_real, double *x_imag, double *X_real, double *X_imag, int N) {
  for (int n = 0; n < N; n++) {
    for (int k = 0; k < N; k++) {
      double W_real = cos((2 * M_PI * k * n) / N);
      double W_imag = sin((2 * M_PI * k * n) / N);

      x_real[n] += (W_real * X_real[k]) - (W_imag * X_imag[k]);
      x_imag[n] += (W_real * X_imag[k]) + (W_imag * X_real[k]);
    }

    x_real[n] /= N;
    x_imag[n] /= N;
  }
}
