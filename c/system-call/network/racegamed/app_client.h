#include <stdbool.h>
#include <curses.h>
#include <signal.h>
#include "app.h"

extern void app_client_init(int sock);
extern void app_client_loop(void);
