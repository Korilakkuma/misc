#include <iostream>
#include <cstdlib>

using INT = long long;

static INT euclidean_algorithm(INT n1, INT n2);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./euclidean_algorithm n1 n2" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT n1 = std::stoll(argv[1]);
  INT n2 = std::stoll(argv[1]);

  INT divisor = euclidean_algorithm(n1, n2);

  std::cout << "maximum divisor: " << divisor << std::endl;

  return 0;
}

static INT euclidean_algorithm(INT n1, INT n2) {
  if (n2 == 0) {
    return n1;
  }

  return euclidean_algorithm(n2, (n1 % n2));
}
