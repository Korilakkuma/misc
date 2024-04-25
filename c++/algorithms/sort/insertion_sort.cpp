#include "data.hpp"

static inline void swap(INT &a, INT &b) {
  INT t;

  t = a;
  a = b;
  b = t;
}

static void insertion_sort(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./insertion_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  insertion_sort(list);

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void insertion_sort(std::vector<INT> &list) {
  for (INT i = 1; i < list.size(); i++) {
    for (INT j = i; j >= 1; j--) {
      if (list[j - 1] > list[j]) {
        swap(list[j - 1], list[j]);
      }
    }

    std::string label = std::to_string(i) + ": ";

    dump_list(list, label);
  }
}
