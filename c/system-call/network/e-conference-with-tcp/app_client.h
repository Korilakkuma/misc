#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <curses.h>
#include "socketlib.h"

#define PORT            (in_port_t)18888
#define HOSTNAME_LENGTH 64

#define BUF_SIZE 80

#define SEND_WIN_WIDTH  60
#define SEND_WIN_HEIGHT 1

#define RECEIVE_WIN_WIDTH  60
#define RECEIVE_WIN_HEIGHT 13

#define CLEAR ' '

extern void app_client_init(int sock);
extern void app_client_loop(void);
