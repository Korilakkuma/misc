#include <vector>
#include <cmath>
#include <complex>

int pow2(int n) {
  if (n == 0) {
    return 1;
  }

  return 2 << (n - 1);
}

void FFT(std::vector<std::complex<double>> &x, int N) {
  std::vector<int> index(N);

  int number_of_stages = std::log2(N);

  for (int stage = 1; stage <= number_of_stages; stage++) {
    for (int i = 0; i < pow2(stage - 1); i++) {
      int rest = number_of_stages - stage;

      for (int j = 0; j < pow2(rest); j++) {
        int n = i * pow2(rest + 1) + j;
        int m = pow2(rest) + n;
        int r = j * pow2(stage - 1);

        double a_real = x[n].real();
        double a_imag = x[n].imag();
        double b_real = x[m].real();
        double b_imag = x[m].imag();
        double c_real = std::cos((2.0 * M_PI * r) / N);
        double c_imag = -std::sin((2.0 * M_PI * r) / N);

        if (stage < number_of_stages) {
          x[n] = std::complex<double>((a_real + b_real), (a_imag + b_imag));
          x[m] = std::complex<double>((c_real * (a_real - b_real)) - (c_imag * (a_imag - b_imag)), (c_real * (a_imag - b_imag)) + (c_imag * (a_real - b_real)));
        } else {
          x[n] = std::complex<double>((a_real + b_real), (a_imag + b_imag));
          x[m] = std::complex<double>((a_real - b_real), (a_imag - b_imag));
        }
      }
    }
  }

  for (int stage = 1; stage <= number_of_stages; stage++) {
    int rest = number_of_stages - stage;

    for (int i = 0; i < pow2(stage - 1); i++) {
      index[pow2(stage - 1) + i] = index[i] + pow2(rest);
    }
  }

  for (int k = 0; k < N; k++) {
    if (index[k] <= k) {
      continue;
    }

    auto tmp    = x[index[k]];
    x[index[k]] = x[k];
    x[k]        = tmp;
  }
}

void IFFT(std::vector<std::complex<double>> &x, int N) {
  std::vector<int> index(N);

  int number_of_stages = std::log2(N);

  for (int stage = 1; stage <= number_of_stages; stage++) {
    for (int i = 0; i < pow2(stage - 1); i++) {
      int rest = number_of_stages - stage;

      for (int j = 0; j < pow2(rest); j++) {
        int n = i * pow2(rest + 1) + j;
        int m = pow2(rest) + n;
        int r = j * pow2(stage - 1);

        double a_real = x[n].real();
        double a_imag = x[n].imag();
        double b_real = x[m].real();
        double b_imag = x[m].imag();
        double c_real = std::cos((2.0 * M_PI * r) / N);
        double c_imag = std::sin((2.0 * M_PI * r) / N);

        if (stage < number_of_stages) {
          x[n] = std::complex<double>((a_real + b_real), (a_imag + b_imag));
          x[m] = std::complex<double>((c_real * (a_real - b_real)) - (c_imag * (a_imag - b_imag)), (c_real * (a_imag - b_imag)) + (c_imag * (a_real - b_real)));
        } else {
          x[n] = std::complex<double>((a_real + b_real), (a_imag + b_imag));
          x[m] = std::complex<double>((a_real - b_real), (a_imag - b_imag));
        }
      }
    }
  }

  for (int stage = 1; stage <= number_of_stages; stage++) {
    int rest = number_of_stages - stage;

    for (int i = 0; i < pow2(stage - 1); i++) {
      index[pow2(stage - 1) + i] = index[i] + pow2(rest);
    }
  }

  for (int k = 0; k < N; k++) {
    if (index[k] <= k) {
      continue;
    }

    auto tmp    = x[index[k]];
    x[index[k]] = x[k];
    x[k]        = tmp;
  }

  for (int k = 0; k < N; k++) {
    x[k] = std::complex<double>((x[k].real() / N), (x[k].imag() / N));
  }
}
