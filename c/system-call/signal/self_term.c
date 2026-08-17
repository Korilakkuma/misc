#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  if (raise(SIGTERM) == -1) {
    perror("kill");
    exit(EXIT_FAILURE);
  }

  return 0;
}
