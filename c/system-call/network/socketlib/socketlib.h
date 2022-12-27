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
extern int mserver_socket(in_port_t port, int num);
extern int mserver_maccept(int sock, int limit, void (*callback)(void));
extern char *trim_newline(char *str, int len);
