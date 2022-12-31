#include "app_server.h"

static char buf[BUF_SIZE];

typedef struct {
  bool is_active;
  char name[16];
  struct sockaddr_in address;
} TALKER;

static TALKER talkers[MAX_ATTENDANTS];

static int app_socket;
static char buf[BUF_SIZE];

static struct sockaddr_in to;
static struct sockaddr_in from;

static ssize_t length_of_to;
static ssize_t length_of_from;

static void login(void);
static void logout(void);
static int find_free_slot(void);
static void distribute(void);
static void end(int sig);

int app_server_init(in_port_t port) {
  memset((char *)&to, 0, sizeof(to));

  to.sin_family      = AF_INET;
  to.sin_addr.s_addr = htonl(INADDR_ANY);
  to.sin_port        = htons(port);

  if ((app_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  if (bind(app_socket, (struct sockaddr *)&to, (socklen_t)sizeof(to)) == -1) {
    perror("bind");
    return -1;
  }

  fputs("Successfully bound !\n", stdout);
  fflush(stdout);

  for (int i = 0; i < MAX_ATTENDANTS; i++) {
    talkers[i].is_active = FREE;
  }

  signal(SIGINT, end);

  return 0;
}

void app_server_loop(void) {
  while (1) {
    length_of_from = sizeof(from);
    length_of_to   = recvfrom(app_socket, buf, (size_t)BUF_SIZE, 0, (struct sockaddr *)&from, (socklen_t *)&length_of_from);

    switch (buf[0]) {
      case LOGIN:
        login();
        break;
      case LOGOUT:
        logout();
        break;
      case DATA:
        distribute();
        break;
      default:
        break;
    }
  }
}

static void login(void) {
  int slot;

  if ((slot = find_free_slot()) == -1) {
    buf[0] = CANNOT_LOGIN;
    sendto(app_socket, buf, (size_t)2, 0, (struct sockaddr *)&from, (socklen_t)sizeof(from));
    return;
  }

  talkers[slot].is_active = USED;
  strcpy(talkers[slot].name, (buf + 1));
  talkers[slot].address = from;

  printf("talkers[%d]: %s logged in\n", slot, talkers[slot].name);

  buf[0] = LOGIN;
  sprintf((buf + 1), "%02d\n", slot);
  sendto(app_socket, buf, (size_t)4, 0, (struct sockaddr *)&from, (socklen_t)sizeof(from));
}

static void logout(void) {
  int slot;

  sscanf((buf + 1), "%02d", &slot);

  talkers[slot].is_active = FREE;

  printf("talkers[%d]: %s logged out\n", slot, talkers[slot].name);
}

static int find_free_slot(void) {
  for (int slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (!talkers[slot].is_active) {
      return slot;
    }
  }

  return -1;
}

static void distribute(void) {
  buf[0] = DATA;

  for (int slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (!talkers[slot].is_active) {
      continue;
    }

    sendto(app_socket, buf, (size_t)length_of_to, 0, (struct sockaddr *)&talkers[slot].address, (socklen_t)sizeof(from));
  }
}

static void end(int sig) {
  buf[0] = END;

  for (int slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (!talkers[slot].is_active) {
      continue;
    }

    sendto(app_socket, buf, (size_t)1, 0, (struct sockaddr *)&talkers[slot].address, (socklen_t)sizeof(from));
  }

  exit(EXIT_SUCCESS);
}
