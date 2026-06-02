#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t new_pid;

  int shared_value = 527;

  fprintf(stdout, "current process ID is %d.\n", getpid());

  fflush(NULL);

  if ((new_pid = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (new_pid == 0) {
    // Child process
    fprintf(stdout, "child process ID is %d. shared value is %d.\n", getpid(), shared_value);

    sleep(1);

    for (int i = 0; i < 10; i++) {
      fprintf(stdout, "<%d>", i);
      fflush(stdout);
      sleep(1);
    }

    exit(EXIT_SUCCESS);
  } else {
    // Parent process
    fprintf(stdout, "parent process ID is %d shared value is %d.\n", getpid(), shared_value);

    sleep(1);

    for (int i = 9; i >= 0; i--) {
      fprintf(stdout, "[%d]", i);
      fflush(stdout);
      sleep(2);
    }

    putc('\n', stdout);

    exit(EXIT_SUCCESS);
  }

  return 0;
}
