#include "app.h"

int main(int argc, char **argv) {
  int sock;

  char stone      = 'x';
  char peer_stone = 'o';

  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((sock = setup_client(hostname, PORT)) == -1) {
    exit(EXIT_FAILURE);
  }

  app_init(sock, stone, peer_stone);

  while (1)  {
    app_show();

    fputs("Go ahead !\n", stdout);
    write(STDOUT_FILENO, PROMPT, 2);

    if (app_turn() == ENDED_TURN) {
      break;
    }

    app_show();

    fputs("Wait ...\n", stdout);

    if (app_peer_turn() == ENDED_TURN) {
      break;
    }
  }

  app_destroy();

  return 0;
}
