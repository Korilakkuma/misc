#include <stdlib.h>

void TimeStretch(double rate, double *in, double *out, int fs, int length) {
  if ((rate == 1.0) || (rate <= 0.0)) {
    for (int n = 0; n < length; n++) {
      out[n] = in[n];
    }

    return;
  }

  int template_size = (int)(fs * 0.01);
  int p_min = (int)(fs * 0.005);
  int p_max = (int)(fs * 0.02);

  double *x = (double *)calloc(template_size, sizeof(double));
  double *y = (double *)calloc(template_size, sizeof(double));
  double *r = (double *)calloc((p_max + 1), sizeof(double));

  int offset0 = 0;
  int offset1 = 0;

  while ((offset0 + (2 * p_max)) < length) {
    for (int n = 0; n < template_size; n++) {
      x[n] = in[offset0 + n];
    }

    double max_of_r = 0.0;
    int p = p_min;

    for (int m = p_min; m <= p_max; m++) {
      for (int n = 0; n < template_size; n++) {
        y[n] = in[offset0 + m + n];
      }

      r[m] = 0.0;

      for (int n = 0; n < template_size; n++) {
        r[m] += x[n] * y[n];
      }

      if (r[m] > max_of_r) {
        max_of_r = r[m];
        p = m;
      }
    }

    if (rate < 1.0) {
      for (int n = 0; n < p; n++) {
        out[offset1 + n] = in[offset0 + n];
      }
    }

    for (int n = 0; n < p; n++) {
      if (rate > 1.0) {
        out[offset1 + n] = (in[offset0 + n] * (p - n)) / p;
        out[offset1 + n] += (in[offset0 + p + n] * n) / p;
      } else if (rate < 1.0) {
        out[offset1 + p + n] = (in[offset0 + p + n] * (p - n)) / p;
        out[offset1 + p + n] += (in[offset0 + n] * n) / p;
      }
    }

    int q = 0;

    if (rate > 1.0) {
      q = (int)((p / (rate - 1.0)) + 0.5);
    } else if (rate < 1.0) {
      q = (int)(((p * rate) / (1.0 - rate)) + 0.5);
    }

    if (rate > 1.0) {
      for (int n = p; n < q; n++) {
        if ((offset0 + p + n) >= length) {
          break;
        }

        out[offset1 + n] = in[offset0 + p + n];
      }

      offset0 += p + q;
      offset1 += q;
    } else if (rate < 1.0) {
      for (int n = p; n < q; n++) {
        if ((offset0 + n) >= length) {
          break;
        }

        out[offset1 + p + n] = in[offset0 + n];
      }

      offset0 += q;
      offset1 += p + q;
    }
  }

  free(x);
  free(y);
  free(r);
}
