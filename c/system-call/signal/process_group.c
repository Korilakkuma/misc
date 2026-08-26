#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t new_pid;

  new_pid = fork();

  if (new_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (new_pid == 0) {
    pid_t pid = setpgrp();

    while (1) {
      fprintf(stdout, "This PID is %d (PGID is %d)\n", pid, getpgrp());
      sleep(1);
    }

    exit(EXIT_SUCCESS);
  }

  while (1) {
    fprintf(stdout, "This PID is %d\n", getpid());
    sleep(1);
  }

  return 0;
}
