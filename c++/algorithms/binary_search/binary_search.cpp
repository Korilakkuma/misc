#include "data.hpp"

static INT binary_search(std::vector<INT> &list, INT key);

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

  dump_list(list);

  std::cout << std::endl;

  INT found_index = binary_search(list, key);

  if (found_index != -1) {
    std::cout << "Found (" << key << ") is index " << found_index << std::endl;
  } else {
    std::cout << "Not Found (" << key << ")" << std::endl;
  }

  return 0;
}

static INT binary_search(std::vector<INT> &list, INT key) {
  INT left  = 0;
  INT right = list.size() -1;

  while (left <= right) {
    INT middle = (left + right) / 2;

    if (key == list[middle]) {
      return middle;
    } else if (key < list[middle]) {
      right = middle -1;
    } else if (key > list[middle]) {
      left = middle + 1;
    }
  }

  return -1;
}
