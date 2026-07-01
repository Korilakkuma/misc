#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./chown owner_id filename", stderr);
    exit(EXIT_FAILURE);
  }

  uid_t uid      = (unsigned int)strtoul(argv[1], NULL, 10);
  char *filename = argv[2];

  if (chown(filename, uid, -1) == -1) {
    perror("chown");
    exit(EXIT_FAILURE);
  }

  return 0;
}
