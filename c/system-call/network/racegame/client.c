#include "app_client.h"

int main(int argc, char **argv) {
  int app_socket;
  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((app_socket = setup_client(hostname, PORT)) == -1) {
    exit(EXIT_FAILURE);
  }

  app_client_init(app_socket);
  app_client_loop();

  return 0;
}
