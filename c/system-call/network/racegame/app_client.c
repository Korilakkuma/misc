#include "app_client.h"

static char players[MAX_PLAYERS * PLAYER_SIZE];
static char grades[MAX_PLAYERS * GRADE_SIZE];
static char tmp[PLAYER_SIZE];

static int number_of_stages;
static int number_of_players;

static int app_socket;
static int entry_number;

static bool final_status;

static char *self;

static WINDOW *win1;
static WINDOW *win2;

static bool get_location(void);
static void show_all_locations(void);
static void delete_all_locations(void);
static void send_data(void);
static void receive_all(void);
static bool new_stage(int z);

static void init_data(void);
static bool check(int dx, int dy);
static void end(int end_status);

static void die(int sig);

void app_client_init(int sock) {
  char buf[BUF_SIZE];

  app_socket = sock;

  read(app_socket, buf, BUF_SIZE);
  fputs(buf, stdout);
  fgets(buf, BUF_SIZE, stdin);
  trim_newline(buf, BUF_SIZE);

  write(app_socket, buf, (strlen(buf) + 1));
  read(app_socket, buf, BUF_SIZE);
  sscanf(buf, "%d", &entry_number);
  printf("Your entry number is %d\n", entry_number);
  read(app_socket, buf, BUF_SIZE);
  sscanf(buf, "%d %d", &number_of_players, &number_of_stages);
  printf("The number of players are %d\n", number_of_players);
  printf("Final statge is %d\n", number_of_stages);

  signal(SIGINT,  die);
  signal(SIGQUIT, die);
  signal(SIGSEGV, die);
  signal(SIGPIPE, die);
  signal(SIGTERM, die);

  initscr();
  noecho();
  crmode();
  nonl();

  win1 = newwin(22, 12, 1, 10);
  win2 = newwin(11, 25, 5, 40);

  box(win1, '|', '-');
  box(win2, '|', '-');

  new_stage(1);

  init_data();

  mvwprintw(win2, 2, 1, "Entry Number = %d", entry_number);
  mvwprintw(win2, 3, 2, "entry stage damage");
  mvwprintw(win2, (entry_number + 4), 1, "*");

  show_all_locations();
}

void app_client_loop(void) {
  while (get_location()) {
    send_data();
    delete_all_locations();
    receive_all();
    show_all_locations();
  }

  end(final_status);
  die(0);
}

static bool get_location(void) {
  int c = fgetc(stdin);

  switch (c) {
    case UP:
      if (tmp[POS_Y] == (char)1) {
        tmp[STAGE]++;
        tmp[POS_Y] = (char)21;

        if (!new_stage((int)tmp[STAGE])) {
          final_status = 0;
          return false;
        }
      } else if (check(0, -1)) {
        tmp[POS_Y]--;
      } else {
        tmp[DAMAGE]++;
      }

      break;
    case DOWN:
      if (check(0, 1)) {
        tmp[POS_Y]++;
      } else {
        tmp[DAMAGE]++;
      }

      break;
    case LEFT:
      if (check(-1, 0)) {
        tmp[POS_X]--;
      } else {
        tmp[DAMAGE]++;
      }

      break;
    case RIGHT:
      if (check(1, 0)) {
        tmp[POS_X]++;
      } else {
        tmp[DAMAGE]++;
      }

      break;
    default:
      break;
  }

  if (tmp[DAMAGE] >= MAX_DAMAGE) {
    final_status = -1;
    return false;
  }

  return true;
}

static void show_all_locations(void) {
  for (int i = 0; i < number_of_players; i++) {
    int offset = i * PLAYER_SIZE;

    if (players[offset + STAGE] == self[STAGE]) {
      mvwaddch(win1, players[offset + POS_Y], players[offset + POS_X], (char)(i + 0x30));
    }
  }

  touchwin(win1);
  wrefresh(win1);

  mvwprintw(win2, 9, 2, "stage = %2d /%2d", (int)self[STAGE], number_of_stages);

  for (int i = 0; i < number_of_players; i++) {
    mvwprintw(win2, (i + 4), 2, " %2d    %2d    %2d ", i, (int)players[(i * PLAYER_SIZE) + STAGE], (int)players[(i * PLAYER_SIZE) + DAMAGE]);
  }

  wrefresh(win2);
}

static void delete_all_locations(void) {
  for (int i = 0; i < number_of_players; i++) {
    int offset = i * PLAYER_SIZE;

    if (players[offset + STAGE] == self[STAGE]) {
      mvwaddch(win1, players[offset + POS_Y], players[offset + POS_X], CLEAR);
    }
  }

  touchwin(win1);
  wrefresh(win1);
}

static void send_data(void) {
  write(app_socket, &tmp, PLAYER_SIZE);
}

static void receive_all(void) {
  read(app_socket, players, (number_of_players * PLAYER_SIZE));
}

static bool new_stage(int z) {
  FILE *fp;

  char filename[24];

  static char buf[16];

  if (z > number_of_stages) {
    return false;
  }

  sprintf(filename, "racedata/data_0%d.txt", z);

  fp = fopen(filename, "r");

  if (fp == NULL) {
    perror("fopen");
    return false;
  }

  for (int i = 0; i <= STAGE_HEIGHT; i++) {
    fgets(buf, 24, fp);

    buf[15] = '\0';

    wmove(win1, i, 1);
    waddstr(win1, buf);
  }

  fclose(fp);

  return true;
}

static void init_data(void) {
  self = players + (entry_number * PLAYER_SIZE);

  for (int i = 0; i < number_of_players; i++) {
    int offset = i * PLAYER_SIZE;

    players[offset + STAGE]  = (char)1;
    players[offset + POS_X]  = (char)(i + 2);
    players[offset + POS_Y]  = (char)20;
    players[offset + DAMAGE] = (char)0;
  }

  memcpy(tmp, self, PLAYER_SIZE);
}

static bool check(int dx, int dy) {
  int newx = tmp[POS_X] + dx;
  int newy = tmp[POS_Y] + dy;

  if ((newx < 1) || (newx > STAGE_WIDTH)) {
    return false;
  }

  if ((newy < 1) || (newy > (STAGE_HEIGHT + 1))) {
    return false;
  }

  if ((A_CHARTEXT & mvwinch(win1, newy, newx)) == '*') {
    return false;
  }

  return true;
}

static void end(int end_status) {
  wclear(win2);
  box(win2, '|', '-');

  if (end_status == 0) {
    mvwprintw(win2, 1, 3, "Goal in ! Wait ...");
  } else {
    mvwprintw(win2, 1, 3, "Crashed ! Wait ...");
  }

  wrefresh(win2);

  read(app_socket, grades, (GRADE_SIZE * number_of_players));

  for (int i = 0; i < number_of_players; i++) {
    mvwprintw(win2, (i + 3), 1, "[%d] %d: %s", (i + 1), (int)grades[(i * GRADE_SIZE) + ENTRY_NUMBER], &grades[(i * GRADE_SIZE) + ENTRY_NAME]);
  }

  wrefresh(win2);
  fgetc(stdin);
}

static void die(int sig) {
  endwin();
  echo();
  exit(EXIT_FAILURE);
}
