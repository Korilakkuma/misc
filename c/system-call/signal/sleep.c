#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void alarm_handler(int sig) {
  // Noop
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./sleep sec\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long long sec = strtoull(argv[1], NULL, 10);

  signal(SIGALRM, alarm_handler);

  alarm(sec);
  pause();

  return 0;
}
