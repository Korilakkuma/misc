#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int row = 10;
int col = 0;
int dir = 1;

static void set_ticker(int msec);
static void move_message(int sig);

int main(void) {
  int delay  = 200;
  int ndelay = 0;

  initscr();
  crmode();
  noecho();
  clear();

  move(row, col);
  addstr("Animating ...");

  signal(SIGALRM, move_message);

  set_ticker(delay);

  while (1) {
    ndelay = 0;

    int ch = fgetc(stdin);

    switch (ch) {
      case 'Q': {
        break;
      }

      case ' ': {
        dir = -dir;
        break;
      }

      case 'f': {
        if (delay > 2) {
          ndelay = delay / 2;
        }

        break;
      }

      case 's': {
        ndelay = 2 * delay;

        break;
      }
    }

    if (ndelay > 0) {
      set_ticker(ndelay);
    }
  }

  endwin();

  return 0;
}

static void set_ticker(int msec) {
  struct itimerval current_timer;;
  struct itimerval next_timer;

  time_t sec       = msec / 1000;
  suseconds_t usec = (msec % 1000) * 1000UL;

  next_timer.it_value.tv_sec  = sec;
  next_timer.it_value.tv_usec = usec;

  next_timer.it_interval.tv_sec  = sec;
  next_timer.it_interval.tv_usec = usec;

  getitimer(ITIMER_REAL, &current_timer);
  setitimer(ITIMER_REAL, &next_timer, &current_timer);
}

static void move_message(int sig) {
  signal(SIGALRM, move_message);

  move(row, col);
  addstr("             ");

  col += dir;

  move(row, col);
  addstr("Animating ...");
  refresh();

  if ((dir == -1) && (col <= 0)) {
    dir = 1;
  } else if ((dir == 1) && ((col + strlen("Animating ...")) >= COLS)) {
    dir = -1;
  }
}
