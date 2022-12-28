#include "app.h"
#include "network.h"

int main(int argc, char **argv) {
  int sock;

  if ((sock = setup_server(PORT)) == -1) {
    exit(EXIT_FAILURE);
  }

  app_init(sock, 'o', 1, 1, 'x', 10, 10);
  app_loop();

  return 0;
}
