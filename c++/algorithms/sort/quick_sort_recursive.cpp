#include "data.hpp"

static inline void swap(INT &a, INT &b) {
  INT t;

  t = a;
  a = b;
  b = t;
}

static void quick_sort_recursive(std::vector<INT> &list, INT low, INT high);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./quick_sort_recursive N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  quick_sort_recursive(list, 0, (list.size() - 1));

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void quick_sort_recursive(std::vector<INT> &list, INT low, INT high) {
  if (low >= high) {
    return;
  }

  INT n = high - low;
  INT l = low;
  INT h = high - 1;

  INT pivot = (INT)(n * ((double)std::rand() / (RAND_MAX + 1.0))) + low;
  // INT pivot = high;

  std::cout << "pivot: " << pivot << std::endl;

  swap(list[pivot], list[high]);

  pivot = high;

  while (true) {
    while (list[l] < list[pivot]) {
      ++l;
    }

    while ((l < h) && (list[h] > list[pivot])) {
      --h;
    }

    if (l >= h) {
      break;
    }

    swap(list[l], list[h]);

    ++l;
    --h;
  }

  swap(list[l], list[pivot]);

  dump_list(list, (std::to_string(n) + ": "));

  quick_sort_recursive(list, low, (l - 1));
  quick_sort_recursive(list, (l + 1), high);
}
