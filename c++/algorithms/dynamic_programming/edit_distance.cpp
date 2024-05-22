#include "data.hpp"

template<class T> void choice_min(T &a, T b) {
  if (a > b) {
    a = b;
  }
}

static INT edit_distance(std::string &src, std::string &dest);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./edit_distance src dest" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string src  = argv[1];
  std::string dest = argv[2];

  INT diff = edit_distance(src, dest);

  std::cout << std::endl;
  std::cout << "edit distance is " << diff << std::endl;

  return 0;
}

static INT edit_distance(std::string &src, std::string &dest) {
  INT number_of_rows = src.length() + 1;
  INT number_of_cols = dest.length() + 1;

  std::vector<std::vector<INT>> dp(number_of_rows, std::vector<INT>(number_of_cols, (1 << 16)));

  dp[0][0] = 0;

  for (INT i = 0; i < number_of_rows; i++) {
    for (INT j = 0; j < number_of_cols; j++) {
      if ((i >= 1) && (j >= 1)) {
        if (src[i - 1] == dest[j - 1]) {
          choice_min(dp[i][j], dp[i - 1][j - 1]);
        } else {
          choice_min(dp[i][j], (dp[i - 1][j - 1] + 1));
        }
      }

      // delete from src
      if (i >= 1) {
        choice_min(dp[i][j], (dp[i - 1][j] + 1));
      }

      // insert into src from dest
      if (j >= 1) {
        choice_min(dp[i][j], (dp[i][j - 1] + 1));
      }
    }

    std::string label = std::to_string(i) + " : ";

    dump_list(dp[i], label);
  }

  return dp[number_of_rows - 1][number_of_cols - 1];
}
