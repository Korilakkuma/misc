#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define GET "GET / HTTP/1.0\r\n\r\n"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./wget domain\n", stderr);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  struct addrinfo *addr;

  int s;

  ssize_t read_byte;

  char buf[1024];

  char *domain = argv[1];

  memset(&hints, 0, sizeof(hints));

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((read_byte = getaddrinfo(domain, "http", &hints, &addr)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(read_byte));
    exit(EXIT_FAILURE);
  }

  if ((s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (connect(s, addr->ai_addr, addr->ai_addrlen) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(addr);

  write(s, GET, strlen(GET));

  while ((read_byte = read(s, buf, sizeof(buf))) > 0) {
    if (write(STDIN_FILENO, buf, read_byte) != read_byte) {
      perror("write");
      exit(EXIT_FAILURE);
    }
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
