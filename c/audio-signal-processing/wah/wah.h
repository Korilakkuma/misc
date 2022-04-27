#include <math.h>
#include "../iir-filter/iir_filters.h"

enum {
  I = 2,
  J = 2,
  Q = 2
};

void Wah(double depth, double rate, double *in, double *out, double fs, int length) {
  double a[3];
  double b[3];

  for (int n = 0; n < length; n++) {
    // LFO
    double fc = (1000.0 + (depth * sin((2.0 * M_PI * rate * n) / fs))) / fs;

    IIR_RESONATOR(fc, Q, a, b);

    for (int m = 0; m <= J; m++) {
      if ((n - m) >= 0) {
        out[n] += b[m] * in[n - m];
      }
    }

    for (int m = 1; m <= I; m++) {
      if ((n - m) >= 0) {
        out[n] += -1 * a[m] * out[n - m];
      }
    }
  }
}
