#include <stdio.h>
#include <unistd.h>

int main(void) {
  fputs("Begin run\n", stdout);

  sleep(1);

  fputs("step 1 failed\n", stdout);

  sleep(1);

  for (int i = 2; i < 1000; i++) {
    fprintf(stdout, "step %d succeeded\n", i);
    sleep(1);
  }

  fputs("End run\n", stdout);

  return 0;
}
