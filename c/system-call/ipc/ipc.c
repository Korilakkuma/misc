#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./ipc message\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *message = argv[1];

  char buf[1024];

  int fd[2];

  pid_t new_pid;

  int status;

  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((new_pid = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (new_pid == 0) {
    close(fd[1]);

    fputs("from parent process\n", stdout);

    ssize_t read_byte = read(fd[0], buf, sizeof(buf));
    write(STDOUT_FILENO, buf, read_byte);

    close(fd[0]);
  } else {
    close(fd[0]);

    write(fd[1], message, strlen(message));

    close(fd[1]);

    wait(&status);
  }

  return 0;
}
