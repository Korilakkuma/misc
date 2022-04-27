#include <vector>

void Distortion(
  std::vector<double> &dryL,
  std::vector<double> &dryR,
  int length,
  double gain,
  double level) {
  for (int n = 0; n < length; n++) {
    dryL[n] = dryL[n] * gain;
    dryR[n] = dryR[n] * gain;

    if (dryL[n] > 1.0) {
      dryL[n] = 1.0;
    }

    if (dryR[n] < -1.0) {
      dryR[n] = -1.0;
    }

    dryL[n] = dryL[n] * level;
    dryR[n] = dryR[n] * level;
  }
}
