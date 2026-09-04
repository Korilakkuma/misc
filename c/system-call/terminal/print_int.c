#include <stdio.h>

// ./print_int | cat -v
// stty -isig; ./print_int | cat -v
int main(void) {
  fputc(3, stdout);
  fputc('\n', stdout);
}
