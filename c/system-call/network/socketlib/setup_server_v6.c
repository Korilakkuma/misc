#include "socketlib.h"

#define QUEUE_SIZE_FOR_LISTEN 1

int setup_server(in_port_t port) {
  struct addrinfo hints;
  struct addrinfo *res;
  int socket_for_waiting;
  int sock;

  char portstring[8];
  char hostname[HOSTNAME_LENGTH];

  gethostname(hostname, HOSTNAME_LENGTH);
  sprintf(portstring, "%d", port);
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname, portstring, &hints, &res) != 0) {
    perror("getaddrinfo");
    return -1;
  }

  if ((socket_for_waiting = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_for_waiting, res->ai_addr, res->ai_addrlen) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  listen(socket_for_waiting, QUEUE_SIZE_FOR_LISTEN);

  fprintf(stdout, "Listen (%d) ...\n", port);
  fflush(stdout);

  sock = accept(socket_for_waiting, NULL, NULL);

  close(socket_for_waiting);
  freeaddrinfo(res);

  return sock;
}
