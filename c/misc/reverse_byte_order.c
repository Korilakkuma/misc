#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long INT;

INT reverse_byte_order(const INT n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./reverse_byte_order number_string\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  fprintf(stdout, "default byte order number is 0x%016llx.\n", n);

  INT r_n = reverse_byte_order(n);

  fprintf(stdout, "reverse byte order number is 0x%016llx.\n", r_n);

  return 0;
}

INT reverse_byte_order(const INT n) {
  INT r_n;

  char *r_s = (char *)&r_n;

  const char *s = (const char *)&n;

  const size_t size = sizeof(INT);

  for (INT i = 0; i < size; i++) {
    r_s[i] = s[(size - i) - 1];
  }

  return r_n;
}
