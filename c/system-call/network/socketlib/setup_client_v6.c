#include "socketlib.h"
#include <stdio.h>

int setup_client(char *hostname, in_port_t port) {
  struct addrinfo hints;
  struct addrinfo *res;
  int sock;

  char portstring[8];

  sprintf(portstring, "%d", port);
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname, portstring, &hints, &res) != 0) {
    perror("getaddrinfo");
    return -1;
  }

  if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  fputs("connected.\n", stdout);
  fflush(stdout);

  freeaddrinfo(res);

  return sock;
}
