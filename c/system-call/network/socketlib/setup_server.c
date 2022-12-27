#include "socketlib.h"

#define QUEUE_SIZE_FOR_LISTEN 1

int setup_server(in_port_t port) {
  struct sockaddr_in hserver;
  int socket_for_waiting;
  int sock;

  memset((char *)&hserver, 0, sizeof(hserver));

  hserver.sin_family      = AF_INET;
  hserver.sin_addr.s_addr = htonl(INADDR_ANY);  // IPv4 32 bits
  hserver.sin_port        = htons(port);        // Port number is 2 bytes

  if ((socket_for_waiting = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_for_waiting, (struct sockaddr *)&hserver, (socklen_t)sizeof(hserver)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  listen(socket_for_waiting, QUEUE_SIZE_FOR_LISTEN);

  fprintf(stdout, "Listen (%d) ...\n", port);
  fflush(stdout);

  sock = accept(socket_for_waiting, NULL, NULL);

  close(socket_for_waiting);

  return sock;
}
