#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>
#include "app.h"

#define SOCKET 0

extern void service(int sock, char *name);

int main(int argc, char **argv) {
  openlog(argv[0], LOG_PID, LOG_USER);
  syslog(LOG_INFO, "startup");

  service(SOCKET, argv[0]);

  syslog(LOG_INFO, "shutdown");
  closelog();

  shutdown(SOCKET, 2);

  return 0;
}
