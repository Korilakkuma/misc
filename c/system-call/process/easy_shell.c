#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
  char command[1024];
  char tmp[1024];

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

    strcpy(command, tmp);

    pid_t new_process_id = fork();

    if (new_process_id == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if (new_process_id == 0) {
      execlp(command, command, (char *)0);
      perror(command);
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
