#include "data.hpp"

static inline INT min(INT a, INT b);
static INT difference_between(std::vector<std::vector<INT>> &list, INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./difference_between N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<std::vector<INT>> list((N + 1), std::vector<INT>(N + 1));

  for (INT i = 0; i <= N; i++) {
    for (INT j = 0; j <= N; j++) {
      list[i][j] = generate_random_value(10);
    }

    std::string label = std::to_string(i) + ": ";

    dump_list(list[i], label);
  }

  std::cout << std::endl;

  INT min = difference_between(list, (N + 1));

  std::cout << "minimum value is " << min << std::endl;

  return 0;
}

static INT difference_between(std::vector<std::vector<INT>> &list, INT N) {
  std::vector<INT> dp(N, (1 << 16));

  dp[0] = 0;

  for (INT i = 0; i < N; i++) {
    for (INT j = 0; j < i; j++) {
      dp[i] = min(dp[i], (dp[j] + list[j][i]));
    }

    std::string label = std::to_string(i) + ": ";

    dump_list(dp, label);
  }

  return dp[N - 1];
}

static inline INT min(INT a, INT b) {
  return a < b ? a : b;
}
