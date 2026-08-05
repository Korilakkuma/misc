#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

inline void hint_loop(void) {
  for (int i = 0; i < 0xfffff; i++) {
    ;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./sleep sec\n", stderr);
    exit(EXIT_FAILURE);
  }

  unsigned long long sec = strtoull(argv[1], NULL, 10);

  alarm(sec);

  while (1) {
    hint_loop();
  }

  return 0;
}
