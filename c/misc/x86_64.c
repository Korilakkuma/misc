#include <stdio.h>

typedef struct {
  unsigned int a1;
  unsigned int ah;
} H_Registers;

typedef union {
  H_Registers h;
  unsigned long ax;
} Registers;

int main(int argc, char **argv) {
  Registers r;

  r.h.ah = 0x1A1A1A1A;
  r.h.a1 = 0x2B2B2B2B;

  printf("ah = %X\n", r.h.ah);
  printf("a1 = %X\n", r.h.a1);
  printf("ax = %lX\n", r.ax);

  return 0;
}
