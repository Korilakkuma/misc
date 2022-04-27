#include <vector>
#include <cmath>

void hanning_window(std::vector<double> &w, int N) {
  if ((N % 2) == 0) {
    for (int n = 0; n < N; n++) {
      w[n] = 0.5 - (0.5 * std::cos((2.0 * M_PI * n) / N));
    }
  } else {
    for (int n = 0; n < N; n++) {
      w[n] = 0.5 - (0.5 * std::cos((2.0 * M_PI * (n - 0.5)) / N));
    }
  }
}

void sine_window(std::vector<double> &w, int N) {
  for (int n = 0; n < N; n++) {
    w[n] = std::sin(M_PI * (static_cast<double>(n) / N));
  }
}

void vorbis_window(std::vector<double> &w, int N) {
  for (int n = 0; n < N; n++) {
    w[n] = std::sin((M_PI / 2.0) * std::pow(std::sin(M_PI * (static_cast<double>(n) / N)), 2.0));
  }
}
