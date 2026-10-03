#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST

static void who(struct utmp *record);

int main(int argc, char **argv) {
  struct utmp current_utmp;
  int fd;

  size_t size_of_record = sizeof(struct utmp);

  if ((fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while (read(fd, &current_utmp, size_of_record) > 0) {
    who(&current_utmp);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}

static void who(struct utmp *record) {
  fprintf(stdout, "%-20.20s %-10.10s %10ld", record->ut_user, record->ut_line, record->ut_tv.tv_sec);

#ifdef SHOWHOST
  fprintf(stdout, "(%s)", record->ut_host);
#endif

  fputc('\n', stdout);
}
