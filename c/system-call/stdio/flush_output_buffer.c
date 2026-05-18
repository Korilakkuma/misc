#include <stdio.h>
#include <unistd.h>

int main(void) {
  for (int i = 5; i > 0; i--) {
    // puts("count ");  // No line buffering (Therefore, flush output buffer)
    fprintf(stdout, "%d ", i);
    fflush(stdout);
    sleep(1);
  }

  fputs("done.\n", stdout);

  return 0;
}
