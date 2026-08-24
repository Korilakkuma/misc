#include <signal.h>
#include <stdio.h>

int is_sigint_blocked(void) {
  sigset_t old_set;

  sigprocmask(SIG_SETMASK, NULL, &old_set);

  return sigismember(&old_set, SIGINT);
}

int main(void) {
  int blocked = 0;;

  sigset_t set;

  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask(SIG_BLOCK, &set, NULL);

  blocked = is_sigint_blocked();

  fprintf(stdout, "%s (%d)\n", (blocked ? "blocked" : "non blocked"), blocked);

  sigprocmask(SIG_UNBLOCK, &set, NULL);

  blocked = is_sigint_blocked();

  fprintf(stdout, "%s (%d)\n", (blocked ? "blocked" : "non blocked"), blocked);
}
