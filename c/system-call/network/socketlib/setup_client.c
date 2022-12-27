#include "socketlib.h"

int setup_client(char *hostname, in_port_t port) {
  struct hostent *server_ent;
  struct sockaddr_in server;
  int sock;

  if ((server_ent = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  memset((char *)&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port   = htons(port);  // Port number is 2 bytes

  memcpy((char *)&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  fputs("connected.\n", stdout);
  fflush(stdout);

  return sock;
}
