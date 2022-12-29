#include "app_server.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Usage: ./server attendants\n", stderr);
    exit(EXIT_FAILURE);
  }

  int sock;
  int max_fd;

  int n = (int)strtol(argv[1], NULL, 10);

  if ((n <= 0) || (n > MAX_NUMBER_OF_ATTENDANTS)) {
    fprintf(stderr, "Attendants limit is %d\n", MAX_NUMBER_OF_ATTENDANTS);
    exit(EXIT_FAILURE);
  }

  if ((sock = setup_multiple_server(PORT, n)) == -1) {
    fputs("Setup server failed\n", stderr);
    exit(EXIT_FAILURE);
  }

  max_fd = multiple_accept(sock, n, enter);

  app_server_init(n, max_fd);
  app_server_loop();

  return 0;
}
