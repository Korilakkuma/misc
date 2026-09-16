#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
  struct termios term;
  struct termios new_term;

  tcgetattr(STDIN_FILENO, &term);

  new_term = term;

  new_term.c_iflag &= ~(BRKINT | ICRNL | INLCR | INPCK | ISTRIP | IXON);
  new_term.c_oflag &= ~(OPOST);
  new_term.c_lflag &= ~(ECHO | ECHOE | ECHONL | ICANON | IEXTEN);
  new_term.c_cflag |= CS8;

  new_term.c_cc[VMIN]  = 1;
  new_term.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  int ch;

  while ((ch = getchar()) != EOF) {
    if (ch == 'q') {
      break;
    }

    putchar(ch);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  return 0;
}
