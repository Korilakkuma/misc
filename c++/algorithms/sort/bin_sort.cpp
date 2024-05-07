#include "data.hpp"

#define EMPTY    -1
#define BIN_SIZE 100

static void bin_sort(std::vector<INT> &list, std::vector<INT> &bin);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./bin_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);
  std::vector<INT> bin(BIN_SIZE, EMPTY);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  bin_sort(list, bin);

  std::cout << std::endl;
  std::cout << N << ": ";

  for (INT i = 0; i < bin.size(); i++) {
    if (bin[i] == EMPTY) {
      continue;
    }

    std::cout << bin[i] << " ";
  }

  std::cout << std::endl;

  return 0;
}

static void bin_sort(std::vector<INT> &list, std::vector<INT> &bin) {
  for (auto& l : list) {
    bin[l] = l;
  }
}
