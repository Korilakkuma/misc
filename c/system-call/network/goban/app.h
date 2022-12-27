#include <sys/types.h>
#include "socketlib.h"

#define PORT            (in_port_t)18888
#define HOSTNAME_LENGTH 64
#define DATA_SIZE       (ssize_t)10

#define NOW_TURN    1
#define ENDED_TURN -1

#define PROMPT "> "

extern void app_init(int sock, char stone, char peer_stone);
extern void app_destroy(void);
extern void app_show(void);
extern int app_peer_turn(void);
extern int app_turn(void);
