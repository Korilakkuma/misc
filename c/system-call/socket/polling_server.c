#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_CLIENT = 5;

int clients[MAX_CLIENT];

int number_of_clients = 0;

void talk(int client_socket);

int main(void) {
  int s;
  int soval;

  struct sockaddr_in sa;

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

  sa.sin_len         = sizeof(sa);
  sa.sin_family      = AF_INET;
  sa.sin_port        = htons(3000);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(s, 5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  int maxfd;

  while (1) {
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(s, &readfds);

    maxfd = s;

    for (int n = 0; n < number_of_clients; n++) {
      FD_SET(clients[n], &readfds);

      if (clients[n] > maxfd) {
        maxfd = clients[n];
      }
    }

    fprintf(stdout, "Polling ... (maxfd is %d)\n", maxfd);

    if (select((maxfd + 1), &readfds, NULL, NULL, NULL) < 0) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    if (FD_ISSET(s, &readfds)) {
      int ws;

      struct sockaddr_in ca;

      socklen_t ca_len = sizeof(ca);

      if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
        perror("accept");
        continue;
      }

      if (number_of_clients >= MAX_CLIENT) {
        if (shutdown(ws, SHUT_RDWR) == -1) {
          perror("shutdown");
          exit(EXIT_FAILURE);
        }

        if (close(ws) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      } else {
        clients[number_of_clients] = ws;

        ++number_of_clients;

        fprintf(stdout, "Accepted connection on descriptor %d\n", ws);
      }

      for (int n = 0; n < number_of_clients; n++) {
        if (FD_ISSET(clients[n], &readfds)) {
          talk(clients[n]);
          break;
        }
      }
    }
  }

  return 0;
}

void talk(int client_socket) {
  fputs("Talking ...\n", stdout);

  char ch;

  ssize_t read_byte;

  do {
    if ((read_byte = read(client_socket, &ch, 1)) == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (read_byte == 0) {
      shutdown(client_socket, SHUT_RDWR);

      if (close(client_socket) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }

      for (int n = 0; n < number_of_clients; n++) {
        if (clients[n] == client_socket) {
          clients[n] = clients[number_of_clients - 1];

          --number_of_clients;

          break;
        }
      }

      fprintf(stdout, "Connection closed on descriptor %d.\n", client_socket);
    } else {
      for (int n = 0; n < number_of_clients; n++) {
        if (write(clients[n], &ch, 1) != 1) {
          perror("write");
          exit(EXIT_FAILURE);
        }
      }
    }
  } while (ch != '\n');
}
