#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t new_pid = fork();

  if (new_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (new_pid == 0) {
    fprintf(stdout , "[Child] PID = %d. Parent PID = %d.\n", getpid(), getppid());
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stdout , "[Parent] PID = %d. Parent PID = %d.\n", getpid(), getppid());

    for (;;) {
      sleep(1);
    }
  }

  return 0;
}
