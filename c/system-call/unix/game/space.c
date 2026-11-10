#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./space row col\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long row = strtoul(argv[1], NULL, 10);
  unsigned long col = strtoul(argv[2], NULL, 10);

  initscr();

  clear();

  move(row, col);
  addstr("Space World");
  move((LINES - 1), 0);

  refresh();
  getch();

  endwin();

  return 0;
}
