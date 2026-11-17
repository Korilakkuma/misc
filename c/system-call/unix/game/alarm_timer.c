#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./alarm_timer\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long t = strtoul(argv[1], NULL, 10);

  if (alarm(t) == -1) {
    perror("alarm");
    exit(EXIT_FAILURE);
  }

  if (pause() == -1) {
    perror("pause");
    exit(EXIT_FAILURE);
  }

  return 0;
}
