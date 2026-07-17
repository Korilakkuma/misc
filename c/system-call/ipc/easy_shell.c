#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char command0[1024];
  char command1[1024];

  char tmp[1024];

  int fd[2];  // for pipe

  int status;

  pid_t zombie_pid;

  while (1) {
    while ((zombie_pid = waitpid(-1, &status, WNOHANG)) > 0) {
      fprintf(stderr, "process (PID %d) is salvaged.\n", zombie_pid);
    }

    if ((zombie_pid == -1) && (errno != ECHILD)) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }

    fputs("@ ", stdout);

    fgets(tmp, sizeof(tmp), stdin);

    char *p;

    int ch;

    if ((p = strchr(tmp, '\n')) == NULL) {
      while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n') {
          break;
        }
      }
    } else {
      *p = '\0';
    }

    int is_bg_job = 0;

    if (tmp[strlen(tmp) - 1] == '&') {
      is_bg_job = 1;

      tmp[strlen(tmp) - 2] = '\0';
    }

    int is_pipe = 0;

    if ((p = strchr(tmp, '|')) != NULL) {
      *p = '\0';

      while (isspace(tmp[strlen(tmp) - 1])) {
        tmp[strlen(tmp) - 1] = '\0';
      }

      ++p;

      while (isspace(*p)) {
        ++p;
      }

      strcpy(command0, tmp);
      strcpy(command1, p);

      is_pipe = 1;
    } else {
      strcpy(command0, tmp);
    }

    pid_t new_process_id = fork();

    if (new_process_id == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if (new_process_id == 0) {
      if (is_pipe) {
        if (pipe(fd) == -1) {
          perror("pipe");
          exit(EXIT_FAILURE);
        }

        new_process_id = fork();

        if (new_process_id == -1) {
          perror("fork");
          exit(EXIT_FAILURE);
        }

        if (new_process_id == 0) {
          if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
          }

          close(fd[1]);
          close(fd[0]);

          execlp(command0, command0, (char *)0);
          perror(command0);
          exit(EXIT_FAILURE);
        } else {
          if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
          }

          close(fd[0]);
          close(fd[1]);

          execlp(command1, command1, (char *)0);
          perror(command1);
          exit(EXIT_FAILURE);
        }
      }

      execlp(command0, command0, (char *)0);
      perror(command0);
      exit(EXIT_FAILURE);
    }

    if (is_bg_job) {
      continue;
    }

    pid_t wait_process_id = wait(&status);

    if (wait_process_id == (pid_t)-1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
      int s = WEXITSTATUS(status);
      // do something ...
      fprintf(stdout, "status code is %d.\n", s);
    }

    if (WIFSIGNALED(status)) {
      int signal = WTERMSIG(status);

      // do something ...
      fprintf(stdout, "signal is %d.\n", signal);
    }
  }

  return 0;
}
