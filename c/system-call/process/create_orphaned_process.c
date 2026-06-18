#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t orphaned_pid = fork();

  if (orphaned_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (orphaned_pid == 0) {
    sleep(5);
    fprintf(stdout, "This process is orphaned process (PID is %d). PPID is %d.\n", getpid(), getppid());
    exit(EXIT_SUCCESS);
  }

  fputs("Parent process will exit as soon as.\n", stdout);

  exit(EXIT_SUCCESS);

  return 0;
}
