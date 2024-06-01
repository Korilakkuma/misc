#include "data.hpp"

static INT minimum_pair_search(std::vector<INT> &a, std::vector<INT> &b, INT M);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./minimum_pair_search N M" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);
  INT M = std::stoll(argv[2]);

  std::vector<INT> a(N);
  std::vector<INT> b(N);

  init_list(a);
  init_list(b);

  std::sort(b.begin(), b.end());

  b.push_back(1 << 16);

  dump_list(a);
  dump_list(b);

  INT min = minimum_pair_search(a, b, M);

  std::cout << std::endl;
  std::cout << "minimum value is " << min << " (greater than or equal to " << M << ")" << std::endl;

  return 0;
}

static INT minimum_pair_search(std::vector<INT> &a, std::vector<INT> &b, INT M) {
  INT min = 1 << 16;

  for (INT i = 0; i < a.size(); i++) {
    auto iterator =  std::lower_bound(b.begin(), b.end(), (M - a[i]));

    INT value = *iterator;

    if ((a[i] + value) < min) {
      min = a[i] + value;
    }
  }

  return min;
}
