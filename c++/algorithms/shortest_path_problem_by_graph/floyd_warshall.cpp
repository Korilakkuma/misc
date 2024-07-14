#include "data.hpp"

static const INT INF = 1 << 16;

static std::vector<std::vector<INT>> graph_source = {
  { 0, 1, 3 },
  { 0, 3, 100 },
  { 1, 2, 50 },
  { 1, 3, 57 },
  { 1, 4, 4 },
  { 2, 3, 10 },
  { 2, 4, 5 },
  { 3, 1, 5 },
  { 4, 2, 5 },
  { 4, 3, 25 },
  { 4, 5, 8 }
};


static void floyd_warshall();

template <class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;

    return true;
  }

  return false;
}

int main(int argc, char **argv) {
  floyd_warshall();

  return 0;
}

static void floyd_warshall() {
  INT N = 5;
  INT M = graph_source.size();

  std::vector<std::vector<INT>> dp(N, std::vector<INT>(N, INF));

  for (INT e = 0; e < M; e++) {
    std::vector<INT> ve = graph_source[e];

    dp[ve[0]][ve[1]] = ve[2];
  }

  for (INT v = 0; v < N; v++) {
    dp[v][v] = 0;
  }

  for (INT k = 0; k < N; k++) {
    for (INT i = 0; i < N; i++) {
      for (INT j = 0; j < N; j++) {
        chmin(dp[i][j], (dp[i][k] + dp[k][j]));
      }
    }
  }

  bool has_negative_cycle = false;

  for (INT v = 0; v < N; v++) {
    if (dp[v][v] < 0) {
      has_negative_cycle = true;
      break;
    }
  }

  if (has_negative_cycle) {
    std::cout << "There is negative cycle." << std::endl;
  } else {
    for (INT i = 0; i < N; i++) {
      for (INT j = 0; j < N; j++) {
        if (j > 0) {
          std::cout << " ";
        }

        if (dp[i][j] < (INF / 2)) {
          std::cout << dp[i][j];
        } else {
          std::cout << dp[i][j] << " is not relaxation.";
        }
      }

      std::cout << std::endl;
    }
  }
}
