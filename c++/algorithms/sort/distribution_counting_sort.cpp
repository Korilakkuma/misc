#include "data.hpp"

#define DISTRIBUTION_SIZE 50

static void distribution_counting_sort(std::vector<INT> &list, std::vector<INT> &counts);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./distribution_counting_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);
  std::vector<INT> counts(DISTRIBUTION_SIZE, 0);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  distribution_counting_sort(list, counts);

  std::cout << std::endl;
  std::cout << N << ": ";

  for (INT i = 0; i < list.size(); i++) {
    std::cout << list[i] << " ";
  }

  std::cout << std::endl;

  return 0;
}

static void distribution_counting_sort(std::vector<INT> &list, std::vector<INT> &counts) {
  std::vector<INT> tmp(list.size());

  for (auto& l : list) {
    ++counts[l];
  }

  dump_list(counts, "counts       : ");

  for (INT i = 1; i < counts.size(); i++) {
    counts[i] += counts[i - 1];
  }

  dump_list(counts, "distributions: ");

  for (INT i = (list.size() - 1); i >= 0; i--) {
    tmp[--counts[list[i]]] = list[i];
  }

  for (INT i = 0; i < list.size(); i++) {
    list[i] = tmp[i];
  }
}
