#include <iostream>

using INT = long long;

static INT brute_force_search(std::string text, std::string pattern);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./brute_force_search text pattern" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string text    = std::string(argv[1]);
  std::string pattern = std::string(argv[2]);

  INT index = brute_force_search(text, pattern);

  if (index == -1) {
    std::cout << "Not Found (\"" << pattern << "\")" << std::endl;
  } else {
    std::cout << "Found (\"" << pattern << "\") is from " << index << std::endl;
  }

  return 0;
}

static INT brute_force_search(std::string text, std::string pattern) {
  INT t = 0;
  INT p = 0;

  INT start_index = 0;

  while ((t < text.length()) && (p < pattern.length())) {
    if (text[t] == pattern[p]) {
      ++t;
      ++p;
    } else {
      t = ++start_index;  // or t = t + 1 - p; start_index = t - p;
      p = 0;
    }
  }

  return (p == pattern.length()) ? start_index : -1;  // or (t - p) : -1;
}
