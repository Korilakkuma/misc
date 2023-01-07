#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "socketlib.h"
#include "app.h"

#define SERVICE  "experiment"
#define PROTOCOL "tcp"

extern void session(int sock);

static int setup(char *hostname);

int main(int argc, char **argv) {
  int sock = -1;

  char hostname[HOSTNAME_LENGTH];

  fputs("Type hostname > ", stdout);
  fgets(hostname, HOSTNAME_LENGTH, stdin);
  trim_newline(hostname, HOSTNAME_LENGTH);

  if ((sock = setup(hostname)) < 0) {
    exit(EXIT_FAILURE);
  }

  session(sock);

  return 0;
}

static int setup(char *hostname) {
  int sock;

  struct servent *service;
  struct hostent *host;
  struct sockaddr_in server;

  if ((service = getservbyname(SERVICE, PROTOCOL)) == NULL) {
    perror("getservbyname");
    exit(EXIT_FAILURE);
  }

  if ((host = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  memset((char *)&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port   = service->s_port;

  memcpy((char *)&server.sin_addr, host->h_addr_list[0], host->h_length);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  fputs("connected.\n", stdout);
  fflush(stdout);

  return sock;
}
