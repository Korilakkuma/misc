#include "data.hpp"

static inline void swap(INT &a, INT &b) {
  INT t;

  t = a;
  a = b;
  b = t;
}

static void shell_sort(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./shell_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  shell_sort(list);

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void shell_sort(std::vector<INT> &list) {
  INT h = list.size() / 2;

  /*
  INT h = 1;

  while (h < (list.size() / 9)) {
    h = (3 * h) + 1;
  }
  */

  while (h > 0) {
    for (INT i = h; i < list.size(); i++) {
      for (INT j = i; j >= h; j -= h) {
        if (list[j - h] > list[j]) {
          swap(list[j - h], list[j]);
        }
      }
    }

    std::string label = "h(" + std::to_string(h) + ") - sort: ";

    dump_list(list, label);

    h /= 2;
  }
}
