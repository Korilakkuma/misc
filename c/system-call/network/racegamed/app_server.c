#include "app_server.h"

static int sockets[MAX_PLAYERS];
static char names[MAX_PLAYERS][16];
static char players[MAX_PLAYERS * PLAYER_SIZE];

static fd_set mask;
static fd_set readfds;
static int number_of_fds;

static int number_of_statges;
static int number_of_players;
static int number_of_reached;
static int number_of_crashed;

static char grades[MAX_PLAYERS * GRADE_SIZE];

static void receive_data(void);
static void send_data(void);
static void end(void);

void enter(int entry_number, int fd) {
  static char *prompt = "Type your name > ";
  char message[32];

  sockets[entry_number] = fd;

  write(sockets[entry_number], prompt, (strlen(prompt) + 1));
  read(sockets[entry_number], names[entry_number], 16);

  sprintf(message, "%d\n", entry_number);
  write(sockets[entry_number], message, (strlen(message) + 1));

  printf("[%d] %s\n", entry_number, names[entry_number]);
}

void app_server_init(int p, int s, int max_fd) {
  static char message[10];

  number_of_players = p;
  number_of_statges = s;

  number_of_fds = max_fd + 1;

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);

  for (int i = 0; i < number_of_players; i++) {
    FD_SET(sockets[i], &mask);
  }

  sprintf(message, "%d %d\n", number_of_players, number_of_statges);

  for (int i = 0; i < number_of_players; i++) {
    write(sockets[i], message, 10);
  }

  number_of_reached = 0;
  number_of_crashed = 0;
}

void app_server_loop(void) {
  while ((number_of_reached + number_of_crashed) < number_of_players) {
    receive_data();
    send_data();
  }

  end();
}

static void receive_data(void) {
  readfds = mask;

  select(number_of_fds, &readfds, NULL, NULL, NULL);

  for (int i = 0; i < number_of_players; i++) {
    if (!FD_ISSET(sockets[i], (fd_set *)&readfds)) {
      continue;
    }

    int offset = i * PLAYER_SIZE;

    read(sockets[i], &players[offset], PLAYER_SIZE);

    if (players[offset + DAMAGE] >= MAX_DAMAGE) {
      grades[((number_of_crashed - 1) * GRADE_SIZE) + ENTRY_NAME] = i;
      strcpy(&grades[((number_of_crashed - 1) * GRADE_SIZE) + ENTRY_NAME], names[i]);
      number_of_crashed++;
    }

    if (players[i + STAGE] > number_of_statges) {
      grades[(number_of_reached * GRADE_SIZE) + ENTRY_NUMBER] = (char)i;
      strcpy(&grades[(number_of_reached * GRADE_SIZE) + ENTRY_NAME], names[i]);
      number_of_reached++;
    }
  }
}

static void send_data(void) {
  for (int i = 0; i < number_of_players; i++) {
    if (FD_ISSET(sockets[i], &readfds)) {
      write(sockets[i], players, (number_of_players * PLAYER_SIZE));
    }
  }
}

static void end(void) {
  for (int i = 0; i < number_of_players; i++) {
    write(sockets[i], grades, (number_of_players * GRADE_SIZE));
  }
}
