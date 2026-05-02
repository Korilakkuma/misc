#include <stdio.h>
#include <stdlib.h>

typedef unsigned long PTR;

PTR roundup(PTR p, PTR n);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./roundup [2 pow number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  // 1 byte
  unsigned char v = 10;

  unsigned char *p = &v;

  PTR alignment = (PTR)strtoul(argv[1], NULL, 10);

  fprintf(stdout, "+0 round up 0x%lx\n", roundup((PTR)(p + 0U), alignment));
  fprintf(stdout, "+1 round up 0x%lx\n", roundup((PTR)(p + 1U), alignment));
  fprintf(stdout, "+2 round up 0x%lx\n", roundup((PTR)(p + 2U), alignment));
  fprintf(stdout, "+3 round up 0x%lx\n", roundup((PTR)(p + 3U), alignment));
  fprintf(stdout, "+4 round up 0x%lx\n", roundup((PTR)(p + 4U), alignment));
  fprintf(stdout, "+5 round up 0x%lx\n", roundup((PTR)(p + 5U), alignment));
  fprintf(stdout, "+6 round up 0x%lx\n", roundup((PTR)(p + 6U), alignment));
  fprintf(stdout, "+7 round up 0x%lx\n", roundup((PTR)(p + 7U), alignment));
  fprintf(stdout, "+8 round up 0x%lx\n", roundup((PTR)(p + 8U), alignment));

  return 0;
}

PTR roundup(PTR p, PTR alignment) {
  PTR fraction = alignment - 1;

  return (p + fraction) & ~fraction;
}
