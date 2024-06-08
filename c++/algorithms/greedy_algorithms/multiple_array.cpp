#include "data.hpp"

static INT multiple_array(std::vector<INT> &A, std::vector<INT> &B);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./multiple_array N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> A(N);
  std::vector<INT> B(N);

  init_list(A);

  for (INT i = 0; i < A.size(); i++) {
    B[i] = A[i] + generate_random_value(3);
  }

  dump_list(A, "A: ");
  dump_list(B, "B: ");

  INT count = multiple_array(A, B);

  std::cout << std::endl;
  std::cout << "minimum count is " << count << std::endl;

  return 0;
}

static INT multiple_array(std::vector<INT> &A, std::vector<INT> &B) {
  INT N = A.size();

  INT count = 0;

  for (INT i = (N - 1); i >= 0; i--) {
    A[i] += count;

    INT remainder = A[i] % B[i];

    if (remainder == 0) {
      return count;
    }

    INT d = B[i] - remainder;

    count += d;
  }

  return count;
}
