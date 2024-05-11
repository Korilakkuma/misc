#include <iostream>
#include <vector>

using INT = long long;

enum class Result {
  FAIL,
  SUCCESS
};

enum class Status {
  USED,
  FREE
};

static const INT N = 8;

static std::vector<INT> positions(N);
static std::vector<Status> columns(N);
static std::vector<Status> downs((2 * N) - 1);
static std::vector<Status> ups((2 * N) - 1);

static void backtracking(INT row);

static void init_board();
static void print_board();

static INT number_of_solutions = 0;

int main(int argc, char **argv) {
  init_board();
  backtracking(0);

  return 0;
}

static void backtracking(INT row) {
  for (INT col = 0; col < N; col++) {
    INT down = row - col + (N - 1);
    INT up   = row + col;

    if ((columns[col] == Status::FREE) && (downs[down] == Status::FREE) && (ups[up] == Status::FREE)) {
      positions[row] = col;
      columns[col]   = Status::USED;
      downs[down]    = Status::USED;
      ups[up]        = Status::USED;

      if ((row + 1) >= N) {
        print_board();
      } else {
        backtracking(row + 1);
      }

      positions[row] = -1;
      columns[col]   = Status::FREE;
      downs[down]    = Status::FREE;
      ups[up]        = Status::FREE;
    }
  }
}

static void init_board() {
  for (INT i = 0; i < N; i++) {
    positions[i] = -1;
  }

  for (INT i = 0; i < N; i++) {
    columns[i] = Status::FREE;
  }

  for (INT i = 0; i < ((2 * N) - 1); i++) {
    downs[i] = Status::FREE;
    ups[i]   = Status::FREE;
  }
}

static void print_board() {
  std::cout << number_of_solutions++ << std::endl;

  for (INT i = 0; i < N; i++) {
    for (INT j = 0; j < N; j++) {
      if (positions[i] == j) {
        std::cout << "Q ";
      } else {
        std::cout << ". ";
      }
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
}
