#include "app_server.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Usage: ./server players stages\n", stderr);
    exit(EXIT_FAILURE);
  }

  int app_socket;

  int nplayers = (int)strtol(argv[1], NULL, 10);
  int nstages  = (int)strtol(argv[2], NULL, 10);

  if ((nplayers <= 0) || (nplayers > MAX_PLAYERS)) {
    fprintf(stderr, "Players limit = %d\n", MAX_PLAYERS);
    exit(EXIT_FAILURE);
  }

  if ((nstages <= 0) || (nstages > MAX_STAGES)) {
    fprintf(stderr, "Stages limit = %d\n", MAX_STAGES);
    exit(EXIT_FAILURE);
  }

  if ((app_socket = setup_multiple_server(PORT, nplayers)) == -1) {
    fputs("Setup server failed\n", stderr);
    exit(EXIT_FAILURE);
  }

  int max_fd = multiple_accept(app_socket, nplayers, enter);

  app_server_init(nplayers, nstages, max_fd);
  app_server_loop();

  return 0;
}
