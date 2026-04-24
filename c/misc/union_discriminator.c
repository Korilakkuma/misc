#include <stdio.h>

enum DISC {
  INT,
  DOUBLE
};

union Number {
  int i;
  double d;
};

struct NumberWithDisc {
  enum DISC disc;
  union Number num;
};

int main(int argc, char **argv) {
  struct NumberWithDisc x;

  x.num.i = 527;
  x.disc  = INT;

  fprintf(stdout, "field 'i' is %d.\n", x.num.i);

  x.num.d = 527.101;
  x.disc  = DOUBLE;

  fprintf(stdout, "field 'd' is %f.\n", x.num.d);

  return 0;
}
