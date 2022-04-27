#include <cmath>
#include <vector>

void Tremolo(
  double depth,
  double rate,
  std::vector<double> &in,
  std::vector<double> &out,
  double fs,
  int length
) {
  if (depth > 1) {
    depth = 1.0;
  }

  for (int n = 0; n < length; n++) {
    out[n] = in[n] * (1.0 + (depth * std::sin((2.0 * M_PI * rate * n) / fs)));
  }
}
