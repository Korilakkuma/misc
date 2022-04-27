#include <vector>

void Stereo(
  double delay,
  std::vector<double> &inL,
  std::vector<double> &inR,
  std::vector<double> &outL,
  std::vector<double> &outR,
  int fs,
  int length
) {
  double d = fs * delay;

  for (int n = 0; n < length; n++) {
    int m = static_cast<int>(static_cast<double>(n) - d);

    if (m >= 0) {
      outL[n] = inL[n] + inL[m];
      outR[n] = inR[n] - inR[m];
    }
  }
}
