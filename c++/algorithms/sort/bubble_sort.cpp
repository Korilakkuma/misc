#include "data.hpp"

static inline void swap(INT &a, INT &b) {
  INT t;

  t = a;
  a = b;
  b = t;
}

static void bubble_sort(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./bubble_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  bubble_sort(list);

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void bubble_sort(std::vector<INT> &list) {
  for (INT i = 0; i < (list.size() - 1); i++) {
    for (INT j = (list.size() - 1); j > i; j--) {
      if (list[j - 1] > list[j]) {
        swap(list[j - 1], list[j]);
      }
    }

    std::string label = std::to_string(i + 1) + ": ";

    dump_list(list, label);
  }
}
