#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROW 10

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./animation dir\n", stderr);
    exit(EXIT_FAILURE);
  }

  int dir = (int)strtol(argv[1], NULL, 10);

  int pos = 10;

  initscr();

  clear();

  while (1) {
    move(ROW, pos);
    addstr("Animating ...");
    move((LINES - 1), (COLS - 1));
    refresh();

    sleep(1);

    move(ROW, pos);
    addstr("             ");

    pos += dir;

    if (pos >= 30) {
      dir *= -1;
    }

    if (pos <= 10) {
      dir *= -1;
    }
  }

  endwin();

  return 0;
}
