#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "socketlib.h"

#define MAX_PLAYERS 5

#define PLAYER_SIZE 4

#define STAGE  0
#define POS_X  1
#define POS_Y  2
#define DAMAGE 3

#define GRADE_SIZE 16

#define ENTRY_NUMBER 0
#define ENTRY_NAME   1

#define MAX_STAGES   10
#define STAGE_WIDTH  10
#define STAGE_HEIGHT 20

#define MAX_DAMAGE 10

#define PORT            (in_port_t)18888
#define HOSTNAME_LENGTH 64

#define BUF_SIZE 80

#define UP    'k'
#define DOWN  'j'
#define LEFT  'h'
#define RIGHT 'l'
#define QUIT  'q'
#define CLEAR ' '
