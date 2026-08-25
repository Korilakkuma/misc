#include <signal.h>
#include <stdio.h>

static volatile sig_atomic_t counter_1 = 0;
static volatile sig_atomic_t counter_2 = 0;

void handler(int sig) {
  ++counter_1;
  ++counter_2;
}

int main(void) {
  sigset_t set;
  sigset_t old_set;

  sigemptyset(&set);
  sigaddset(&set, SIGINT);

  signal(SIGINT, handler);

  for (sig_atomic_t i = 0; i < 10000000; i++) {
    sigprocmask(SIG_BLOCK, &set, &old_set);

    ++counter_1;

    sigprocmask(SIG_SETMASK, &old_set, NULL);
  }

  sigprocmask(SIG_BLOCK, &set, &old_set);

  fprintf(stdout, "counter 1 = %d\n", counter_1);
  fprintf(stdout, "counter 2 = %d\n", counter_2);

  sigprocmask(SIG_SETMASK, &old_set, NULL);
}
