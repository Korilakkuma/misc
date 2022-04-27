#include "../iir-filter/iir_filters.h"

enum {
  NUMBER_OF_BANDS = 3,
  BASS   = 500,
  MIDDLE = 1000,
  TREBLE = 2000
};

void Equalizer(
  double *s0,
  double *s1,
  double fs,
  int length,
  int I,
  int J,
  double bass,
  double middle,
  double treble,
  double Q) {
  double A[NUMBER_OF_BANDS][3];
  double B[NUMBER_OF_BANDS][3];
  double a[3];
  double b[3];

  IIR_LOW_SHELVING((double)BASS / fs, Q, bass, a, b);

  for (int m = 0; m <= I; m++) {
    A[0][m] = a[m];
  }

  for (int m = 0; m <= J; m++) {
    B[0][m] = b[m];
  }

  IIR_PEAKING((double)MIDDLE / fs, Q, middle, a, b);

  for (int m = 0; m <= I; m++) {
    A[1][m] = a[m];
  }

  for (int m = 0; m <= J; m++) {
    B[1][m] = b[m];
  }

  IIR_HIGH_SHELVING((double)TREBLE / fs, Q, treble, a, b);

  for (int m = 0; m <= I; m++) {
    A[2][m] = a[m];
  }

  for (int m = 0; m <= J; m++) {
    B[2][m] = b[m];
  }

  for (int i = 0; i < NUMBER_OF_BANDS; i++) {
    for (int n = 0; n < length; n++) {
      s1[n] = 0.0;
    }

    for (int n = 0; n < length; n++) {
      for (int m = 0; m <= J; m++) {
        if ((n - m) >= 0) {
          s1[n] += B[i][m] * s0[n - m];
        }
      }

      for (int m = 1; m <= I; m++) {
        if ((n - m) >= 0) {
          s1[n] += -A[i][m] * s1[n - m];
        }
      }
    }

    for (int n = 0; n < length; n++) {
      s0[n] = s1[n];
    }
  }
}
