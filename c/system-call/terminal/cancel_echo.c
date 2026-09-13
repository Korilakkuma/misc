#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
  struct termios new_term;
  struct termios term;

  tcgetattr(STDIN_FILENO, &term);

  new_term = term;

  new_term.c_lflag &= ~ECHO;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  int ch;

  while ((ch = getchar()) != EOF) {
    putchar(ch);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  return 0;
}
