#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
  fputs("@", stdout);
  fflush(stdout);
}

int main(void) {
  signal(SIGINT, handler);

  while (1) {
    sleep(1);

    fputs(".", stdout);
    fflush(stdout);
  }
}
