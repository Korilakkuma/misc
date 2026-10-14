#include <dirent.h>
#include <stdio.h>

static void ls(char *directory_name);

int main(int argc, char **argv) {
  if (argc == 1) {
    ls(".");
  } else {
    while (--argc) {
      fprintf(stdout, "%s:\n", *(++argv));

      ls(*argv);
    }
  }

  return 0;
}

static void ls(char *directory_name) {
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir(directory_name)) == NULL) {
    perror("opendir");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    fprintf(stdout, "%s\n", entry->d_name);
  }

  if (closedir(dir) == -1) {
    perror("closedir");
    return;
  }
}
