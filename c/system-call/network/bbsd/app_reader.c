#include "socketlib.h"
#include "app.h"

extern void session(int sock);

int main(int argc, char **argv) {
  int sock = -1;

  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((sock = setup_client(hostname, PORT)) < 0) {
    exit(EXIT_FAILURE);
  }

  session(sock);

  return 0;
}
