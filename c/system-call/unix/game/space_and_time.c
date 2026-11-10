#include <curses.h>
#include <unistd.h>

int main(void) {
  initscr();

  clear();

  for (int n = 0; n < LINES; n++) {
    move(n, (n + n));

    if ((n % 2) == 0) {
      standout();
    }

    addstr("Animating ...");

    if ((n % 2) == 0) {
      standend();
    }

    sleep(1);

    refresh();
  }

  endwin();

  return 0;
}
