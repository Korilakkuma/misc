#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

static int logout_from_tty(char *line);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("sudo ./logout ttyname\n", stderr);
    exit(EXIT_FAILURE);
  }

  logout_from_tty(argv[1]);

  return 0;
}

static int logout_from_tty(char *line) {
  int fd;

  struct utmp utmp_record;

  size_t size = sizeof(struct utmp);

  int result = -1;

  if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
  // if ((fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror("open");
    return result;
  }

  while (read(fd, &utmp_record, size) == size) {
    if (strncmp(utmp_record.ut_line, line, sizeof(utmp_record.ut_line)) == 0) {
      utmp_record.ut_type = DEAD_PROCESS;

      if (time(&utmp_record.ut_tv.tv_sec) != -1) {
        if (lseek(fd, -size, SEEK_CUR) != -1) {
          if (write(fd, &utmp_record, size) == size) {
            result = 0;
          } else {
            perror("write");
          }
        } else {
          perror("write");
        }
      }

      break;
    }
  }

  if (close(fd) == -1) {
    perror("close");
    return result;
  }

  return result;
}
