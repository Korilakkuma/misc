#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t id = getpid();

  fprintf(stdout, "current process ID is `%d`.\n", id);

  return 0;
}
