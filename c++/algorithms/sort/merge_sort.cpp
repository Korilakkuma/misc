#include "data.hpp"

static void merge_sort(std::vector<INT> &list, std::vector<INT> &tmp_list, INT low, INT high);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./merge_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);
  std::vector<INT> tmp_list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  merge_sort(list, tmp_list, 0, (list.size() - 1));

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void merge_sort(std::vector<INT> &list, std::vector<INT> &tmp_list, INT low, INT high) {
  if (low >= high) {
    return;
  }

  INT m = (low + high) / 2;

  merge_sort(list, tmp_list, low, m);
  merge_sort(list, tmp_list, (m + 1), high);

  INT i = low;
  INT l = low;
  INT h = high;

  while (l <= m) {
    tmp_list[i++] = list[l++];
  }

  while (h > m) {
    tmp_list[i++] = list[h--];
  }

  l = low;
  h = high;

  for (INT k = low; k <= high; k++) {
    if (tmp_list[l] <= tmp_list[h]) {
      list[k] = tmp_list[l++];
    } else {
      list[k] = tmp_list[h--];
    }
  }

  std::string label = "low: " + std::to_string(low) + " high: " + std::to_string(high) + " : ";

  dump_list(list, label);
}
