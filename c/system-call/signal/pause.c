#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
  fputs("Receive SIGINT\n", stdout);
}

int main(void) {
  fputs("pause until receiving signal ...\n", stdout);

  signal(SIGINT, handler);

  pause();
}
