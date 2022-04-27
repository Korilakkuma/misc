#include <math.h>

void Chorus(double depth, double rate, double *in, double *out, double fs, int length) {
  if (depth > 0.02) {
    depth = 0.02;
  }

  for (int n = 0; n < length; n++) {
    double tau = (fs * 0.02) + ((fs * depth) * sin((2.0 * M_PI * rate * n) / fs));
    double t = (double)n - tau;
    int m = (int)t;
    double delta = t - (double)m;

    // Linear Interpolation
    if ((m >= 0) && ((m + 1) < length)) {
      out[n] = in[n] + ((delta * in[m + 1]) + ((1.0 - delta) * in[m]));
    }
  }
}
