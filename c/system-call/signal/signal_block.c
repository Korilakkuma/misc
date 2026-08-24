#include <signal.h>
#include <stdio.h>

static volatile long long counter_1 = 0;
static volatile long long counter_2 = 0;

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

  for (long long i = 0; i < 10000000; i++) {
    sigprocmask(SIG_BLOCK, &set, &old_set);

    ++counter_1;

    sigprocmask(SIG_SETMASK, &old_set, NULL);
  }

  sigprocmask(SIG_BLOCK, &set, &old_set);

  fprintf(stdout, "counter 1 = %lld\n", counter_1);
  fprintf(stdout, "counter 2 = %lld\n", counter_2);

  sigprocmask(SIG_SETMASK, &old_set, NULL);
}
