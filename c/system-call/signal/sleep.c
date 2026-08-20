#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void alarm_handler(int sig) {
  // Noop
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./sleep sec\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long long sec = strtoull(argv[1], NULL, 10);

  sigset_t new_sigset;
  sigset_t current_sigset;

  sigemptyset(&new_sigset);
  sigaddset(&new_sigset, SIGALRM);
  signal(SIGALRM, alarm_handler);
  sigprocmask(SIG_BLOCK, &new_sigset, &current_sigset);
  alarm(sec);
  sigsuspend(&current_sigset);  // sigprocmask(SIG_SETMASK, mask, &current_sigset) + pause()
  sigprocmask(SIG_SETMASK, &current_sigset, NULL);

  return 0;
}
