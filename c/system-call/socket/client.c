#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

  if (argc != 3) {
    fputs("./client ip port\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *ip = argv[1];
  unsigned short port = (unsigned short)strtoul(argv[2], NULL, 10);

  int s;
  ssize_t read_byte;
  char buf[1024];

  struct sockaddr_in sa;

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof(sa));

  sa.sin_len         = sizeof(sa);
  sa.sin_family      = AF_INET;
  sa.sin_port        = htons(port);
  sa.sin_addr.s_addr = inet_addr(ip);

  fputs("Connecting to Server ...\n", stdout);

  if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  fputs("Connected !\n", stdout);

  while ((read_byte = read(s, buf, sizeof(buf))) > 0) {
    write(STDOUT_FILENO, buf, read_byte);
  }

  if (read_byte == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  if (shutdown(s, SHUT_RDWR) == -1) {
    perror("shutdown");
    exit(EXIT_FAILURE);
  }

  if (close(s) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
