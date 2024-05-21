#include "data.hpp"

static inline INT max(INT a, INT b);
static INT knapsack_problem(std::vector<INT> &values, std::vector<INT> &weights, INT N, INT W);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./knapsack_problem N W" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);
  INT W = std::stoll(argv[2]);

  std::vector<INT> values(N);
  std::vector<INT> weights(N);

  init_list(values);
  init_list(weights);

  dump_list(values, "values : ");
  dump_list(weights, "weights: ");

  std::cout << std::endl;

  INT max = knapsack_problem(values, weights, (N + 1), (W + 1));

  std::cout << std::endl;

  std::cout << "maximum value is " << max << std::endl;

  return 0;
}

static INT knapsack_problem(std::vector<INT> &values, std::vector<INT> &weights, INT N, INT W) {
  std::vector<std::vector<INT>> dp(N, std::vector<INT>(W, 0));

  for (INT n = 0; n < (N - 1); n++) {
    for (INT w = 0; w < W; w++) {
      if ((w - weights[n]) >= 0) {
        dp[n + 1][w] = max(dp[n + 1][w], (dp[n][w - weights[n]] + values[n]));
      }

      dp[n + 1][w] = max(dp[n + 1][w], dp[n][w]);
    }

    std::string label = std::to_string(n) +  ": ";

    dump_list(dp[n], label);
  }

  return dp[N - 1][W - 1];
}

static inline INT max(INT a, INT b) {
  return a > b ? a : b;
}
