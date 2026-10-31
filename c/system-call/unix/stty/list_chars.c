#include <stdio.h>
int main(void) {
  int ch;

  int n = 0;

  while ((ch = fgetc(stdin)) != 'Q') {
    fprintf(stdout, "char %3d is %c code %d\n", n++, ch, ch);
  }

  return 0;
}
