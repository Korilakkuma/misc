#include "data.hpp"

static bool subset_summary_problem(std::vector<INT> &list, INT n, INT W);

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

  bool found = subset_summary_problem(list, N, W);

  std::cout << std::endl;

  if (found) {
    std::cout << "Found (" << W << ")" << std::endl;
  } else {
    std::cout << "Not found (" << W << ")" << std::endl;
  }

  return 0;
}

static bool subset_summary_problem(std::vector<INT> &list, INT n, INT W) {
  if (n == 0) {
    if (W == 0) {
      return true;
    }

    return false;
  }

  if (subset_summary_problem(list, (n - 1), (W - 0))) {
    return true;
  }

  if (subset_summary_problem(list, (n - 1), (W - list[n - 1]))) {
    return true;
  }

  return false;
}
