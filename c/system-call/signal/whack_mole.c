#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int out  = 0;
int hit  = 0;
int miss = 0;

void whack(int sig) {
  if (out) {
    putc('!', stdout);

    ++hit;

    out = 0;
  } else {
    ++miss;
  }
}

int main(void) {

  setbuf(stdout, NULL);
  srandom((unsigned long)time(NULL));

  signal(SIGINT, whack);

  fputs("*** Whack a Mole ***\nInterrupt (^C) to hit the mole. Game time is 15 seconds.\n", stdout);

  for (int i = 3; i > 0; i--) {
    fprintf(stdout, "%d ", i);

    sleep(1);
  }

  fputs("Start !\n", stdout);
  // fflush(NULL);

  for (int i = 0; i < 15; i++) {
    if (out) {
      if ((random() % 3) < 2) {
        out = 0;

        putc('_', stdout);
      }
    } else if ((random() % 6) == 0) {
      out = 1;

      putc('O', stdout);
    }

    sleep(1);
  }

  fputs("Game Over !\n", stdout);
  fprintf(stdout, "hit = %d, miss = %d\n", hit, miss);
  fprintf(stdout, "Score is %d points !\n", (hit - miss));

  return 0;
}
