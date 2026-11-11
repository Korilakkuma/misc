#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./animation row col\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long row = strtoul(argv[1], NULL, 10);
  unsigned long col = strtoul(argv[2], NULL, 10);

  initscr();

  clear();

  int d = 0;

  while (1) {
    d = (d == 0) ? 1 : 0;

    move((row + d), col);

    addstr("Animating ...");

    sleep(1);

    refresh();

    move((row + d), col);
    addstr("             ");
  }

  endwin();

  return 0;
}
