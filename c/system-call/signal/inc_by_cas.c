#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t n = 1;

void handler(int sig) {
  fprintf(stdout, "signal is %d. value is %d\n", sig, n);
}

bool CAS(int *addr, int val, int new_value) {
  if (*addr == val) {
    *addr = new_value;
    return true;
  }

  return false;
}

void inc(int *p) {
  int val;

  do {
    val = *p;
  } while (!CAS(p, val, (val + 1)));
}

int main(void) {
  volatile sig_atomic_t *p = &n;

  signal(SIGINT, handler);

  sleep(1);

  inc((int *)p);

  fprintf(stdout, "p = %d\n", *p);

  return 0;
}
