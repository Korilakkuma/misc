#include <iostream>
#include <vector>

using INT = long long;

static INT memoized_fibonacci(std::vector<INT> &memos, INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./memoized_fibonacci N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);
  std::vector<INT> memos((N + 1), -1);

  INT a_n = memoized_fibonacci(memos, N);

  std::cout << "fibonacci(" << N << "): " << a_n << std::endl;

  std::cout << std::endl;

  for (INT i = 0; i < memos.size(); i++) {
    std::cout << memos[i] << " ";
  }

  std::cout << std::endl;

  return 0;
}

static INT memoized_fibonacci(std::vector<INT> &memos, INT N) {
  if (N == 0) {
    memos[0] = 0;

    return memos[N];
  }

  if (N == 1) {
    memos[0] = 0;
    memos[1] = 1;

    return memos[N];
  }

  if (memos[N] != -1) {
    return memos[N];
  }

  memos[N] = memoized_fibonacci(memos, (N - 2)) + memoized_fibonacci(memos, (N - 1));

  return memos[N];
}
