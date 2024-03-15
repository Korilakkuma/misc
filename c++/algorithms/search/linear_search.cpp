#include "data.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./linear_search N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N   = std::stoll(argv[1]);
  INT key = std::stoll(argv[2]);

  std::vector<INT> list(N, 0);

  init_list(list);

  INT i = 0;

  bool found = false;

  while (i < N) {
    if (key == list[i]) {
      found = true;
      break;
    }

    ++i;
  }

  if (found) {
    std::cout << "Found (" << key << ") is index " << i << std::endl;
  } else {
    std::cout << "Not Found (" << key << ")" << std::endl;
  }

  dump_list(list);

  return 0;
}
