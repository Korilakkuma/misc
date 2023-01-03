#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "app.h"

extern void service(int sock, char *name);

int main(int argc, char **argv) {
  struct sockaddr_in server;
  int socket_for_waiting;
  int ssocket;
  pid_t pid;

  memset((char *)&server, 0, sizeof(server));

  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port        = htons(PORT);

  if ((socket_for_waiting = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (bind(socket_for_waiting, (struct sockaddr *)&server, (socklen_t)sizeof(server)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  listen(socket_for_waiting, 5);

  fputs("Successfully bound, now waiting ...\n", stdout);
  fflush(stdout);

  while (1) {
    ssocket = accept(socket_for_waiting, NULL, NULL);

    if ((pid = fork()) == (pid_t)0) {
      close(socket_for_waiting);
      service(ssocket, argv[0]);
      exit(EXIT_SUCCESS);
    }

    close(ssocket);
    fprintf(stdout, "Child process [%d] is now servicing\n", (int)pid);
  }

  return 0;
}
