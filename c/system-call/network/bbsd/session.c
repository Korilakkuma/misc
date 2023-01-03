#include "app.h"

void session(int sock) {
  int number_of_fds;
  fd_set mask;
  char buf[BUF_SIZE];

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);
  FD_SET(sock, &mask);

  number_of_fds = sock + 1;

  while (1) {
    fd_set readfds = mask;

    select(number_of_fds, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, (fd_set *)&readfds)) {
      ssize_t s = read(STDIN_FILENO, buf, (size_t)BUF_SIZE);
      write(sock, buf, (size_t)s);

      if (buf[0] == '9') {
        exit(EXIT_SUCCESS);
      }
    }

    if (FD_ISSET(sock, (fd_set *)&readfds)) {
      ssize_t s = read(sock, buf, (size_t)BUF_SIZE);
      write(STDOUT_FILENO, buf, (size_t)s);
    }
  }
}
