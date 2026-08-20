#include <signal.h>
#include <stdio.h>

// static unsigned long long counter = 0;
static volatile unsigned long long counter = 0;

void handler(int sig) {
  fprintf(stdout, "counter = %lld\n", counter);
}

int main(void) {
  signal(SIGINT, handler);

  for (unsigned long long i = 0; i < 10000000000; i++) {
    ++counter;
  }

  fprintf(stdout, "counter = %lld\n", counter);

  return 0;
}
