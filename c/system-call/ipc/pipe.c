#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./pipe command command\n", stderr);
    exit(EXIT_FAILURE);
  }

  int fd[2];

  pid_t new_pid;

  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((new_pid = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (new_pid == 0) {
    if (dup2(fd[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    close(fd[1]);
    close(fd[0]);

    execlp(argv[1], argv[1], (char *)NULL);
  } else {
    if (dup2(fd[0], STDIN_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);

    execlp(argv[2], argv[2], (char *)NULL);
  }

  return 0;
}
