#include "app_server.h"

int main(int argc, char **argv) {
  if (app_server_init(PORT) == -1) {
    fputs("Setup server failed\n", stderr);
    exit(EXIT_FAILURE);
  }

  app_server_loop();

  return 0;
}
