#include "data.hpp"

template<class T> void choice_min(T &a, T b) {
  if (a > b) {
    a = b;
  }
}

static INT relaxation_frog(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./relaxation_frog N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  list[0] = 0;

  dump_list(list);

  std::cout << std::endl;

  INT min = relaxation_frog(list);

  std::cout << "minimum value is " << min << std::endl;

  return 0;
}

static INT relaxation_frog(std::vector<INT> &list) {
  std::vector<INT> dp(list.size(), (1 << 16));

  dp[0] = 0;

  for (INT i = 0; i < list.size(); i++) {
    if ((i + 1) < list.size()) {
      choice_min(dp[i + 1], (dp[i] + abs(list[i + 1] - list[i])));
    }

    if ((i + 2) < list.size()) {
      choice_min(dp[i + 2], (dp[i] + abs(list[i + 2] - list[i])));
    }
  }

  return dp[dp.size() - 1];
}
