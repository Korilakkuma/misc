#include <iostream>

using INT = long long;

static INT fibonacci(INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./fibonacci N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  INT a_n = fibonacci(N);

  std::cout << "fibonacci(" << N << "): " << a_n << std::endl;

  return 0;
}

static INT fibonacci(INT N) {
  if (N == 0) {
    return 0;
  }

  if (N == 1) {
    return 1;
  }

  return fibonacci(N - 2) + fibonacci(N - 1);
}
