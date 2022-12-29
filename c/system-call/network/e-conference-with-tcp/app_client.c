#include "app_client.h"

static WINDOW *win_send;
static WINDOW *win_receive;
static WINDOW *frame_send;
static WINDOW *frame_receive;

static char send_buf[BUF_SIZE];
static char receive_buf[BUF_SIZE];
static int app_socket;
static fd_set mask;
static int number_of_fds;

static void die(int sig);

void app_client_init(int sock) {
  app_socket    = sock;
  number_of_fds = sock + 1;

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);
  FD_SET(sock, &mask);

  initscr();

  signal(SIGINT, die);

  win_send   = newwin(SEND_WIN_HEIGHT, SEND_WIN_WIDTH, 19, 1);
  frame_send = newwin((SEND_WIN_HEIGHT + 2), (SEND_WIN_WIDTH + 2), 18, 0);

  box(frame_send, '|', '-');
  scrollok(win_send, true);
  wmove(win_send, 0, 0);

  win_receive   = newwin(RECEIVE_WIN_HEIGHT, RECEIVE_WIN_WIDTH, 1, 1);
  frame_receive = newwin((RECEIVE_WIN_HEIGHT + 2), (RECEIVE_WIN_WIDTH + 2), 0, 0);

  box(frame_receive, '|', '-');
  scrollok(win_receive, true);
  wmove(win_receive, 0, 0);

  cbreak();
  noecho();

  wrefresh(win_send);
  wrefresh(frame_send);
  wrefresh(win_receive);
  wrefresh(frame_receive);
}

void app_client_loop(void) {
  fd_set readfds;

  size_t s = 0;

  int x = 24;
  int y = 200;

  while (1) {
    readfds = mask;

    select(number_of_fds, (fd_set *)&readfds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      int c = fgetc(stdin);

      if ((c == '\b') || (c == 0x10) || (c == 0x7f)) {
        if (s == 0) {
          continue;
        }

        s--;

        getyx(win_send, y, x);
        wmove(win_send, y, (x - 1));
        waddch(win_send, CLEAR);
        wmove(win_send, y, (x - 1));
      } else if ((c == '\n') || (c == '\r')) {
        send_buf[s++] = '\n';

        write(app_socket, send_buf, s);

        wclear(win_send);

        s = 0;
      } else {
        send_buf[s++] = c;
        waddch(win_send, c);
      }

      wrefresh(win_send);
    }

    if (FD_ISSET(app_socket, &readfds)) {
      ssize_t s = read(app_socket, receive_buf, (size_t)BUF_SIZE);

      for (int i = 0; i < s; i++) {
        waddch(win_receive, receive_buf[i]);
      }

      wrefresh(win_receive);
    }

    if (strstr(receive_buf, "quit") != NULL) {
      wrefresh(win_send);
      wrefresh(win_receive);
      die(0);
      break;
    }
  }

  die(0);
}

static void die(int sig) {
  endwin();
  close(app_socket);
  exit(EXIT_FAILURE);
}
