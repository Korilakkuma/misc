#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "socketlib.h"

#define PORT                     (in_port_t)18888
#define BUF_SIZE                 80
#define MAX_NUMBER_OF_ATTENDANTS 5
#define NAME_LENGTH              32

extern void enter(int entry_number, int fd);
extern void app_server_init(int num, int max_fd);
extern void app_server_loop(void);
