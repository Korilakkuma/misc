#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool check_bit(unsigned int n, unsigned int b);

int main(int argc, char** argv) {
  if (argc != 2) {
    fputs("./bit_checker number\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned int n = (unsigned int)strtoul(argv[1], NULL, 10);

  for (int b = 31; b >= 0; b--) {
    fprintf(stdout, "%d", check_bit(n, b));
  }

  fputs("\n", stdout);
}

bool check_bit(unsigned int n, unsigned int b) {
  return (n & (1U << b)) != 0;
}
