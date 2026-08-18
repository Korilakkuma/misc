#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
  fputs("@", stdout);
  fflush(stdout);
}

int main(void) {
  struct sigaction act;

  act.sa_handler = handler;
  act.sa_flags   = 0;

  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL);

  while (1) {
    sleep(1);

    fputs(".", stdout);
    fflush(stdout);
  }
}
