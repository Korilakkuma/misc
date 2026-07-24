#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int s;
  int ws;
  int soval;

  ssize_t written_byte;

  char buf[1024];

  struct sockaddr_in sa;
  struct sockaddr_in ca;

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  soval = 1;

  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof(sa));

  sa.sin_len = sizeof(sa);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(3000);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(s, 5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    socklen_t ca_len = sizeof(ca);

    fputs("Waiting for connection ...\n", stdout);

    if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
      perror("accept");
    }

    fputs("Connection Established !\n", stdout);

    fputs("Type messaeg > ", stdout);

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
      if ((written_byte = write(ws, buf, strlen(buf))) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }

      fputs("Type messaeg > ", stdout);
    }

    clearerr(stdin);

    if (shutdown(ws, SHUT_RDWR) == -1) {
      perror("shutdown");
      exit(EXIT_FAILURE);
    }

    if (close(ws) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
