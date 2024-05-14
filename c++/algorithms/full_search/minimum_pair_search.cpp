#include "data.hpp"

static INT minimum_pair_search(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./minimum_pair_search N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);
  dump_list(list);

  INT min = minimum_pair_search(list);

  std::cout << "min value is " << min << std::endl;

  return 0;
}

static INT minimum_pair_search(std::vector<INT> &list) {
  INT min = 1 << 16;

  for (INT i = 0; i < list.size(); i++) {
    for (INT j = 0; j < list.size(); j++) {
      if (i == j) {
        continue;
      }

      INT sum = list[i] + list[j];

      if (sum < min) {
        min = sum;
      }
    }
  }

  return min;
}
