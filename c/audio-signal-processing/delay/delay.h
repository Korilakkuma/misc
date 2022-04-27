#include <math.h>

void Delay(double *dryL, double *mixL, double *dryR, double *mixR, int length, double wet, double time, int feedback) {
  for (int n = 0; n < length; n++) {
    mixL[n] = dryL[n];
    mixR[n] = dryR[n];

    for (int i = 1; i <= feedback; i++) {
      int m = (int)((double)n - (double)(i * time));

      if ((m >= 0) && (m < length)) {
        mixL[n] += pow(wet, (double)i) * dryL[m];
        mixR[n] += pow(wet, (double)i) * dryR[m];
      } else if (m >= length) {
        break;
      }
    }
  }
}
