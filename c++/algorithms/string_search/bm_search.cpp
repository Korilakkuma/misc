#include <iostream>

using INT = long long;

static INT bm_search(std::string text, std::string pattern);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./bm_search text pattern" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string text    = std::string(argv[1]);
  std::string pattern = std::string(argv[2]);

  INT index = bm_search(text, pattern);

  if (index == -1) {
    std::cout << "Not Found (\"" << pattern << "\")" << std::endl;
  } else {
    std::cout << "Found (\"" << pattern << "\") is from " << index << std::endl;
  }

  return 0;
}

static INT bm_search(std::string text, std::string pattern) {
  INT skips[256];

  for (INT i = 0; i < 256; i++) {
    skips[i] = pattern.length();
  }

  for (INT i = 0, len = pattern.length(); i < len; i++) {
    skips[static_cast<unsigned char>(pattern[i])] = len - i - 1;
  }

  INT text_tail = pattern.length() - 1;

  while (text_tail < text.length()) {
    INT start_index  = text_tail;
    INT pattern_tail = pattern.length() - 1;

    while (text[text_tail] == pattern[pattern_tail]) {
      if (pattern_tail == 0) {
        return text_tail;
      }

      --text_tail;
      --pattern_tail;
    }

    text_tail = start_index + skips[static_cast<unsigned char>(text[text_tail])];

    std::cout << "Text pointer skips to " << text_tail << std::endl;
  }

  return -1;
}
