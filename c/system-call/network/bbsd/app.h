#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

#define PORT (in_port_t)18888

#define BUF_SIZE        1024
#define HOSTNAME_LENGTH 64
#define FILENAME_LENGTH 128
