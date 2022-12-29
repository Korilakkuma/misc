#include "socketlib.h"

int setup_multiple_server(in_port_t port, int queue_size) {
  struct sockaddr_in hserver;
  int socket_for_waiting;

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

  if (listen(socket_for_waiting, queue_size) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Listen (%d) ...\n", port);
  fflush(stdout);

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
