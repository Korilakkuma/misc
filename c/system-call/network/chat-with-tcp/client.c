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
  int rwsocket;
  char buf[BUF_SIZE];

  if ((server_ent = gethostbyname(hostname)) == NULL) {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  memset((char *)&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port   = htons(port);  // Port number is 2 bytes

  memcpy((char *)&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

  if ((rwsocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (connect(rwsocket, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  write(STDOUT_FILENO, "Wait ...\n", 9);

  do {
    ssize_t n;

    // Show prompt
    write(STDOUT_FILENO, RECEIVE_PROMPT, 2);

    // Receive from Server and Output
    n = read(rwsocket, buf, BUF_SIZE);
    write(STDOUT_FILENO, buf, n);

    // Show prompt
    write(STDOUT_FILENO, SEND_PROMPT, 2);

    // Input and Send to Server
    n = read(STDIN_FILENO, buf, BUF_SIZE);
    write(rwsocket, buf, n);
  } while (strncmp(buf, "quit", 4) != 0);

  close(rwsocket);

  return 0;
}
