#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char MODES[11] = " ---------";

void setMode(const char *entry_name);

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
    setMode(entry->d_name);

    fprintf(stdout, "%lld %s %s\n", entry->d_ino, MODES, entry->d_name);
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

void setMode(const char *entry_name) {
  struct stat inode;

  if (stat(entry_name, &inode) == -1) {
    perror("stat");
    return;
  }

  // fprintf(stdout, "S_IFMT  is %o\n", S_IFMT);
  // fprintf(stdout, "S_IFWHT is %o\n", S_IFWHT);

  switch (inode.st_mode & S_IFWHT) {
    case S_IFREG: {
      MODES[0] = ' ';
      break;
    }

    case S_IFDIR: {
      MODES[0] = 'd';
      break;
    }

    case S_IFCHR: {
      MODES[0] = 'c';
      break;
    }

    case S_IFBLK: {
      MODES[0] = 'b';
      break;
    }

    case S_IFLNK: {
      MODES[0] = 'l';
      break;
    }

    case S_IFIFO: {
      MODES[0] = 'f';
      break;
    }

    case S_IFSOCK: {
      MODES[0] = 's';
      break;
    }
  }

  if (inode.st_mode & S_IRUSR) {
    MODES[1] = 'r';
  }

  if (inode.st_mode & S_IWUSR) {
    MODES[2] = 'w';
  }

  if (inode.st_mode & S_IXUSR) {
    MODES[3] = 'x';
  }

  if (inode.st_mode & S_ISUID) {
    MODES[3] = 's';
  }

  if (inode.st_mode & S_IRGRP) {
    MODES[4] = 'r';
  }

  if (inode.st_mode & S_IWGRP) {
    MODES[5] = 'w';
  }

  if (inode.st_mode & S_IXGRP) {
    MODES[6] = 'x';
  }

  if (inode.st_mode & S_ISGID) {
    MODES[6] = 's';
  }

  if (inode.st_mode & S_IROTH) {
    MODES[7] = 'r';
  }

  if (inode.st_mode & S_IWOTH) {
    MODES[8] = 'w';
  }

  if (inode.st_mode & S_IXOTH) {
    MODES[9] = 'x';
  }

  if (inode.st_mode & S_ISVTX) {
    MODES[9] = 't';
  }
}
