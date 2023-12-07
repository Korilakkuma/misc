#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VirtualMachine.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./arithmetic_operations_assembler source dest\n", stderr);
    exit(EXIT_FAILURE);
  }

  int i;
  char buf[80];

  FILE *fin = fopen(argv[1], "r");
  FILE *fout = fopen(argv[2], "w");

  while (fgets(buf, sizeof(buf), fin) != NULL) {
    if (strncmp(buf, "PUSH", 4) == 0) {
      int value = 0;

      i = 5;  // Next index from 'P', 'U', 'S', 'H' '\0'

      while ((buf[i] >= '0') && (buf[i] <= '9')) {
        value = (value * 10) + (buf[i++] - '0');
      }

      fputc(PUSH, fout);
      fputc(value, fout);
    } else {
      if (strncmp(buf, "ADD", 3) == 0) { fputc(ADD, fout); }
      if (strncmp(buf, "SUB", 3) == 0) { fputc(SUB, fout); }
      if (strncmp(buf, "MUL", 3) == 0) { fputc(MUL, fout); }
      if (strncmp(buf, "DIV", 3) == 0) { fputc(DIV, fout); }
    }
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
