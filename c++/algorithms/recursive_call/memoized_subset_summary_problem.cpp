#include "data.hpp"

static bool memoized_subset_summary_problem(std::vector<INT> &list, std::vector<bool> &memos, INT n, INT W);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./memoized_subset_summary_problem N W" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);
  INT W = std::stoll(argv[2]);

  std::vector<INT> list(N);
  std::vector<bool> memos(N, false);

  init_list(list);
  dump_list(list);

  std::cout << std::endl;

  bool found = memoized_subset_summary_problem(list, memos, N, W);

  std::cout << std::endl;

  if (found) {
    std::cout << "Found (" << W << ")" << std::endl;
  } else {
    std::cout << "Not found (" << W << ")" << std::endl;
  }

  return 0;
}

static bool memoized_subset_summary_problem(std::vector<INT> &list, std::vector<bool> &memos, INT n, INT W) {
  if (memos[n - 1]) {
    return memos[n - 1];
  }

  if (n == 0) {
    if (W == 0) {
      memos[0] = true;
    } else {
      memos[0] = false;
    }

    return memos[0];
  }

  memos[n - 1] = memoized_subset_summary_problem(list, memos, (n - 1), (W - 0));

  if (memos[n - 1]) {
    return true;
  }

  memos[n - 1] = memoized_subset_summary_problem(list, memos, (n - 1), (W - list[n - 1]));

  if (memos[n - 1]) {
    return true;
  }

  return false;
}
