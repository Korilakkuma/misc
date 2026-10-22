#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int pwd(void);
static int pwd_loop(ino_t current_ino);
static int save_dirname(ino_t target_ino, char dirname_buf[]);

int main(void) {
  pwd();
  return 0;
}

static int pwd(void) {
  struct stat current_stat;

  if (stat(".", &current_stat) == -1) {
    perror("stat");
    return -1;
  }

  pwd_loop(current_stat.st_ino);

  return 0;
}

static int pwd_loop(ino_t current_ino) {
  struct stat current_stat;
  struct stat parent_stat;

  char dirname_buf[BUFSIZ];

  if (stat(".", &current_stat) == -1) {
    perror("stat");
    return -1;
  }

  if (stat("..", &parent_stat) == -1) {
    perror("stat");
    return -1;
  }

  // `/` directory ?
  if (current_stat.st_ino != parent_stat.st_ino) {
    if (chdir("..") == -1) {
      perror("chdir");
      return -1;
    }

    save_dirname(current_ino, dirname_buf);

    pwd_loop(parent_stat.st_ino);

    if (strlen(dirname_buf) > 0) {
      fprintf(stdout, "/%s", dirname_buf);
    }
  }

  return 0;
}

static int save_dirname(ino_t target_ino, char dirname_buf[]) {
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir(".")) == NULL) {
    perror("opendir");
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_ino == target_ino) {
      strncpy(dirname_buf, entry->d_name, (strlen(entry->d_name) + 1));

      dirname_buf[strlen(entry->d_name) + 1] = '\0';

      break;
    }
  }

  if (closedir(dir) == -1) {
    perror("closedir");
    return -1;
  }

  return 0;
}
