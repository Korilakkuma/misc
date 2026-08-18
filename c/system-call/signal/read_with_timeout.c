#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t read_with_timeout(int fd, void *buf, size_t count, unsigned int seconds);
void alarm_handler(int sig);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./read_with_timeout seconds\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned int seconds = (unsigned int)strtoul(argv[1], NULL, 10);

  char buf[1024];

  fputs("Type > ", stdout);

  fflush(NULL);

  ssize_t read_byte = read_with_timeout(STDIN_FILENO, buf, sizeof(buf), seconds);

  if (read_byte == -1) {
    fputs("Timeout\n", stderr);
  } else {
    write(STDOUT_FILENO, buf, read_byte);
  }

  return 0;
}

ssize_t read_with_timeout(int fd, void *buf, size_t count, unsigned int seconds) {
  ssize_t read_byte;

  struct sigaction act;

  act.sa_handler = alarm_handler;
  act.sa_flags   = 0;

  sigemptyset(&act.sa_mask);
  sigaction(SIGALRM, &act, NULL);

  alarm(seconds);

  read_byte = read(fd, buf, count);

  alarm(0);

  return read_byte;
}

void alarm_handler(int sig) {
  // Noop
}
