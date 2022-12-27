#include "app.h"

static char app_stone;
static char app_peer_stone;

static char plane[][6] = {
  "+123+",
  "a...|",
  "b...|",
  "c...|",
  "+---+"
};

static int app_socket;

void app_init(int sock, char stone, char peer_stone) {
  app_socket     = sock;
  app_stone      = stone;
  app_peer_stone = peer_stone;
}

void app_destroy(void) {
  close(app_socket);
}

void app_show(void) {
  for (int i = 0; i < 5; i++) {
    fprintf(stdout, "%s\n", plane[i]);
  }
}

int app_peer_turn(void) {
  char data[DATA_SIZE];
  int x;
  int y;

  read(app_socket, data, DATA_SIZE);

  if (data[0] == 'q') {
    return ENDED_TURN;
  }

  x = (int)data[1] - (int)'0';
  y = (int)data[0] - (int)'a' + 1;

  plane[y][x] = app_peer_stone;

  return NOW_TURN;
}

int app_turn(void) {
  char data[DATA_SIZE];
  int x;
  int y;

  while (1) {
    fgets(data, DATA_SIZE, stdin);

    if (data[0] == 'q') {
      write(app_socket, data, 1);
      return ENDED_TURN;
    }

    int tx = data[1];
    int ty = data[0];

    if ((ty < 'a') || (ty > 'c') || (tx < '1') || (tx > '3')) {
      continue;
    }

    break;
  }

  x = (int)data[1] - (int)'0';
  y = (int)data[0] - (int)'a' + 1;

  plane[y][x] = app_stone;

  write(app_socket, data, DATA_SIZE);

  return NOW_TURN;
}
