#include "chat.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Usage: ./server port\n", stderr);
    exit(EXIT_FAILURE);
  }

  in_port_t port = (in_port_t)strtol(argv[1], NULL, 10);

  struct sockaddr_in server;
  struct sockaddr_in client;
  int sock;
  char buf[BUF_SIZE];

  socklen_t len = (socklen_t)sizeof(client);

  memset((char *)&server, 0, sizeof(server));

  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port        = htons(port);

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  if (bind(sock, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    perror("bind");
    return -1;
  }

  write(STDOUT_FILENO, "Wait ...\n", 9);

  do {
    ssize_t n;

    // Show prompt
    write(STDOUT_FILENO, RECEIVE_PROMPT, 2);

    // Receive from Client and Output
    n = recvfrom(sock, buf, (size_t)BUF_SIZE, 0, (struct sockaddr *)&client, &len);
    write(STDOUT_FILENO, buf, n);

    // Show prompt
    write(STDOUT_FILENO, SEND_PROMPT, 2);

    // Input and Send to Client
    n = read(STDIN_FILENO, buf, BUF_SIZE);
    sendto(sock, buf, (size_t)n, 0, (struct sockaddr *)&client, (socklen_t)sizeof(client));
  } while (strncmp(buf, "quit", 4) != 0);

  close(sock);

  return 0;
}
