#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) {
  pid_t pid;

  while ((pid = waitpid(-1, NULL, WNOHANG)) != 0) {
    if (pid == -1) {
      if (errno == ECHILD) {
        break;
      }

      if (errno == EINTR) {
        continue;
      }

      perror("waitpid");
      exit(EXIT_FAILURE);
    }

    fprintf(stdout, "pid = %d\n", pid);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./async_wait 'command'\n", stderr);
    exit(EXIT_FAILURE);
  }

  pid_t new_pid = fork();

  if (new_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  signal(SIGCHLD, handler);

  char *command = argv[1];

  if (new_pid == 0) {
    execvp(command, &argv[1]);
  } else {
    while (1) {
      fputs(".", stdout);
      fflush(NULL);
      sleep(1);
    }
  }

  return 0;
}
