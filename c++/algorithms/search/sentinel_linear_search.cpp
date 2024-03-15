#include "data.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./sentinel_linear_search N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N   = std::stoll(argv[1]);
  INT key = std::stoll(argv[2]);

  std::vector<INT> list((N + 1), 0);

  init_list(list);

  list[N] = key;

  INT i = 0;

  while (key != list[i++]) {
    ;
  }

  if (i < N) {
    std::cout << "Found (" << key << ") is index " << i << std::endl;
  } else {
    std::cout << "Not Found (" << key << ")" << std::endl;
  }

  dump_list(list);

  return 0;
}
