#include "socketlib.h"

int setup_multiple_server(in_port_t port, int queue_size) {
  struct addrinfo hints;
  struct addrinfo *res;
  int socket_for_waiting;

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

  listen(socket_for_waiting, queue_size);

  fprintf(stdout, "Listen (%d) ...\n", port);
  fflush(stdout);

  freeaddrinfo(res);

  return socket_for_waiting;
}

int multiple_accept(int socket_for_waiting, int limit, void (*callback)(int n, int sock)) {
  int sock;

  for (int i = 0; i < limit; i++) {
    if ((sock = accept(socket_for_waiting, NULL, NULL)) == -1) {
      perror("accept");
      return -1;
    }

    (*callback)(i, sock);
  }

  close(socket_for_waiting);

  return sock;
}
