#include "app_server.h"

static char buf[BUF_SIZE];
static fd_set mask;
static int number_of_fds;
static int number_of_attendants;

typedef struct {
  int fd;
  char name[NAME_LENGTH];
} ATTENDANT;

static ATTENDANT attendants[MAX_NUMBER_OF_ATTENDANTS];

static void broadcast(int entry_number, ssize_t bytes);
static void end(void);

void enter(int entry_number, int fd) {
  static char *type_prompt = "Type your name\n";
  static char *wait_prompt = "Wait ...\n";

  attendants[entry_number].fd = fd;
  memset(attendants[entry_number].name, 0, NAME_LENGTH);

  write(fd, type_prompt, (size_t)strlen(type_prompt));

  ssize_t s = read(fd, attendants[entry_number].name, (size_t)NAME_LENGTH);

  sprintf((attendants[entry_number].name + s - 1), "  -->  ");

  write(fd, wait_prompt, strlen(wait_prompt));
}

void app_server_init(int num, int max_fd) {
  static char *msg = "Communication Ready !\n";

  number_of_attendants = num;

  number_of_fds = max_fd + 1;

  FD_ZERO(&mask);
  FD_SET(STDIN_FILENO, &mask);

  for (int i = 0; i < number_of_attendants; i++) {
    FD_SET(attendants[i].fd, &mask);
  }

  for (int i = 0; i < number_of_attendants; i++) {
    write(attendants[i].fd, msg, strlen(msg));
  }
}

void app_server_loop(void) {
  fd_set readfds;

  while (1) {
    readfds = mask;

    select(number_of_fds, (fd_set *)&readfds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      end();
    }

    for (int i = 0; i < number_of_attendants; i++) {
      if (FD_ISSET(attendants[i].fd, &readfds)) {
        ssize_t n = read(attendants[i].fd, buf, (size_t)BUF_SIZE);
        broadcast(i, n);
      }
    }
  }
}

static void broadcast(int entry_number, ssize_t bytes) {
  for (int i = 0; i < number_of_attendants; i++) {
    write(attendants[i].fd, attendants[entry_number].name, (size_t)strlen(attendants[entry_number].name));
    write(attendants[i].fd, buf, (size_t)bytes);
  }
}

static void end(void) {
  for (int i = 0; i < number_of_attendants; i++) {
    write(attendants[i].fd, "q", (size_t)1);
  }

  for (int i = 0; i < number_of_attendants; i++) {
    close(attendants[i].fd);
  }

  exit(EXIT_SUCCESS);
}
