#include <stdio.h>
#include <sys/ioctl.h>

static int print_screen_dimensions(void);

int main(void) {
  print_screen_dimensions();

  return 0;
}

static int print_screen_dimensions(void) {
  struct winsize wbuf;

  if (ioctl(0, TIOCGWINSZ, &wbuf) == -1) {
    return -1;
  }

  fprintf(stdout, "%d rows x %d cols\n", wbuf.ws_row, wbuf.ws_col);
  fprintf(stdout, "%d wide x %d tall\n", wbuf.ws_xpixel, wbuf.ws_ypixel);

  return 0;
}
