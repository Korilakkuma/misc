#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./rmdir directory_name", stderr);
    exit(EXIT_FAILURE);
  }

  char *dir = argv[1];

  if (rmdir(dir) == -1) {
    int prev_errno = errno;

    perror("rmdir");

    if (prev_errno == ENOTEMPTY) {
      char command[64];

      sprintf(command, "ls -ltrAh %s\n", dir);

      fprintf(stdout, "%s", command);

      system(command);

      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stdout, "rmdir %s\n", dir);
  }

  return 0;
}
