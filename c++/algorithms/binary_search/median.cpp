#include "data.hpp"

static INT median(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./median N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::sort(list.begin(), list.end());

  dump_list(list);

  INT m = median(list);

  std::cout << std::endl;
  std::cout << "median is " << m << std::endl;

  return 0;
}

static INT median(std::vector<INT> &list) {
  INT n = (list.size() - 1) / 2;

  return list[n];
}
