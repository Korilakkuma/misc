#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./term pid\n", stdout);
    exit(EXIT_FAILURE);
  }

  pid_t pid = (pid_t)strtoll(argv[1], NULL, 10);

  if (kill(pid, SIGTERM) == -1) {
    perror("kill");
    exit(EXIT_FAILURE);
  }

  return 0;
}
