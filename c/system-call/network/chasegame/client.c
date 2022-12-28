#include "app.h"
#include "network.h"

int main(int argc, char **argv) {
  int sock;
  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((sock = setup_client(hostname, PORT)) == -1) {
    exit(EXIT_FAILURE);
  }

  app_init(sock, 'x', 10, 10, 'o', 1, 1);
  app_loop();

  return 0;
}
