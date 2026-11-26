#include <curses.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#define TICKS_PER_SEC 50

struct Ball {
  int y_pos;
  int x_pos;

  int y_dir;
  int x_dir;

  int y_ticks;
  int x_ticks;

  int y_interval_ticks;
  int x_interval_ticks;

  char symbol;
};

static void setup(void);
static void wrapup(void);

static int bounce_or_lose(struct Ball *ball);
static void move_ball(int sig);

static void set_ticker(int msec);

struct Ball ball;

int main(void) {
  int ch;

  setup();

  while ((ch = fgetc(stdin)) != 'Q') {
    switch (ch) {
      case 'f': {
        --ball.x_interval_ticks;
        break;
      }

      case 's': {
        ++ball.x_interval_ticks;
        break;
      }

      case 'F': {
        --ball.y_interval_ticks;
        break;
      }

      case 'S': {
        ++ball.y_interval_ticks;
        break;
      }
    }
  }

  wrapup();

  return 0;
}

static void setup(void) {
  ball.y_pos = 10;
  ball.x_pos = 10;

  ball.y_dir = 1;
  ball.x_dir = 1;

  ball.y_ticks = 8;
  ball.x_ticks = 5;

  ball.y_interval_ticks = 8;
  ball.x_interval_ticks = 5;

  ball.symbol = 'O';

  initscr();
  noecho();
  crmode();

  // signal(SIGINT, SIG_IGN);

  mvaddch(ball.y_pos, ball.x_pos, ball.symbol);
  refresh();

  signal(SIGALRM, move_ball);

  set_ticker(1000 / TICKS_PER_SEC);
}

static void wrapup(void) {
  set_ticker(0);
  endwin();
}

static int bounce_or_lose(struct Ball *ball) {
  int return_val = 0;

  if (ball->y_pos == 5) {
    ball->y_dir = 1;

    return_val = 1;
  }

  if (ball->y_pos == 20) {
    ball->y_dir = -1;

    return_val = -1;
  }

  if (ball->x_pos == 10) {
    ball->x_dir = 1;

    return_val = 1;
  }

  if (ball->x_pos == 70) {
    ball->x_dir = -1;

    return_val = -1;
  }

  return return_val;
}

static void move_ball(int sig) {
  int current_y = ball.y_pos;
  int current_x = ball.x_pos;

  int moved = 0;

  signal(SIGALRM, SIG_IGN);

  if ((ball.y_interval_ticks > 0) && (ball.y_ticks-- == 1)) {
    ball.y_pos  += ball.y_dir;
    ball.y_ticks = ball.y_interval_ticks;

    moved = 1;
  }

  if ((ball.x_interval_ticks > 0) && (ball.x_ticks-- == 1)) {
    ball.x_pos  += ball.x_dir;
    ball.x_ticks = ball.x_interval_ticks;

    moved = 1;
  }

  if (moved) {
    mvaddch(current_y, current_x, ' ');
    mvaddch(ball.y_pos, ball.x_pos, ball.symbol);

    bounce_or_lose(&ball);

    move((LINES - 1), (COLS - 1));
    refresh();
  }

  signal(SIGALRM, move_ball);
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
