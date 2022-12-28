#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <curses.h>
#include <signal.h>

#define BUF_SIZE 20
#define MIN_X    1
#define MIN_Y    1
#define MAX_X    60
#define MAX_Y    20

#define UP    'k'
#define DOWN  'j'
#define LEFT  'h'
#define RIGHT 'l'
#define QUIT  'q'
#define CLEAR ' '

static int app_socket;
static fd_set mask;
static int number_of_fds;

typedef struct {
  int x;
  int y;
  char look;
} PLAYER;

static PLAYER host;
static PLAYER peer;

static char buf[BUF_SIZE];

static WINDOW *win;

static void show(PLAYER *player);
static void hide(PLAYER *player);
static bool update(PLAYER *player, int ch);
static bool interpret(PLAYER *player);
static void die(int sig);

void app_init(int sock, char hc, int hx, int hy, int pc, int px, int py) {
  app_socket    = sock;
  number_of_fds = sock + 1;

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);
  FD_SET(app_socket, &mask);

  host.look = hc;
  host.x    = hx;
  host.y    = hy;

  peer.look = pc;
  peer.x    = px;
  peer.y    = py;

  initscr();
  signal(SIGINT, die);

  win = newwin((MAX_Y + 2), (MAX_X + 2), 0, 0);
  box(win, '|', '-');

  cbreak();
  noecho();
}

void app_loop(void) {
  int ch;
  bool f;
  fd_set readfds;

  show(&host);

  f = true;

  while (f) {
    readfds = mask;

    select(number_of_fds, (fd_set *)&readfds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      ch = fgetc(stdin);

      hide(&host);

      f = update(&host, ch);

      show(&host);

      write(app_socket, buf, BUF_SIZE);

      if (f == false) {
        break;
      }
    }

    if (FD_ISSET(app_socket, &readfds)) {
      read(app_socket, buf, BUF_SIZE);

      hide(&peer);

      f = interpret(&peer);

      show(&peer);

      if (f == false) {
        break;
      }
    }
  }

  die(0);
}

static void show(PLAYER *player) {
  wmove(win, player->y, player->x);
  waddch(win, player->look);
  wmove(win, player->y, player->x);
  wrefresh(win);
}

static void hide(PLAYER *player) {
  wmove(win, player->y, player->x);
  waddch(win, CLEAR);
}

static bool update(PLAYER *player, int ch) {
  if (ch == QUIT) {
    buf[0] = QUIT;
    return false;
  }

  switch (ch) {
    case UP:
      if (player->y > MIN_Y) {
        player->y--;
      }

      break;

    case DOWN:
      if (player->y < MAX_Y) {
        player->y++;
      }

      break;
    case LEFT:
      if (player->x > MIN_X) {
        player->x--;
      }

      break;

    case RIGHT:
      if (player->x < MAX_X) {
        player->x++;
      }

      break;
    default:
      break;
  }

  sprintf(buf, "%d %d\n", player->x, player->y);

  return true;
}

static bool interpret(PLAYER *player) {
  if (buf[0] == QUIT) {
    return false;
  }

  sscanf(buf, "%d %d", &player->x, &player->y);

  return true;
}

static void die(int sig) {
  endwin();
  exit(EXIT_SUCCESS);
}
