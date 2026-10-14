#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <uuid/uuid.h>

static void ls(char *directory_name);
static void print_stat(char *filename);
static void print_mode(mode_t mode);
static char *uid_to_username(uid_t uid);
static char *gid_to_groupname(gid_t gid);

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
    print_stat(entry->d_name);
  }

  if (closedir(dir) == -1) {
    perror("closedir");
    return;
  }
}

static void print_stat(char *filename) {
  struct stat stat_buf;

  if (stat(filename, &stat_buf) == -1) {
    perror("stat");
    return;
  }

  print_mode(stat_buf.st_mode);
  fprintf(stdout, "%4d ", stat_buf.st_nlink);
  fprintf(stdout, "%-8s ", uid_to_username(stat_buf.st_uid));
  fprintf(stdout, "%-8s ", gid_to_groupname(stat_buf.st_gid));
  fprintf(stdout, "%8lld ", stat_buf.st_size);
  // fprintf(stdout, "%.12s ", (4 + ctime(stat_buf.st_mtimespec.tv_nsec)));
  fprintf(stdout, "%s\n", filename);
}

static void print_mode(mode_t mode) {
  char mode_chars[] = "----------";

  if ((S_IFWHT & mode) & S_IFDIR) {
    mode_chars[0] = 'd';
  }

  if ((S_IFWHT & mode) & S_IFCHR) {
    mode_chars[0] = 'c';
  }

  if ((S_IFWHT & mode) & S_IFBLK) {
    mode_chars[0] = 'b';
  }

  if ((S_IFWHT & mode) & S_IFLNK) {
    mode_chars[0] = '@';
  }

  if (S_IRUSR & mode) {
    mode_chars[1] = 'r';
  }

  if (S_IWUSR & mode) {
    mode_chars[2] = 'w';
  }

  if (S_IXUSR & mode) {
    mode_chars[3] = 'x';
  }

  if (S_IRGRP & mode) {
    mode_chars[4] = 'r';
  }

  if (S_IWGRP & mode) {
    mode_chars[5] = 'w';
  }

  if (S_IXGRP & mode) {
    mode_chars[6] = 'x';
  }

  if (S_IROTH & mode) {
    mode_chars[7] = 'r';
  }

  if (S_IWOTH & mode) {
    mode_chars[8] = 'w';
  }

  if (S_IXOTH & mode) {
    mode_chars[9] = 'x';
  }

  fprintf(stdout, "%s", mode_chars);
}

static char *uid_to_username(uid_t uid) {
  struct passwd *p;

  static char fallback[10];

  if ((p = getpwuid(uid)) == NULL) {
    sprintf(fallback, "%d", uid);

    return fallback;
  }

  return p->pw_name;
}

static char *gid_to_groupname(gid_t gid) {
  struct group *p;

  static char fallback[10];

  if ((p = getgrgid(gid)) == NULL) {
    sprintf(fallback, "%d", gid);

    return fallback;
  }

  return p->gr_name;
}
