#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <syslog.h>
#include "app.h"

extern void service(int sock, char *name);

static void end(int sig);

int main(int argc, char **argv) {
  struct sockaddr_in server;
  int socket_for_waiting;
  int ssocket;;
  pid_t pid;

  if (fork() != 0) {
    // End the 1st generation parent process
    exit(EXIT_SUCCESS);
  }

  // Child process becomes session leader and group leader
  setsid();

  if (fork() != 0) {
    // End the 2nd generation parent process
    exit(EXIT_SUCCESS);
  }

  chdir("/tmp");

  // Detach ttys
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // Daemon

  signal(SIGTERM, end);
  openlog(argv[0], LOG_PID, LOG_USER);

  memset((char *)&server, 0, sizeof(server));

  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port        = htons(PORT);

  if ((socket_for_waiting = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    syslog(LOG_ERR, "`socket` failed");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_for_waiting, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    syslog(LOG_ERR, "`bind` failed");
    exit(EXIT_FAILURE);
  }

  listen(socket_for_waiting, 5);
  syslog(LOG_INFO, "`listen` successed");

  while (1) {
    ssocket = accept(socket_for_waiting, NULL, NULL);

    if ((pid = fork()) == (pid_t)0) {
      close(socket_for_waiting);
      service(ssocket, argv[0]);
      exit(EXIT_SUCCESS);
    }

    close(ssocket);
  }

  return 0;
}

static void end(int sig) {
  syslog(LOG_INFO, "Service terminated");
  closelog();
  exit(EXIT_SUCCESS);
}
