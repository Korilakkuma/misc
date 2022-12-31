#include "app_client.h"

static char send_buf[BUF_SIZE];
static char receive_buf[BUF_SIZE];
static char *input;
static int app_socket;

static fd_set mask;
static int number_of_fds;
static struct sockaddr_in server;
static struct sockaddr_in client;
static struct sockaddr_in from;

static size_t length_of_from;

static char name[NAME_LENGTH];

static int slot;

static WINDOW *win_send;
static WINDOW *win_receive;
static WINDOW *frame_send;
static WINDOW *frame_receive;

static int login(void);
static void logout(void);
static void die(int sig);

int setup_client(char *hostname, in_port_t port) {
  struct hostent *server_ent;

  if ((server_ent = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname");
    return -1;
  }

  memset((char *)&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port   = htons(port);

  memcpy((char *)&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

  if ((app_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  memset((char *)&client, 0, sizeof(client));

  client.sin_family      = AF_INET;
  client.sin_addr.s_addr = htonl(INADDR_ANY);
  client.sin_port        = 0;

  if (bind(app_socket, (struct sockaddr *)&client, (socklen_t)sizeof(client)) == -1) {
    perror("bind");
    return -1;
  }

  fputs("Successfully bound !\n", stdout);

  return login();
}

void app_client_init(int sock) {
  number_of_fds = app_socket + 1;

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);
  FD_SET(app_socket, &mask);

  initscr();

  signal(SIGINT, die);

  win_send   = newwin(SEND_WIN_HEIGHT, SEND_WIN_WIDTH, 19, 1);
  frame_send = newwin((SEND_WIN_HEIGHT + 2), (SEND_WIN_WIDTH + 2), 18, 0);

  box(frame_send, '|', '-');
  scrollok(win_send, true);
  wmove(win_send, 0, 0);

  win_receive   = newwin(RECEIVE_WIN_HEIGHT, RECEIVE_WIN_WIDTH, 0, 0);
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
          break;
        }

        s--;

        getyx(win_send, y, x);
        wmove(win_send, y, (x - 1));
        waddch(win_send, CLEAR);
        wmove(win_send, y, (x - 1));
      } else if ((c == '\n') || (c == '\r')) {
        input[s++] = '\n';

        sendto(app_socket, send_buf, (s + 1), 0, (struct sockaddr *)&server, (socklen_t)sizeof(server));

        wclear(win_send);

        s = 0;
      } else {
        input[s++] = c;
        waddch(win_send, c);
      }

      wrefresh(win_send);
    }

    if (FD_ISSET(app_socket, &readfds)) {
      length_of_from = sizeof(from);

      ssize_t s = recvfrom(app_socket, receive_buf, BUF_SIZE, 0, (struct sockaddr *)&from, (socklen_t *)&length_of_from);

      if ((unsigned char)receive_buf[0] == END) {
        wrefresh(win_send);
        wrefresh(win_receive);
        break;
      }

      if ((unsigned char)receive_buf[0] == DATA) {
        for (int i = 1; i < s; i++) {
          waddch(win_receive, receive_buf[i]);
        }
      }

      wrefresh(win_send);
      wrefresh(win_receive);
    }
  }

  die(0);
}

static int login(void) {
  send_buf[0] = LOGIN;

  fputs("Type your name > ", stdout);
  fgets(name, NAME_LENGTH, stdin);
  trim_newline(name, NAME_LENGTH);

  strcpy((send_buf + 1), name);

  sendto(app_socket, send_buf, (strlen(send_buf) + 1), 0, (struct sockaddr *)&server, (socklen_t)sizeof(server));

  length_of_from = sizeof(from);

  recvfrom(app_socket, receive_buf, BUF_SIZE, 0, (struct sockaddr *)&from, (socklen_t *)&length_of_from);

  if ((unsigned char)receive_buf[0] == CANNOT_LOGIN) {
    fputs("Cannot login\n", stderr);
    return -1;
  } else if ((unsigned char)receive_buf[0] == LOGIN) {
    sscanf((receive_buf + 1), "%02d", &slot);
    send_buf[0] = DATA;
    sprintf((send_buf + 1), "%-10s-> ", name);
    input = send_buf + 1;

    fputs("Logged in \n", stdout);
    return 1;
  }

  fprintf(stderr, "receive_buf[0] == %u", (unsigned char)receive_buf[0]);
  return -2;
}

static void logout(void) {
  send_buf[0] = LOGOUT;

  sprintf((send_buf + 1), "%02d\n", slot);

  sendto(app_socket, send_buf, 4, 0, (struct sockaddr *)&server, (socklen_t)sizeof(server));
}

static void die(int sig) {
  endwin();
  logout();
  close(app_socket);
  exit(EXIT_SUCCESS);
}
