#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

enum {
  LEFT,
  CENTER,
  RIGHT
};

int main(int argc, char **argv) {
  long long a[2];
  long long b[2];
  long long c[2];

  srand((unsigned int)time(NULL));

  for (short i = 0; i < 2; i++) {
    a[i] = (long long)(10 * ((double)rand() / RAND_MAX));
    b[i] = (long long)(10 * ((double)rand() / RAND_MAX));
    c[i] = b[i] + (long long)(10 * ((double)rand() / RAND_MAX));
  }

  long long ba[] = { a[0] - b[0], a[1] - b[1] };
  long long bc[] = { c[0] - b[0], c[1] - b[1] };
  long long ca[] = { a[0] - c[0], a[1] - c[1] };
  long long cb[] = { a[0] - c[0], b[1] - c[1] };

  long long innerB = (ba[0] * bc[0]) + (ba[1] * bc[1]);  // or |ba| * |bc| * cos
  long long innerC = (ca[0] * cb[0]) + (ca[1] * cb[1]);  // or |ca| * |cb| * cos

  int pattern = CENTER;

  if ((innerB <= 0) && (innerC >= 0)) {
    pattern = LEFT;
  } else if ((innerB >= 0) && (innerC <= 0)) {
    pattern = RIGHT;
  }

  double ah = 0.0;
  double d  = 0.0;

  long long cross = 0LL;

  switch (pattern) {
    case LEFT:
      ah = sqrt(powl((a[0] - b[0]), 2) + powl((a[1] - b[1]), 2));
      break;
    case RIGHT:
      ah = sqrt(powl((a[0] - c[0]), 2) + powl((a[1] - c[1]), 2));
      break;
    case CENTER:
      cross = (long long)abs((int)((ba[0] * bc[1]) - (ba[1] * bc[0])));  // or |ba| * |bc| * |sin|
      d     = sqrt(powl((c[0] - b[0]), 2) + powl((c[1] - b[1]), 2));
      ah    = cross / d;

      break;
    default:
      break;
  }

  printf("A (%lld, %lld)\n", a[0], a[1]);
  printf("B (%lld, %lld)\n", b[0], b[1]);
  printf("C (%lld, %lld)\n", c[0], c[1]);
  printf("innerB = %lld\n", innerB);
  printf("innerC = %lld\n", innerC);

  if (pattern == CENTER) {
    printf("cross = %lld\n", cross);
  }

  printf("AH = %f\n", ah);

  return 0;
}
