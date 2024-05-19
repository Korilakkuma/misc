#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using INT = long long;

INT generate_random_value(INT max) {
  return (std::rand() * max) / (RAND_MAX + 1.0);
}

void init_list(std::vector<INT> &list, INT value = 10) {
  srand((unsigned int)time(NULL));

  for (auto& l : list) {
    l = generate_random_value(value);
  }
}

void dump_list(std::vector<INT> &list, std::string label = "") {
  std::cout << label;

  for (auto& l : list) {
    std::cout << l << " ";
  }

  std::cout << std::endl;
}
