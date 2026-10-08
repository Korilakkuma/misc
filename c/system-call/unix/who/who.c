#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST

#define NUMBER_OF_RECORDS 16
#define UTMP_SIZE         (sizeof(struct utmp))

static char utmp_buf[NUMBER_OF_RECORDS * UTMP_SIZE];
static int number_of_records;
static int current_record;

static void who(struct utmp *record);
static struct utmp *utmp_next(int fd);
static int utmp_read(int fd);

int main(int argc, char **argv) {
  struct utmp *current_utmp;
  int fd;

  if ((fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  number_of_records = 0;
  current_record    = 0;

  while ((current_utmp = utmp_next(fd)) != (struct utmp *)NULL) {
    who(current_utmp);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}

static void who(struct utmp *record) {
  if (record->ut_type != USER_PROCESS) {
    return;
  }

  fprintf(stdout, "%-20.20s %-10.10s %12.12s", record->ut_user, record->ut_line, ctime(&record->ut_tv.tv_sec));

#ifdef SHOWHOST
  if (record->ut_host[0] != '\0') {
    fprintf(stdout, " (%s)", record->ut_host);
  }
#endif

  fputc('\n', stdout);
}

static struct utmp *utmp_next(int fd) {
  struct utmp *p;

  if (fd == -1) {
    return ((struct utmp *)NULL);
  }

  if (current_record == number_of_records) {
    if (utmp_read(fd) == 0) {
      return ((struct utmp *)NULL);
    }
  }

  p = (struct utmp *)&utmp_buf[current_record * UTMP_SIZE];

  ++current_record;

  return p;
}

static int utmp_read(int fd) {
  ssize_t read_byte = read(fd, utmp_buf, (NUMBER_OF_RECORDS * UTMP_SIZE));

  number_of_records = read_byte / UTMP_SIZE;

  current_record = 0;

  return number_of_records;
}
