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

  if (new_pid != 0) {
    setpgid(new_pid, new_pid);
    tcsetpgrp(STDIN_FILENO, new_pid);
  }

  while (1) {
    fprintf(stdout, "This PID is %d\n", getpid());
    sleep(1);
  }

  return 0;
}
