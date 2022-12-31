#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include "socketlib.h"

#define LOGIN        (uint8_t)100
#define LOGOUT       (uint8_t)101
#define DATA         (uint8_t)102
#define END          (uint8_t)199
#define CAN_LOGIN    (uint8_t)200
#define CANNOT_LOGIN (uint8_t)201

#define USED true
#define FREE false

#define PORT     (in_port_t)18888
#define BUF_SIZE (size_t)256

#define CLEAR ' '
