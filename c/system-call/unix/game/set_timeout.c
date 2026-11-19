#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void alarm_handler(int sig) {
  fprintf(stdout, "timeout (%d) !\n", sig);
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./set_timeout msec\n", stderr);
    exit(EXIT_FAILURE);
  }

  struct itimerval current_timer;
  struct itimerval new_timer;

  unsigned long msec = strtoul(argv[1], NULL, 10);

  unsigned long sec  = msec / 1000;
  unsigned long usec = (msec % 1000) * 1000UL;

  new_timer.it_value.tv_sec  = sec;
  new_timer.it_value.tv_usec = usec;

  new_timer.it_interval.tv_sec  = 0;
  new_timer.it_interval.tv_usec = 0;

  if (getitimer(ITIMER_REAL, &current_timer) == -1) {
    perror("getitimer");
    exit(EXIT_FAILURE);
  }

  if (setitimer(ITIMER_REAL, &new_timer, &current_timer) == -1) {
    perror("setitimer");
    exit(EXIT_FAILURE);
  }

  signal(SIGALRM, alarm_handler);

  char buf[1024];

  fputs("> ", stdout);

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    fputs("> ", stdout);
  }

  return 0;
}
