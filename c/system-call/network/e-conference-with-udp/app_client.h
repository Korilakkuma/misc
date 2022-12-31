#include <curses.h>
#include "app.h"

#define HOSTNAME_LENGTH 64

#define NAME_LENGTH        12
#define SEND_WIN_WIDTH     60
#define SEND_WIN_HEIGHT    1
#define RECEIVE_WIN_WIDTH  60
#define RECEIVE_WIN_HEIGHT 13

extern int setup_client(char *hostname, in_port_t port);
extern void app_client_init(int sock);
extern void app_client_loop(void);
