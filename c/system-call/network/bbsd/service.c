#include "app.h"

#define DATA_DIR "/tmp/"
#define MENU     "1. a\n2. b\n3. c\n4. d\n9. exit\n"

static int process(int sock, int item);

void service(int sock, char *name) {
  pid_t pid;
  char buf[BUF_SIZE];
  int item;

  pid = getpid();

  sprintf(buf, "This is %s[%d] serving\n", name, (int)pid);

  write(sock, buf, (size_t)strlen(buf));

  do {
    write(sock, MENU, (size_t)strlen(MENU));
    read(sock, buf, (size_t)20);
    sscanf(buf, "%d", &item);
  } while (process(sock, item));

  close(sock);
}

static int process(int sock, int item) {
  int fd;

  ssize_t bytes;

  char filename[FILENAME_LENGTH];
  char buf[BUF_SIZE];

  if (item == 9) {
    return 0;
  }

  sprintf(filename, "%s/bbsd/data%d", DATA_DIR, item);

  if ((fd = open(filename, O_RDONLY)) == -1) {
    write(sock, "Invalid number !\n", 17);
    return 1;
  }

  while ((bytes = read(fd, buf, (size_t)BUF_SIZE)) != 0) {
    write(sock, buf, (size_t)bytes);
  }

  close(fd);

  return 1;
}
