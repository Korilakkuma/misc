#include <curses.h>
#include <stdio.h>
#include <term.h>

int main(void) {
  setupterm(NULL, 0, NULL);

  fprintf(stdout, "lines = %d, cols = %d\n", tigetnum("lines"), tigetnum("cols"));

  return 0;
}
