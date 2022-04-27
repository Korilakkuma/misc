#include <cmath>
#include <vector>

void Vibrate(
  double depth,
  double rate,
  std::vector<double> &in,
  std::vector<double> &out,
  double fs,
  int length
) {
  if (depth > 0.02) {
    depth = 0.02;
  }

  for (int n = 0; n < length; n++) {
    double tau = (fs * 0.02) + ((fs * depth) * std::sin((2.0 * M_PI * rate * n) / fs));
    double t = static_cast<double>(n) - tau;
    int m = (int)t;
    double delta = t - static_cast<double>(m);

    if ((m >= 0) && (m + 1) < length) {
      out[n] = (delta * in[m + 1]) + ((1 - delta) * in[m]);
    }
  }
}
