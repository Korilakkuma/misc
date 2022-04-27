#include <string.h>
#include <math.h>

enum {
  J = 24
};

double sinc(double x) {
  if (x == 0.0) {
    return 1.0;
  }

  return sin(x) / x;
}

void Resampling(double pitch, double *in, double *out, int length) {
  if (pitch <= 0) {
    return;
  }

  for (int n = 0; n < (length / pitch); n++) {
    double t = pitch * n;
    int offset = (int)t;

    for (int m = (offset - (J / 2)); m <= (offset + (J / 2)); m++) {
      if ((m >= 0) && (m < length)) {
        out[n] += in[m] * sinc(M_PI * (t - m));
      }
    }
  }
}
