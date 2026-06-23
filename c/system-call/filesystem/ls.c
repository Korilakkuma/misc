#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./ls dirname\n", stdout);
    exit(EXIT_FAILURE);
  }

  const char *dirname = argv[1];

  DIR *dp;

  struct dirent *entry;

  if ((dp = opendir(dirname)) == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  errno = 0;

  while ((entry = readdir(dp)) != NULL) {
    fprintf(stdout, "%lld %s\n", entry->d_ino, entry->d_name);
  }

  if (errno != 0) {
    perror("readdir");
  }

  if (closedir(dp) == -1) {
    perror("closedir");
    exit(EXIT_FAILURE);
  }

  return 0;;
}
