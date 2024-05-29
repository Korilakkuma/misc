#include "data.hpp"

static inline bool predicate(INT a, INT b);
static INT lower_bound(std::vector<INT> &list, INT key);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./lower_bound N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N   = std::stoll(argv[1]);
  INT key = std::stoll(argv[2]);

  std::vector<INT> list(N);

  init_list(list);

  std::sort(list.begin(), list.end());

  dump_list(list);

  std::cout << std::endl;

  INT found_index = lower_bound(list, key);

  std::cout << "lower bound (" << key << ") is index greater than " << found_index << std::endl;

  return 0;
}

static INT lower_bound(std::vector<INT> &list, INT key) {
  INT left  = 0;
  INT right = list.size() -1;

  while ((right - left) > 1) {
    INT middle = (left + right) / 2;

    if (predicate(key, list[middle])) {
      right = middle;
    } else {
      left = middle;
    }
  }

  return right;
}

static inline bool predicate(INT a, INT b) {
  return a < b;
}
