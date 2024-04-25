#include "data.hpp"

static void selection_sort(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./selection_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  selection_sort(list);

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void selection_sort(std::vector<INT> &list) {
  for (INT i = 0; i < (list.size() - 1); i++) {
    INT min_index = i;

    for (INT j = (i + 1); j < list.size(); j++) {
      if (list[j] < list[min_index]) {
        min_index = j;
      }
    }

    INT tmp;

    tmp             = list[i];
    list[i]         = list[min_index];
    list[min_index] = tmp;

    std::string label = std::to_string(i + 1) + ": ";

    dump_list(list, label);
  }
}
