#include "data.hpp"

template<class T> void choice_min(T &a, T b) {
  if (a > b) {
    a = b;
  }
}

static INT memoized_frog(std::vector<INT> &list, std::vector<INT> &memoized_dp, INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./memoized_frog N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  list[0] = 0;

  dump_list(list);

  std::cout << std::endl;

  std::vector<INT> dp(list.size(), (1 << 16));

  INT min = memoized_frog(list, dp, (N - 1));

  std::cout << "minimum value is " << min << std::endl;

  return 0;
}

static INT memoized_frog(std::vector<INT> &list, std::vector<INT> &memoized_dp, INT n) {
  if (n == 0) {
    memoized_dp[0] = list[0];
    return memoized_dp[0];
  }

  if (n == 1) {
    memoized_dp[1] = list[1];
    return memoized_dp[1];
  }

  INT v = 1 << 16;

  choice_min(v, (memoized_frog(list, memoized_dp, (n - 1)) + abs(list[n] - list[n - 1])));

  if (n >= 2) {
    choice_min(v, (memoized_frog(list, memoized_dp, (n - 2)) + abs(list[n] - list[n - 2])));
  }

  memoized_dp[n] = v;

  return memoized_dp[n];
}
