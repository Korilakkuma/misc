#include "data.hpp"

static inline INT min(INT a, INT b);
static INT frog(std::vector<INT> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./frog N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  list[0] = 0;

  dump_list(list);

  std::cout << std::endl;

  INT min = frog(list);

  std::cout << "minimum value is " << min << std::endl;

  return 0;
}

static INT frog(std::vector<INT> &list) {
  std::vector<INT> dp(list.size(), (1 << 16));

  dp[0] = 0;

  for (INT i = 1; i < list.size(); i++) {
    if (i == 1) {
      dp[1] = list[1];
      continue;
    }

    dp[i] = min((dp[i - 1] + abs(list[i] - list[i - 1])), (dp[i - 2] + abs(list[i] - list[i - 2])));
  }

  return dp[dp.size() - 1];
}

static inline INT min(INT a, INT b) {
  return a < b ? a : b;
}
