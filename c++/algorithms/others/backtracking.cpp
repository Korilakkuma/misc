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

static Result backtracking(INT row);

static void init_board();
static void print_board();

int main(int argc, char **argv) {
  init_board();

  if (backtracking(0) == Result::SUCCESS) {
    print_board();
  } else {
    std::cerr << "No solution." << std::endl;
  }

  return 0;
}

static Result backtracking(INT row) {
  for (INT col = 0; col < N; col++) {
    std::cout << "row   : " << row << std::endl;
    std::cout << "column: " << col << std::endl;

    INT down = row - col + (N - 1);
    INT up   = row + col;

    if ((columns[col] == Status::FREE) && (downs[down] == Status::FREE) && (ups[up] == Status::FREE)) {
      positions[row] = col;
      columns[col]   = Status::USED;
      downs[down]    = Status::USED;
      ups[up]        = Status::USED;

      if ((row + 1) >= N) {
        return Result::SUCCESS;
      }

      if (backtracking(row + 1) == Result::SUCCESS) {
        return Result::SUCCESS;
      }

      positions[row] = -1;
      columns[col]   = Status::FREE;
      downs[down]    = Status::FREE;
      ups[up]        = Status::FREE;
    }
  }

  return Result::FAIL;
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
