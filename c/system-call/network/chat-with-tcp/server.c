#include "chat.h"

#define QUEUE_SIZE_FOR_LISTEN 1

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Usage: ./server port\n", stderr);
    exit(EXIT_FAILURE);
  }

  in_port_t port = (in_port_t)strtol(argv[1], NULL, 10);

  struct sockaddr_in hserver;
  int socket_for_waiting;
  int rwsocket;
  char buf[BUF_SIZE];

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

  rwsocket = accept(socket_for_waiting, NULL, NULL);

  close(socket_for_waiting);

  write(STDOUT_FILENO, "Go ahead\n", 9);

  do {
    ssize_t n;

    // Show prompt
    write(STDOUT_FILENO, SEND_PROMPT, 2);

    // Input and Send to Client
    n = read(STDIN_FILENO, buf, BUF_SIZE);
    write(rwsocket, buf, n);

    // Show prompt
    write(STDOUT_FILENO, RECEIVE_PROMPT, 2);

    // Receive from Client and Output
    n = read(rwsocket, buf, BUF_SIZE);
    write(STDOUT_FILENO, buf, n);
  } while (strncmp(buf, "quit", 4) != 0);

  close(rwsocket);

  return 0;
}
