#include "chat.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("Usage: ./client port hostname\n", stderr);
    exit(EXIT_FAILURE);
  }

  in_port_t port = (in_port_t)strtol(argv[1], NULL, 10);
  char *hostname = argv[2];

  struct hostent *server_ent;
  struct sockaddr_in server;
  int sock;
  char buf[BUF_SIZE];

  socklen_t len = (socklen_t)sizeof(server);

  if ((server_ent = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  memset((char *)&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port   = htons(port);

  memcpy((char *)&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  write(STDOUT_FILENO, "Go ahead\n", 9);

  do {
    ssize_t n;

    // Show prompt
    write(STDOUT_FILENO, SEND_PROMPT, 2);

    // Input and Send to Server
    n = read(STDIN_FILENO, buf, BUF_SIZE);
    sendto(sock, buf, (size_t)n, 0, (struct sockaddr *)&server, (socklen_t)sizeof(server));

    // Show prompt
    write(STDOUT_FILENO, RECEIVE_PROMPT, 2);

    // Receive from Server and Output
    n = recvfrom(sock, buf, (size_t)BUF_SIZE, 0, (struct sockaddr *)&server, &len);
    write(STDOUT_FILENO, buf, n);
  } while (strncmp(buf, "quit", 4) != 0);

  close(sock);

  return 0;
}
