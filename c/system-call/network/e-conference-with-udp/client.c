#include "app_client.h"

int main(int argc, char **argv) {
  int sock;
  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((sock = setup_client(hostname, PORT)) < 0) {
    exit(EXIT_FAILURE);
  }

  app_client_init(sock);
  app_client_loop();

  return 0;
}
