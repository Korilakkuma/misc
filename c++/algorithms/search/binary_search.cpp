#include "data.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./binary_search N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N   = std::stoll(argv[1]);
  INT key = std::stoll(argv[2]);

  std::vector<INT> list(N, 0);

  init_list(list);

  std::sort(list.begin(), list.end());

  bool found = false;

  INT left  = 0;
  INT right = N -1;

  while (left <= right) {
    INT middle = (left + right) / 2;

    if (key == list[middle]) {
      found = true;
      break;
    } else if (key < list[middle]) {
      right = middle -1;
    } else if (key > list[middle]) {
      left = middle + 1;
    }
  }

  if (found) {
    std::cout << "Found (" << key << ") " << std::endl;
  } else {
    std::cout << "Not Found (" << key << ")" << std::endl;
  }

  dump_list(list);

  return 0;
}
