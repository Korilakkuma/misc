#include "data.hpp"

static bool subset_summary_problem(std::vector<INT> &list, INT W);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./subset_summary_problem N W" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);
  INT W = std::stoll(argv[2]);

  std::vector<INT> list(N);

  init_list(list);
  dump_list(list);

  std::cout << std::endl;

  bool found = subset_summary_problem(list, W);

  std::cout << std::endl;

  if (found) {
    std::cout << "Found (" << W << ")" << std::endl;
  } else {
    std::cout << "Not found (" << W << ")" << std::endl;
  }

  return 0;
}

static bool subset_summary_problem(std::vector<INT> &list, INT W) {
  bool found = false;

  INT digits = 1 << list.size();  // std::floor(std::pow(2, list.size()));

  for (INT bits = 0; bits < digits; bits++) {
    INT sum = 0;

    for (INT i = 0; i < list.size(); i++) {
      if (bits & (1 << i)) {
        sum += list[i];
      }
    }

    std::cout << bits << " : " << sum << std::endl;

    if (sum == W) {
      found = true;
      break;
    }
  }

  return found;
}
