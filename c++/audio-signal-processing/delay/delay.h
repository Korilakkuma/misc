#include <vector>
#include <cmath>

void Delay(
  std::vector<double> &dryL,
  std::vector<double> &mixL,
  std::vector<double> &dryR,
  std::vector<double> &mixR,
  int length,
  double wet,
  double time,
  int feedback) {
  for (int n = 0; n < length; n++) {
    mixL[n] = dryL[n];
    mixR[n] = dryR[n];

    for (int i = 1; i <= feedback; i++) {
      int m = static_cast<int>(static_cast<double>(n) - static_cast<double>(i * time));

      if ((m >= 0) && (m < length)) {
        mixL[n] = mixL[n] + pow(wet, (double)i) * dryL[m];
        mixR[n] = mixR[n] + pow(wet, (double)i) * dryR[m];
      } else if (m >= length) {
        break;
      }
    }
  }
}
