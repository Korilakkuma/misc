#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern int setup_server(in_port_t port);
extern int setup_client(char *hostname, in_port_t port);
extern int setup_multiple_server(in_port_t port, int queue_size);
extern int multiple_accept(int socket_for_waiting, int limit, void (*callback)(int n, int sock));
extern char *trim_newline(char *str, int len);
